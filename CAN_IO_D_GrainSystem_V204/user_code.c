//--------------------------------------------------------------------------
/// \file     user_code.c
/// \brief    user code
/// \author   user
/// \date     11.06.2016
/// \version  1.00
/// \comment  file to write user specific code
//--------------------------------------------------------------------------

#include "user_code.h"
#include "src/os_timer.h"

#include "global.h"

volatile uint16_t freq1 = 550;
volatile uint16_t freq2 = 1000;
volatile uint16_t freq3 = 36000;
volatile uint16_t freq4 = 800;

#define EE_CODE				0x01
#define DEALYALLARMI		20
#define RPM_SIMULATA_NOMINALE 550
#define RPM_SIMULATA_SOGLIA_BASSA ((RPM_SIMULATA_NOMINALE * 90) / 100)
#define RPM_SIMULATA_SOGLIA_ALTA  ((RPM_SIMULATA_NOMINALE * 110) / 100)
#define RPM_SIMULATA_MIN 1
#define RPM_SIMULATA_MAX 30000
#define SOGLIA_INGRESSI_MV 5000

uint16_t nCicliSecondo;
uint16_t nCicliSecondoPrec;
uint8_t nCicloWriteCAN;

uint16_t rpmSimulati = RPM_SIMULATA_NOMINALE;

static volatile uint8_t statoIngressoC3;
static volatile uint8_t statoIngressoC4;
static uint32_t timestampPressioneC3;
static uint32_t timestampPressioneC4;
static uint8_t attesaPressioneC3;
static uint8_t attesaPressioneC4;
static uint8_t pulsanteC3Elaborato;
static uint8_t pulsanteC4Elaborato;

uint16_t wCAN[4];

static const uint16_t usciteDigitaliPin[4] = {outEV1, outEV2, outEV3, outEV4};
static const uint16_t usciteDigitaliGiriNominali[4] = {550, 1000, 36000, 800};
static volatile uint16_t usciteDigitaliGiri[4] = {550, 1000, 36000, 800};
static volatile uint8_t usciteDigitaliStato[4] = {1, 1, 1, 1};
static uint16_t usciteDigitaliFase[4];

static void aggiornaFrequenzeSimulate(void);

void cicloGenerale(void);
void cicloMacchina(void);
void letturaIngressi(void);
void comandaUscite(void);
void misuraVelocitaCoclea(void);
void gestioneAllarmi(void);
void segnalazioni(void);
void cicloComunicazioneBaseCAN();
void wrDataCANEEprom(void);
void cicloDaTastierino(void);
void cicloDaEvent(void);

uint8_t verificaRichiestaManuatenzione(uint16_t tImp, uint32_t* tTrasc);
uint8_t gestioneAllarmeEV(uint8_t ev, uint16_t mA, dataCheckEV* cEV);
uint8_t decrementaTimerAllarme(uint8_t tmr);
int16_t calcolaValoreAssoluto(int16_t rLetto, int16_t rFScala);
int16_t converti_4_20ma_ToPoint(uint16_t iw);

void byteFrontiIO(void);

static void aggiornaFrequenzeSimulate(void)
{
    uint8_t n;
    uint32_t giri;
    uint16_t frequenzaPWM;

    bios_disable_interrupts();
    for (n = 0; n < 4; n++)
    {
        giri = ((uint32_t) usciteDigitaliGiriNominali[n] * rpmSimulati + (RPM_SIMULATA_NOMINALE / 2)) / RPM_SIMULATA_NOMINALE;
        if (n == 2)
        {
            if (giri < 1200UL)
            {
                giri = 1200UL;
            }
            if (giri > 60000UL)
            {
                giri = 60000UL;
            }
        }
        else if (giri > 30000UL)
        {
            giri = 30000UL;
        }
        usciteDigitaliGiri[n] = (uint16_t) giri;
    }
    frequenzaPWM = usciteDigitaliGiri[2] / 60;
    bios_enable_interrupts();

    os_pwm_all_channels(4, 500, frequenzaPWM);
}

/// \ingroup user
/// \brief           user-defined c-code INIT
//--------------------------------------------------------------------------
/// \return          None
//--------------------------------------------------------------------------
void usercode_init(void)
{
	uint8_t n;
    // Set the SW-Version, maximal length=20
    //(void)strncpy(EEPROM_SW_Version, "V1.0", 20);

	tmr.t_Clock100ms = 100;
	tmr.t_Clock250ms = 250;
	tmr.t_Clock500ms = 2;
	tmr.t_Clock1000ms = 4;
	dAttrezzo.clockWriteEE = 60;

	os_eeprom_read_data(0, &ee.b[0], EEPROM_SIZE);
	
	if (ee.p.test != EE_CODE)
	{
		uint16_t n;
		
		for (n = 0; n < EEPROM_SIZE; n++)
		{
			ee.b[n] = 0;
		}
		
		ee.p.test = EE_CODE;
		
		ee.p.dCfg.p.ePressione1 = 0;
		ee.p.dCfg.p.ePressione2 = 0;

		ee.p.maxGiri = 200;
		ee.p.minGiri = 50;
		ee.p.taccheGiro = 1;
		
		ee.p.fsPressione1 = 250;
		ee.p.fsPressione2 = 250;
		ee.p.maxPressione1 = 200;
		ee.p.maxPressione2 = 200;

		ee.p.secLavoroTotale = 0;
		ee.p.secLavoroCliente = 0;
		
		for (n = 0; n < 4; n++)
		{
			ee.p.secLavoroParziale[n] = 0;
			ee.p.oreManutenzione[n] = 20;
		}
		
		os_eeprom_write_data(0, &ee.b[0], EEPROM_SIZE);
	}
	
    os_init_pwm();
    for (n = 0; n < 4; n++)
    {
        if (n != 2)
        {
            os_pin_write(usciteDigitaliPin[n], usciteDigitaliStato[n]);
        }
    }
    os_pwm_all_channels(4, 500, 600);

	dAttrezzo.dMaster.p.delayShutDown = 300;
	dAttrezzo.dMaster.p.versionSW = 204;
	
	dAttrezzo.dMaster.p.oRitenuta = 1;
    os_pin_write(outRitenuta, dAttrezzo.dMaster.p.oRitenuta);
	
	dAttrezzo.dGen.p.secLavoroTotalePrec = ee.p.secLavoroTotale;
	
	wrDataCANEEprom();
}


/// \ingroup user
/// \brief           user-defined c-code
//--------------------------------------------------------------------------
/// \return          None
//--------------------------------------------------------------------------
void usercode(void)
{
    uint8_t modificaVelocita;
    uint8_t aumentaVelocita;
    uint32_t nuovaVelocita;
uint32_t ret;

    ret = os_can_send_message(0x702, 0, 8, 0x66, 0x33, 0x11, 0, 0, 0, 0, 0);

   /* uint16_t tensione_ana5_mv;

    // ... (eventuale logica temporizzatori esistente) ...

    cicloGenerale();

    // 1. Lettura analogica in millivolt di ANA5 (Pin C2 / iwGiri)
    tensione_ana5_mv = os_algin_mv(iwGiri);

    // 2. Assegnazione al buffer CAN (Word 0 = Byte 0 e 1)
    wCAN[0] = tensione_ana5_mv;
    wCAN[1] = 0;
    wCAN[2] = 0;
    wCAN[3] = 0;

    // 3. Scrittura nel database CAN sul messaggio 0x701 (WR_TEST1)
    can_db_set_value(0, WR_W0_TEST1, (uint32_t) wCAN[0]);
    can_db_set_value(0, WR_W1_TEST1, (uint32_t) wCAN[1]);
    can_db_set_value(0, WR_W2_TEST1, (uint32_t) wCAN[2]);
    can_db_set_value(0, WR_W3_TEST1, (uint32_t) wCAN[3]);*/

	nCicliSecondo++;
	tmr.imp_Clock100ms = (tmr.s_Clock100ms != tmr.s_Clock100msPrec);
	tmr.s_Clock100msPrec = tmr.s_Clock100ms;

	tmr.imp_Clock250ms = (tmr.s_Clock250ms != tmr.s_Clock250msPrec);
	tmr.s_Clock250msPrec = tmr.s_Clock250ms;
	
	if (tmr.imp_Clock250ms)
	{
		if (tmr.t_Clock500ms > 0)
		{
			tmr.t_Clock500ms--;
		}
		else
		{
			tmr.s_Clock500ms = !tmr.s_Clock500ms;
			tmr.t_Clock500ms = 1;
		}
		
		if (tmr.t_Clock1000ms > 0)
		{
			tmr.t_Clock1000ms--;
		}
		else
		{
			tmr.s_Clock1000ms = !tmr.s_Clock1000ms;
			tmr.t_Clock1000ms = 3;
		}
	}

	tmr.imp_Clock500ms = (tmr.s_Clock500ms != tmr.s_Clock500msPrec);
	tmr.s_Clock500msPrec = tmr.s_Clock500ms;

	tmr.imp_Clock1000ms = (tmr.s_Clock1000ms != tmr.s_Clock1000msPrec);
	tmr.s_Clock1000msPrec = tmr.s_Clock1000ms;
	if (tmr.imp_Clock1000ms)
	{
		nCicliSecondoPrec = nCicliSecondo;
		nCicliSecondo = 0;
	}

	cicloGenerale();

    modificaVelocita = 0;
    aumentaVelocita = 0;
    if (statoIngressoC3 && statoIngressoC4)
    {
        attesaPressioneC3 = 0;
        attesaPressioneC4 = 0;
        pulsanteC3Elaborato = 1;
        pulsanteC4Elaborato = 1;
    }
    else if (!statoIngressoC3 && !statoIngressoC4)
    {
        attesaPressioneC3 = 0;
        attesaPressioneC4 = 0;
        pulsanteC3Elaborato = 0;
        pulsanteC4Elaborato = 0;
    }
    else if (statoIngressoC3)
    {
        attesaPressioneC4 = 0;
        pulsanteC4Elaborato = 0;
        if (!pulsanteC3Elaborato)
        {
            if (!attesaPressioneC3)
            {
                os_timestamp(&timestampPressioneC3, OS_1ms);
                attesaPressioneC3 = 1;
            }
            else if (os_time_past(timestampPressioneC3, 50, OS_1ms))
            {
                pulsanteC3Elaborato = 1;
                attesaPressioneC3 = 0;
                modificaVelocita = 1;
                aumentaVelocita = 1;
            }
        }
    }
    else if (statoIngressoC4)
    {
        attesaPressioneC3 = 0;
        pulsanteC3Elaborato = 0;
        if (!pulsanteC4Elaborato)
        {
            if (!attesaPressioneC4)
            {
                os_timestamp(&timestampPressioneC4, OS_1ms);
                attesaPressioneC4 = 1;
            }
            else if (os_time_past(timestampPressioneC4, 50, OS_1ms))
            {
                pulsanteC4Elaborato = 1;
                attesaPressioneC4 = 0;
                modificaVelocita = 1;
            }
        }
    }

    if (modificaVelocita)
    {
        if (aumentaVelocita)
        {
            nuovaVelocita = ((uint32_t) rpmSimulati * 110 + 50) / 100;
            if (nuovaVelocita > RPM_SIMULATA_MAX)
            {
                nuovaVelocita = RPM_SIMULATA_MAX;
            }
        }
        else
        {
            nuovaVelocita = ((uint32_t) rpmSimulati * 90 + 50) / 100;
            if (nuovaVelocita < RPM_SIMULATA_MIN)
            {
                nuovaVelocita = RPM_SIMULATA_MIN;
            }
        }
        rpmSimulati = (uint16_t) nuovaVelocita;
        aggiornaFrequenzeSimulate();
    }

    wCAN[0] = usciteDigitaliGiri[0];
    wCAN[1] = usciteDigitaliGiri[1];
    wCAN[2] = usciteDigitaliGiri[2];
    wCAN[3] = usciteDigitaliGiri[3];

    can_db_set_value(0, WR_W0_TEST1, (uint32_t) wCAN[0]);
    can_db_set_value(0, WR_W1_TEST1, (uint32_t) wCAN[1]);
    can_db_set_value(0, WR_W2_TEST1, (uint32_t) wCAN[2]);
    can_db_set_value(0, WR_W3_TEST1, (uint32_t) wCAN[3]);

    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //  To start programming in C set 2 #define in "user_code.h":
    //
    //    1. Select your hardware type (#define HW_TYPE HW_CAN_IO_V2)
    //
    //    2. Enable or disable the synchronous execution of graph code (#define GRAPH_DISABLE)
    //
    //  Furthermore you can set up CAN baudrate and CAN filter / masks
    //  in "can_db_tables.h"
    //
    //  CodeWarrior 6.3 can be found here:
    //  => http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=CW-MICROCONTROLLERS
    //
    //  Downloadlink: http://www.freescale.com/lgfiles/devsuites/HC08/CW_MCU_V6_3_SE.exe
    //-----------------------------------------------------------
    //-----------------------------------------------------------


    /*
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // INPUTS AND OUTPUTS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH
    // Procedure	  uint8_t os_digin(uint16_t i);
    // Overview	    read a digital input
    // Input		    digital input, see #os_port_pin in os_io_tables.c
    // Output	      state of the pin
    //--------------------------------------------------------------------------

    // Example: ("D_IN0" = name of digital input, see #os_port_pin in os_io_tables.c):

    variable1 = os_digin(D_IN0);


    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH DISABLED
    // Procedure	  uint8_t os_pin_read(uint16_t i);
    // Overview	    read a digital input
    // Input		    i     digital input, see #os_port_pin in os_io_tables.c
    // Output	      state of the pin
    //--------------------------------------------------------------------------

    // Example: ("D_IN0" = name of digital input, see #os_port_pin in os_io_tables.c):

    variable1 = os_pin_read(D_IN0);


    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH
    // Procedure	  void os_digout(uint16_t i, uint8_t value);
    // Overview	    set a digital output
    // Input		    i       digital output, see #os_port_pin in os_io_tables.c
    //              value   value of the output
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("OUT_HSD0" = name of digital ouput; "1" = output is set to 1):

    os_digout(OUT_HSD0, 1);


    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH DISABLED
    // Procedure	  void os_pin_write(uint16_t i, uint8_t value);
    // Overview	    set a digital output
    // Input		    i       digital output, see #os_port_pin in os_io_tables.c
    //              value   value of the output
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("OUT_HSD0" = name of digital ouput; "1" = output is set to 1):

    os_pin_write(OUT_HSD0, 1);


    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	  uint16_t os_algin_mv(uint8_t i)
    // Overview	    read an analog input
    // Input		    i     analog input, see #os_analog_pin in os_io_tables.c
    // Output	      value of the pin in mV
    //--------------------------------------------------------------------------

    // Example: ("ANA0" = name of analog input, see #os_analog_pin in os_io_tables.c)

    variable1 =  os_algin_mv(ANA0);





    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // CAN-BUS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	  uint8_t os_can_send_message(uint32_t id, uint8_t id_type,  uint8_t dlc,
    //                        uint8_t byte0 , uint8_t byte1 , uint8_t byte2 , uint8_t byte3 ,
    //                        uint8_t byte4 , uint8_t byte5 , uint8_t byte6 , uint8_t byte7);
    // Overview	    send an user-defined CAN message
    // Input		    id        CAN identifier
    //              id_type   "0" 11bit id, "1" 29bit id
    //              dlc       data length
    //              byte 0-7  data of the message
    // Output	      0=OK, 1=Error(Send buffer full or CAN not initialized or bus heavy)
    //--------------------------------------------------------------------------

    // Example:

    ret = os_can_send_message(0x720, 0, 8, 0x66, 0x33, 0x11, 0, 0, 0, 0, 0);


    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	  void user_can_message_receive(bios_can_msg_typ* msg);
    // Overview	    THE PROCEDURE IS AT THE END OF THIS FILE
    //              It's the simpliest way to evaluate a received CAN message
    //              When a message is received it is stored in a buffer. Every
    //              cycle time the buffer is read, and when something is in
    //              the procedure is called
    // Input		    -
    // Output	      -
    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	void can_db_transmit_deactivate(uint8 status);
    // Overview     Stops sending of all can Blocks.
    // Input		status uint8 value for all can dbs, 1 = stop, 0 = run.
    //
    // Output	    None
    //--------------------------------------------------------------------------

    // Example:

	  can_db_transmit_deactivate(1);




    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH
    // Procedure	  uint32_t can_db_get_value(uint8_t bus_nr, uint32_t name_signal);
    // Overview	    read a CAN message
    // Input		    bus_nr        "0"
    //              name_signal   name of datapoint, see #can1_datenpunkt_db_const
    //                            in can_db_tables.c
    // Output	      value of datapoint
    //--------------------------------------------------------------------------

    // Example: ("0" = bus-nr, there's only one bus; "POINT1" = datapoint-name):

    variable1 =  can_db_get_value(0, POINT1);



    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH
    // Procedure	  uint8_t can_db_test_dp_value(uint8_t bus_nr, uint32_t id);
    // Overview	    see whether the CAN message with that datapoint is received
    // Input		    bus_nr    "0"
    //              id        name of datapoint, see #can1_datenpunkt_db_const
    //                        in can_db_tables.c
    // Output	      "0" no message received,
    //              "1" message received, datapoint has same value,
    //              "2" message received, datapoint has another value
    //--------------------------------------------------------------------------

    // Example: ("0" = bus-nr, there's only one bus; "POINT1" = datapoint-name):

    variable1 =  can_db_test_dp_value(0, POINT1);

    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH
    // Procedure	  void can_db_set_value(uint8_t bus_nr, uint32_t name_signal, uint32_t wert_int);
    // Overview	    set the value of a datapoint (it's sended automatically)
    // Input		    bus_nr        "0"
    //              name_signal   name of datapoint, see #can1_datenpunkt_db_const
    //                            in can_db_tables.c
    //              wert_int      specify value of datapoint
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("0" = bus-nr, there's only one bus; "POINT1" = datapoint-name; "23" = datapoint is set to 23);

    can_db_set_value(0, POINT2, 23);



    //--------------------------------------------------------------------------
    // Available    ONLY GRAPH
    // Procedure	  void can_db_set_transmit_flag(uint8_t bus_nr, uint32_t id );
    // Overview	    manually send a message defined in graphical programming
    // Input		    bus_nr    "0"
    //              id        name of datapoint, see #can1_datenpunkt_db_const
    //                        in can_db_tables.c
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: ("0" = bus-nr, there's only one bus; "POINT2" = datapoint-name)

    can_db_set_transmit_flag(0, POINT2);


    //--------------------------------------------------------------------------
    // Available    ONLY C-CODE
    // Procedure    void os_can_set_baudrate (uint8_t bus_id, uint8_t can_baudrate);
    // Overview     Change baudrate during running time.
    //              Make sure that the resulting error frames are handled and the function is only called once.
    //              After restart, the baud rate is reset to the default value.
    // Input        bus_id uint8_t  CAN bus module number (CAN_BUS_0, CAN_BUS_1, CAN_BUS_2, CAN_BUS_3, CAN_BUS_4)
    //              can_baudrate uint8_t  CAN baudrate, e.g. CAN_BAUD_USER_125, CAN_BAUD_USER_250, CAN_BAUD_USER_500, see enum_USER_CAN_BAUDRADE in os_can.h
    //
    // Output	    None
    //--------------------------------------------------------------------------

    // Example: Set CAN0 to 250kBaud

  	void os_can_set_baudrate (CAN_BUS_0, CAN_BAUD_USER_250);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // TIME BEHAVIOR
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // check if a time of 500ms passed since last timestamp
    if( os_time_past(time_val, 500, OS_1ms) )
    {
        // if the time passed, set "time_val" to a new timestamp to start cycling
        os_timestamp(&time_val, OS_1ms);
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // USER VARIABLE FOR DATA EXCHANGE WITH GRAPHICAL PROGRAMMING
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // Index 0 - USER_VARIABLE_SIZE (s. user_code.h) as 32bit values, same index as in graphical programming
    variable1 = (uint16_t)user_variable[0];



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // EEPROM SAVE AND READ DATA
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // The array eeprom_userdaten[] with Index 0 - USERDATENSATZ_SIZE (s. user_code.h)
    // is read from EEPROM on startup
    // You can read and write to that array as often you want to
    variable1 = eeprom_userdaten[0];

    // If you want to save that array to the EEPROM call (maximal 100.000 times)
    os_eeprom_write_all(EEPROM_USER);



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // EEPROM DIRECT ROUTINES
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // User-EEPROM-Range (Addr) is 0x000 - 0x5FF  (=1536 bytes)
    // With the following routines you can read and write to user-EEPROM range for
    // the specified number (Size) in bytes.

    // Read any EEPROM data:

    //--------------------------------------------------------------------------
    // Procedure	  uint8_t os_eeprom_read_data(uint16_t Addr, uint8_t *Data, uint16_t Size);
    // Overview     Read EEPROM data
    // Input        Addr      Start address, range 0x000 - 0x5FF
    //              Size      Length in bytes
    // Output	      *Data     Pointer to uint8_t array
    // Return       0: ERR_OK, > 0: Error occured (see PE_Error.h for error possibilities)
    //--------------------------------------------------------------------------

    e.g.
    ret = os_eeprom_read_data(addr, &data_arr[0], 8);


    // Write any EEPROM data:

    //--------------------------------------------------------------------------
    // Procedure	  uint8_t os_eeprom_write_data(uint16_t Addr, uint8_t *Data, uint16_t Size);
    // Overview     Write EEPROM data
    // Input        Addr      Start address, range 0x000 - 0x5FF
    //              Size      Length in bytes
    // Output	      *Data     Pointer to uint8_t array
    // Return       0: ERR_OK, > 0: Error occured (see PE_Error.h for error possibilities)
    //--------------------------------------------------------------------------

    e.g.
    ret = os_eeprom_write_data(addr, &data_arr[0], 8);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SETTING A PWM IN GENERAL
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Available  BOTH
    // Procedure	void os_pwm_duty_cycle(uint8_t i, uint16_t pwm_promille, uint16_t pwm_frequency, uint16_t pwm_dither_promille, uint16_t pwm_dither_frequency) {
    // Overview	Set a pwm duty cycle and frequency (+dither if needed)
    // Input		i               channel, see os_pwm_pin in os_io_tables.c
    //            pwm_promille    duty cycle in 1/1000 (value range from 0 - 1000)
    //            pwm_frequency   20Hz - 40kHz (1digit / Hz), max frequency depending from output driver
    //            pwm_dither_promille     dither duty cycle
    //            pwm_dither_frequency    dither frequency in Hz
    // Output	    None
    //--------------------------------------------------------------------------

    // Example:
    os_pwm_duty_cycle(0, 500, 150, 0, 0);



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SETTING A PWM AT THE OUTPUTS OF THE CAN I/O
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	  void os_pwm_all_channels(uint8_t i, uint16_t duty_cycle, uint16_t pwm_frequency);
    // Overview	    Set a pwm duty cycle and frequency at outputs OUT_HSD2-7 on CAN I/O
    // Input		    i               0: OUT_HSD2, 1: OUT_HSD3, 2: OUT_HSD4, ..., 5: OUT_HSD7
    //              duty_cycle      in 1/1000 (value range from 0 - 1000)
    //              pwm_frequency   20Hz - 1kHz (1digit / Hz), max frequency depended from high side driver
    // Output	      None
    //--------------------------------------------------------------------------

    // Example: (on OUT_HSD2 a pwm with 150Hz and 50% duty cycle)

    os_pwm_all_channels(0, 500, 150);


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SETTING CURRENT CONTROL WITH PWM IN GENERAL
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Available  BOTH
    // Procedure	void os_pwm_control(uint8_t i, uint16_t w, uint16_t pwm_frequency, uint16_t pwm_dither_promille, uint16_t pwm_dither_frequency, uint16_t rampe, uint16_t kp_min, uint16_t kp_max, uint16_t time_diff, uint16_t *kp_calc, uint8_t *error);
    // Overview	Set a pwm duty cycle and frequency (+dither if needed) for current control
    // Input		i               channel, see os_pwm_pin in os_io_tables.c
    //            w               current in mA, range 0-5000
    //            pwm_frequency   20Hz - 40kHz (1digit / Hz), max frequency depending from output driver
    //            pwm_dither_promille     dither duty cycle in 1/1000
    //            pwm_dither_frequency    dither frequency in Hz
    //            rampe           ramp in ms for 100%
    //            kp_min          minimum kp*1024, e.g. 200
    //            kp_max          maximum kp*1024, e.g. 4000
    //            time_diff       graph_cycle_time, e.g. 10
    //            *kp_calc        pointer for actual kp value
    //            *error          pointer for error, 0=OK
    // Output	    None
    //--------------------------------------------------------------------------

    // Activate #define PWM_ANALOG_IO_TICK in graph_defines.h first! Otherwise current control wouldn't be possible.

    // Example: Setting PWM0 to 500mA at 1kHz PWM

    os_pwm_control(PWM0, 500, 1000, 0, 0, 0, 200, 1500, graph_cycle_time, &var_kp_calc, &var_error);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SETTING MOTOR OUTPUT OF CAN_FULLBRIDGE
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure    void os_pwm_motor_control(uint8_t i, uint16_t w, uint8_t w_dir, uint16_t pwm_frequency, uint16_t time_diff, uint8_t m2_trigger, uint8_t* error_stat);
    // Overview     Set a pwm duty cycle and frequency and direction for motor control
    // Input        i                      channel, see os_pwm_pin in os_io_tables.c
    //              w                      duty cycle in 1/1000 (range 0-1000)
    //              w_dir                  0=direction A, 1=direction B
    //              pwm_frequency          20Hz - 40kHz (1digit / Hz)
    //              time_diff              graph_cycle_time, e.g. 10
    //              m2_trigger             re-check if motor is connected
    //              *error_stat            Bit0=Motor not correctly connected, Bit1=Current too high, waiting 5s
    // Output       None
    //--------------------------------------------------------------------------

    // Example: Setting motor to turn in direction A(0) with duty cycle of 50% at 150Hz

    os_pwm_motor_control(PWM0, 500, 0, 150, graph_cycle_time, 0, &var_error);


	//--------------------------------------------------------------------------
    // SETTING MOTOR OUTPUT FOR 10A, 10A PRO
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Available    C-Code
    // Procedure    void modulhardwarecode_fullbridge_10A_block(uint16_t pwm_freq, uint16_t duty_1, uint32_t ramp_max, uint32_t ramp_up, uint32_t ramp_dwn uint8_t rotation, uint8_t motor_check, uint8_t* error, uint16_t* curr);
    // Overview     Set a pwm duty cycle, frequency and ramp for motor control
    // Input        pwm_freq [in]          pwm frequency [Hz]
    //              duty [in]              duty cycle [% * 10]
    //              ramp_max [in]          ramp max [ms]
    //              ramp_up [in]           ramp up [ms]
    //              ramp_dwn               ramp dwn [ms]
    //              dir [in]               direction
    //              motor_check [in]       motor check enb.
    //              *error [in]            status output
    //              *curr [in]             current output
    // Output       None
    //--------------------------------------------------------------------------

    // Example:     Setting motor to turn in direction B(1) with duty cycle of 50% at 150Hz and motor_check disabled.

    (void)modulhardwarecode_fullbridge_10A_block(150, 500, 1000, 0, 0, 1, 0, &error, &curr);



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ FREQUENCY
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Available  BOTH
    // Procedure	uint16_t os_frequency_calc(uint8_t channel_def)
    // Overview	Read frequency
    // Input		channel_def     D_ANA3 (name of digital input)
    // Output	    frequency value
    //--------------------------------------------------------------------------

    // Activate PORT_INTERRUPT_ACTIVE in graph_defines.h

    ret = os_frequency_calc(D_ANA3);



    //--------------------------------------------------------------------------
    // Available  C-Code
    // Procedure    void os_frequency_read(uint8_t channel_def, uint32_t* measure_period_timestamp, uint16_t* measured_frequency)
    // Overview     Read frequency
    // Input        channel_def                  name of digital input. see os_io_tables.c
    //              measure_period_timestamp     time variable
    // Output       measured_frequency           frequency at selected pin
    //--------------------------------------------------------------------------

    // Activate PORT_INTERRUPT_ACTIVE in graph_defines.h
    // to initialize call os_init_frequency_read in user_init.

    // Exmaple: Read frequency at D_ANA5

    os_frequency_read(D_ANA5, &time_val, &variable1);

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ ENCODER SIGNALS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------




    //--------------------------------------------------------------------------
    // Available  BOTH
    // Procedure	uint16_t os_incremental_encoder(uint8_t mode_type, uint8_t channel_def1, uint8_t channel_def2, uint16_t min, uint16_t max)
    // Overview	Read and calculate 2 encoder signales which have a 90� phase shift
    // Input		mode_type       PORT_INT_INCR_ENCODER
    //            channel_def1    D_ANA3
    //            channel_def2    D_ANA5
    //            min             0
    //            max             100 (if min=max=0, value won't be limited. To get signed value read with cast (int16_t).
    // Output	    actual counter value
    //--------------------------------------------------------------------------

    // Activate PORT_INTERRUPT_ACTIVE in graph_defines.h

    ret = os_incremental_encoder(PORT_INT_INCR_ENCODER, D_ANA3, D_ANA5, 0, 100);





    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ A PWM DUTY CYCLE BY TIMER REGISTER (CAN I/O)
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // See user_code.h
    // Call os_init_pwm_duty_read(); in void usercode_init(void)

    // variable1 = os_pwm_duty_read[i].duty;
    // variable2 = os_pwm_duty_read[i].frequency;

    // i = Timer channel, see PWM_DUTYCYCLE_READ_CH in user_code.h




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SEND AND RECEIVE RS485 DATA WITH SERIAL COMMUNCATION INTERFACE (SCI)
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // You need the CAN I/O with RS485 option!
    // Go to user_code.h and specify 3 #define:
    // Set SCI_USER_ACTIVE, set the USER_BUFFER_SIZE (maximum 255) and the SCI baud rate

    // TYPE 1: SEND A BLOCK: Fill out an array, specify the size and send it all away

    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	  void sci_buffer_send(void)
    // Overview	    After you filled out the array, start sending
    // Input		    None
    // Output	      None
    //--------------------------------------------------------------------------

    // Example1:
    sci_tx_buffer.data[0] = 0x22;
    sci_tx_buffer.data[1] = 0x33;
    sci_tx_buffer.data[2] = 0x44;
    sci_tx_buffer.max = 3;
    sci_buffer_send();

    // Example2:
    (void)sprintf(sci_tx_buffer.data, "This text will be sent!");
    sci_tx_buffer.max = strlen(sci_tx_buffer.data);
    sci_buffer_send();


    // TYPE 2: PUT BYTE TO THE TRANSMIT BUFFER:

    //--------------------------------------------------------------------------
    // Available    BOTH
    // Procedure	  unsigned char sci_put_queue(unsigned char data)
    // Overview	    Put one byte to the transmit buffer, when it's full you get
    //              back FALSE
    // Input		    data    One byte which should be sent
    // Output	      TRUE    byte was put to the buffer
    //              FALSE   buffer is full, byte was not put to the buffer
    //--------------------------------------------------------------------------

    // Example1:
    sci_put_queue(0x58);
    sci_put_queue(0x68);  // ....

    // Example2:
    if( sci_put_queue(0x58) == FALSE )  // Remember not successful sends
    {}




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // USE LOOKUP TABLE FUNCTIONS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    int16_t os_util_lookup1D(int16_t *table_x, int16_t *table_y, uint8_t count, int16_t val, uint8_t mode)

    * \param *table_x [in]             pointer x-array (1D)
    * \param *table_y [in]             pointer y-array (1D)
    * \param count [in]                counts of x-array elements
    * \param val [in]                  x-value
    * \param mode [in]                 LUT_MODE_EXTRAPOLATION (Standard): away from the array extrapolate
    *                                  LUT_MODE_LIMIT: away from the array not extrapolate
    *                                  LUT_MODE_KALIBRATION: y-value always >= 0, extrapolating upwards with difference (y_max - x_max)
    * \return int16_t                  y-value

    //--------------------------------------------------------------------------
    // for example

    int16_t arr_x[] = {136,200,311,444,666};
    int16_t arr_y[] = {-10,5,16,27,39};

    ret = os_util_lookup1D(arr_x, arr_y, 5, 250, LUT_MODE_EXTRAPOLATION);   // ret == 10
    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    int16_t os_util_lookup2D(int16_t *table_x, int16_t *table_y, uint8_t count_x, uint8_t count_y, int16_t *table_z, int16_t val_x, int16_t val_y)

    * \param *table_x [in]             pointer x-array (1D)
    * \param *table_y [in]             pointer y-array (1D)
    * \param count_x [in]              counts of x-array elements
    * \param count_y [in]              counts of y-array elements
    * \param *table_z [in]             pointer z-array (1D with (count_y * count_x) elements)
    * \param val_x [in]                x-value (away from the x-array => extrapolation)
    * \param val_y [in]                y-value (away from the y-array => extrapolation)
    * \return int16_t                  z-value

    //--------------------------------------------------------------------------
    // for example

    int16_t arr_x_2d[] = {10,20,30,40,50};
    int16_t arr_y_2d[] = {100,200,300};

    // y * x Matrix
    int16_t arr_z[] = {
        10,  20,  50, 90,150,
        25,  70, 150,250,400,
        50, 120, 240,400,860
    };

    ret = os_util_lookup2D(arr_x, arr_y, 5, 3, arr_z, 25, 270);   // ret == 159
    //--------------------------------------------------------------------------





    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // ACTIVATE PORT INTERRUPT
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    Goto graph_defines.h and activate #define PORT_INTERRUPT_ACTIVE (or in graphical project use MODULE_CONFIG_STRING and set it to 0x02)

    Initialization code:

    void user_int_port_init(uint8_t channel_def);

    Example:
    Initialization port interrupt D_IN0

    user_int_port_init(D_IN0);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ �C TEMPERATURE
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // This feature is only available if os_analog_pin[] in os_io_tables.c contains an
    // ADC channel for temperature with column parameter1 set to 224

    int8_t os_temperature_read(void);

    // e.g.
    ret = os_temperature_read();  // ret = 32 => 32�C



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // CHECK CAN STATE / ERRORS
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    uint8_t os_can_check_error(void);

    Return values:
    0: 0   <= transmit error counter <= 96
    1: 96  <  transmit error counter <= 127
    2: 127 <  transmit error counter <= 255
    3: transmit error counter > 255



    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // READ ADC-VALUES ON YOUR OWN
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // At first set #define ADC_AUTO_DISABLE in user_code.h

    // To start reading of e.g. ADC channel 8 type:

    bios_analog_select_channel(8);

    // When the value is analog-digital converted an interrupt will raise and call
    // the procedure "void user_int_adc(void);" in user_code.c.
    // To read the value and acknowledge the interrupt call

    variable1 = bios_analog_get_channel_single();

    // After that you can stop reading adc-values or continue with ch9 by calling:

    bios_analog_select_channel(9);


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // EXAMPLES:
    //  - Read analog pin ANA1 and send it on CAN bus on ID 0x18FA3000 byte 0+1
    //  - Switch the output OUT_HSD2 when bit8 of byte3 of 0x1FF004AB is "1"
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // GRAPH DISABLED:

    variable1 = os_algin(ANA1);

    os_can_send_message(0x18FA3000, 1, 8, (uint8_t)variable1, (uint8_t)(variable1>>8), 0, 0, 0, 0, 0, 0);

    // For switching of output OUT_HSD2 see procedure user_can_message_receive() at the end...



    // WITH GRAPH:

    variable1 = os_algin(ANA1);

    can_db_set_value(0, DATA2, variable1); // DATA must be defined in #can1_datenpunkt_db_const


    variable2 = (uint8_t)can_db_get_value(0, DATA3); // DATA3 must be defined in #can1_datenpunkt_db_const

    os_digout(OUT_HSD3, variable2);




    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // USE LIN bus functionality
    //--------------------------------------------------------------------------
    // Actual LIN stack software version string in this macro --> MRS_LIN_STACK_SW_VERSION
    // In case of LIN communication the LIN transceiver EN Pin must be enabled on init (for example os_digout(DO_LIN_EN, 1);)
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    get the datapoint value from the LIN database
    * \details  
    *           
    * \pre      
    *           
    * \param    datapoint_id    [in] uint32_t   index of the LIN datapoint
    * \return   uint32_t                        the datapoint value
    //--------------------------------------------------------------------------
    uint32_t lin_db_get_value(uint32_t datapoint_id);
    
    value = lin_db_get_value(LIN0_RX_SIGNAL_NAME_15)
    
    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    set / write a datapoint value to the LIN database
    * \details  
    *           
    * \pre      
    *           
    * \param    datapoint_id    [in] uint32_t   index of the LIN datapoint
    * \param    set_value       [in] uint32_t   value to set / write
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_db_set_value(uint32_t datapoint_id, uint32_t set_value);
    
    lin_db_set_value(LIN0_TX_SIGNAL_NAME_1, 0x0F);
    
    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Check if a frame from the selected lin_module was received,\n
    *           without knowing about request or response frame on master or slave.
    * \details  
    *           
    * \pre      
    *           
    * \param    lin_module  [in] uint8_t    LIN index number --> enum_lin_bus_id
    * \param    frame_index [in] uint8_t    frame index number or name from enum_lin_frame_id
    * \param    reset       [in] uint8_t    TRUE (1) clear the flag to detect a new receive, FALSE (0) don't clear the flag to reset it manual
    * \return   uint8_t                     TRUE (1) received, FALSE (0) nothing received
    //--------------------------------------------------------------------------
    uint8_t lin_check_for_received_frame(uint8_t lin_module, uint8_t frame_index, uint8_t reset);

    if( lin_check_for_received_frame(LIN_BUS_0, LIN0_FRM_RX_INDEX_NAME_01, TRUE) == TRUE )
    {
        // frame was received
    }

	//--------------------------------------------------------------------------
     * \ingroup  lin
     * \brief    Check if a header form the selected lin_module was received.
     *
     * \details  After received a header you must clear the flag with the command
     *           lin_clear_response_frame_flag(frame_index);
     *           If you don�t clear the frame you will receive just one frame.
     * \pre
     *
     * \param    frame_index	[in] uint8_t	frame index number or name from enum_lin_frame_id
	//--------------------------------------------------------------------------

  	if (lin_check_for_received_response_frame(uint8_t frame_index))
  	{
  	lin_clear_response_frame_flag( uint8_t frame_index );
  	}

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Clear the response frame flag to transmit the slave response data field.
    * \details  On Slave mode to transmit the slave response data field./n
    *           On Master mode to tell the stack that we have read all received data./n
    *           It's not functional necessary for Master mode.
    * \pre      
    *           
    * \param    frame_index [in] uint8_t frame index number or name from enum_lin_frame_id
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_clear_response_frame_flag(uint8_t frame_index);

    lin_clear_response_frame_flag(LIN0_FRM_RESP_INDEX_NAME_01);

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Clear the request frame flag to detect a new receive.
    * \details  Only on Slave mode to detect a master request frame correctly.
    *
    * \pre      
    *           
    * \param    frame_index [in] uint8_t frame index number or name from enum_lin_frame_id
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_clear_request_frame_flag(uint8_t frame_index);

    lin_clear_request_frame_flag(LIN0_FRM_REQ_INDEX_NAME_01);

    //--------------------------------------------------------------------------
    // LIN Master only functionality
    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Schedule table handling and bus idle check with automatic go to sleep command if bus was idle for 4s.
    * \details  --> lin_schedule_handling() must be called for each bus schedule table\n
    *           With lin_st_active parameter is it possible to hold and resume a schedule table.
    * \pre      
    *           
    * \param    lin_schedule_table_index [in] uint8_t
    * \param    lin_st_active            [in] uint8_t
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_schedule_handling(uint8_t lin_schedule_table_index, uint8_t lin_st_active);

    lin_schedule_handling(LIN0_SCHEDULE_TABLE_NR1, TRUE);

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    help function to reset a started schedule table, reset the schedule line index
    * \details  call this function only one time to reset the schedule line index,\n
    *           other you send only the first frame at the first schedule line
    * \pre      
    *           
    * \param    lin_schedule_table_index [in] uint8_t
    * \return   void      
    //--------------------------------------------------------------------------
    void lin_schedule_handling_reset(uint8_t lin_schedule_table_index);

    lin_schedule_handling_reset(LIN0_SCHEDULE_TABLE_NR1);

    //--------------------------------------------------------------------------
    * \ingroup  lin
    * \brief    Check if response timeout was received, only on Master mode.
    * \details  
    *           
    * \pre      
    *           
    * \param    frame_index [in] uint8_t    frame index number or name from enum_lin_frame_id
    * \return   uint8_t                     TRUE (1) = Slave not responding (timeout), FALSE (0) = not timeout reached
    //--------------------------------------------------------------------------
    uint8_t lin_ma_check_frame_resp_timeout(uint8_t frame_index);

    if( lin_ma_check_frame_resp_timeout(LIN0_FRM_INDEX_NAME_01) == TRUE )
    {
        // timeout reached - Slave not responding
    }


    */
}

void trasmetti_frequenze_al_display(void)
{
    bios_can_msg_typ tx_msg;

    tx_msg.id     = 0x702;
    tx_msg.id_ext = 0;        /* Standard frame 11 bit */
    tx_msg.len    = 8;

    /* Little Endian: LSB prima, MSB dopo (compatibile con Opus) */
    tx_msg.data[0] = (uint8_t)(freq1 & 0xFF);         /* freq1 LSB */
    tx_msg.data[1] = (uint8_t)((freq1 >> 8) & 0xFF);  /* freq1 MSB */
    tx_msg.data[2] = (uint8_t)(freq2 & 0xFF);         /* freq2 LSB */
    tx_msg.data[3] = (uint8_t)((freq2 >> 8) & 0xFF);  /* freq2 MSB */
    tx_msg.data[4] = (uint8_t)(freq3 & 0xFF);         /* freq3 LSB */
    tx_msg.data[5] = (uint8_t)((freq3 >> 8) & 0xFF);  /* freq3 MSB */
    tx_msg.data[6] = (uint8_t)(freq4 & 0xFF);         /* freq4 LSB */
    tx_msg.data[7] = (uint8_t)((freq4 >> 8) & 0xFF);  /* freq4 MSB */

    bios_can_send(CAN_BUS_1, &tx_msg);
}

void cicloGenerale(void)
{
	dAttrezzo.dMaster.p.vivo = tmr.s_Clock250ms;
	
	if (!dAttrezzo.dMaster.p.startUP)
	{
		if (tmr.imp_Clock100ms)
		{
			dAttrezzo.delayStart++;
		}
		dAttrezzo.dMaster.p.startUP = (dAttrezzo.delayStart >= 20);
	}

	letturaIngressi();
	
	byteFrontiIO();
	
	dAttrezzo.dMaster.p.okSistema = dAttrezzo.dMaster.p.startUP;
	
	misuraVelocitaCoclea();
	cicloMacchina();
	segnalazioni();
	comandaUscite();
	gestioneAllarmi();
	
	cicloComunicazioneBaseCAN();
	
	cicloDaTastierino();
	cicloDaEvent();
	
	if (tmr.imp_Clock250ms)
	{
		wrDataCANEEprom();
	}

	if (dAttrezzo.dMaster.p.resetContaOre1Opus | dAttrezzo.dMaster.p.resetContaOre1Event)
	{
		ee.p.secLavoroParziale[0] = 0;
	}
	
	if (dAttrezzo.dMaster.p.resetContaOre2Opus | dAttrezzo.dMaster.p.resetContaOre2Event)
	{
		ee.p.secLavoroParziale[1] = 0;
	}

	if (dAttrezzo.dMaster.p.resetContaOre3Opus | dAttrezzo.dMaster.p.resetContaOre3Event)
	{
		ee.p.secLavoroParziale[2] = 0;
	}

	if (dAttrezzo.dMaster.p.resetContaOre4Opus | dAttrezzo.dMaster.p.resetContaOre4Event)
	{
		ee.p.secLavoroParziale[3] = 0;
	}
	
	if (dAttrezzo.dMaster.p.resetContaOreCliente)
	{
		ee.p.secLavoroCliente = 0;
	}
		
	dAttrezzo.dMaster.p.inLavoro = (dAttrezzo.dMaster.p.rpmGiri >= ee.p.minGiri);
	dAttrezzo.dMaster.p.inLavoro &= dAttrezzo.dMaster.p.reqEV1;
	dAttrezzo.dMaster.p.inLavoro &= dAttrezzo.dMaster.p.reqEV2;
		
	if (tmr.imp_Clock1000ms)
	{
		if (dAttrezzo.dMaster.p.inLavoro)
		{
			ee.p.secLavoroTotale += 1;
			ee.p.secLavoroCliente += 1;
			ee.p.secLavoroParziale[0] += 1;
			ee.p.secLavoroParziale[1] += 1;
			ee.p.secLavoroParziale[2] += 1;
			ee.p.secLavoroParziale[3] += 1;
		}

		if (dAttrezzo.clockWriteEE > 0)
		{
			dAttrezzo.clockWriteEE--;
		}
		else
		{
			dAttrezzo.clockWriteEE = 60;
			if (dAttrezzo.dGen.p.secLavoroTotalePrec != ee.p.secLavoroTotale)
			{
				dAttrezzo.dGen.p.secLavoroTotalePrec = ee.p.secLavoroTotale;
				os_eeprom_write_data(0, &ee.b[0], EEPROM_SIZE);
			}
		}
	}
}

void cicloMacchina(void)
{
	dAttrezzo.dMaster.p.oEV1 = 0;
	dAttrezzo.dMaster.p.oEV2 = 0;
	dAttrezzo.dMaster.p.oEV3 = 0;
	dAttrezzo.dMaster.p.oEV4 = 0;

	if (dAttrezzo.dMaster.p.okSistema)
	{
		dAttrezzo.dGen.p.microBracciON = dAttrezzo.dMaster.p.iBraccioDX & dAttrezzo.dMaster.p.iBraccioSX;
		dAttrezzo.dGen.p.microAspoON = dAttrezzo.dMaster.p.iAspoAvantiDX & dAttrezzo.dMaster.p.iAspoAvantiSX & dAttrezzo.dMaster.p.iAspoBasso;
		
		if (tmr.imp_Clock100ms)
		{
			if (dAttrezzo.dGen.p.delayBracciON > 0)
			{
				dAttrezzo.dGen.p.delayBracciON--;
			}
			if (dAttrezzo.dGen.p.delayAspoOn > 0)
			{
				dAttrezzo.dGen.p.delayAspoOn--;
			}
		}
			
		if (dAttrezzo.dGen.p.microBracciON == 0)
		{
			dAttrezzo.dGen.p.delayBracciON = 10;
		}
		if (dAttrezzo.dGen.p.microAspoON == 0)
		{
			dAttrezzo.dGen.p.delayAspoOn = 10;
		}
		
		dAttrezzo.dMaster.p.microBracciON = (dAttrezzo.dGen.p.delayBracciON == 0);
		dAttrezzo.dMaster.p.microBracciOFF = (dAttrezzo.dMaster.p.iBraccioDX == 0) & (dAttrezzo.dMaster.p.iBraccioSX == 0);
		dAttrezzo.dMaster.p.microAspoON = (dAttrezzo.dGen.p.delayAspoOn == 0);
		dAttrezzo.dMaster.p.microAspoOFF = (dAttrezzo.dMaster.p.iAspoAvantiDX == 0) & (dAttrezzo.dMaster.p.iAspoAvantiSX == 0) & (dAttrezzo.dMaster.p.iAspoBasso == 0);
		
		dAttrezzo.dGen.p.eEV4 = dAttrezzo.dMaster.p.microAspoON;
		dAttrezzo.dGen.p.eEV4 |= (dAttrezzo.dMaster.p.microBracciON == 0);
		
		if (dAttrezzo.dMaster.p.disabilitaEV1EV2)
		{
			dAttrezzo.dMaster.p.oEV1 = 0;
			dAttrezzo.dMaster.p.oEV2 = 0;
		}
		else
		{
			dAttrezzo.dMaster.p.oEV1 = dAttrezzo.dMaster.p.reqEV1;
			dAttrezzo.dMaster.p.oEV2 = dAttrezzo.dMaster.p.reqEV2;
		}
		dAttrezzo.dMaster.p.oEV3 = dAttrezzo.dMaster.p.reqEV3;
		dAttrezzo.dMaster.p.oEV4 = dAttrezzo.dMaster.p.reqEV4 & dAttrezzo.dGen.p.eEV4;
		if (dAttrezzo.dMaster.p.reqEV4)
		{
			if (dAttrezzo.dGen.p.eEV4 == 0)
			{
				dAttrezzo.dMaster.p.oEV1 = 0;
			}
		}
	}
	else
	{
		dAttrezzo.dMaster.p.microBracciON = 0;
		dAttrezzo.dMaster.p.microBracciOFF = 0;
		dAttrezzo.dMaster.p.microAspoON = 0;
		dAttrezzo.dMaster.p.microAspoOFF = 0;
		dAttrezzo.dGen.p.delayBracciON = 10;
		dAttrezzo.dGen.p.delayAspoOn = 10;
	}
}

void cicloComunicazioneBaseCAN(void)
{
	if (nCicloWriteCAN == 0)
	{
		can_db_set_value(0, WR_W0_0x221, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x221_W[0]);
		can_db_set_value(0, WR_W1_0x221, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x221_W[1]);
		can_db_set_value(0, WR_W2_0x221, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x221_W[2]);
		can_db_set_value(0, WR_W3_0x221, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x221_W[3]);

		nCicloWriteCAN = 1;
	}
	else if (nCicloWriteCAN == 1)
	{
		can_db_set_value(0, WR_W0_0x222, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x222_W[0]);
		can_db_set_value(0, WR_W1_0x222, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x222_W[1]);
		can_db_set_value(0, WR_W2_0x222, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x222_W[2]);
		can_db_set_value(0, WR_W3_0x222, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x222_W[3]);

		nCicloWriteCAN = 2;
	}
	else if (nCicloWriteCAN == 2)
	{
		can_db_set_value(0, WR_W0_0x223, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x223_W[0]);
		can_db_set_value(0, WR_W1_0x223, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x223_W[1]);
		can_db_set_value(0, WR_W2_0x223, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x223_W[2]);
		can_db_set_value(0, WR_W3_0x223, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x223_W[3]);

		nCicloWriteCAN = 3;
	}
	else if (nCicloWriteCAN == 3)
	{
		can_db_set_value(0, WR_W0_0x224, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x224_W[0]);
		can_db_set_value(0, WR_W1_0x224, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x224_W[1]);
		can_db_set_value(0, WR_W2_0x224, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x224_W[2]);
		can_db_set_value(0, WR_W3_0x224, (uint32_t) dAttrezzo.dMaster.wCAN.wr_0x224_W[3]);

		nCicloWriteCAN = 4;
	}
	else if (nCicloWriteCAN == 4)
	{

		nCicloWriteCAN = 5;
	}
	else if (nCicloWriteCAN == 5)
	{

		nCicloWriteCAN = 6;
	}
	else if (nCicloWriteCAN == 6)
	{

		nCicloWriteCAN = 7;
	}
	else if (nCicloWriteCAN == 7)
	{

		nCicloWriteCAN = 8;
	}
	else if (nCicloWriteCAN == 8)
	{
		wCAN[0] = usciteDigitaliGiri[0];
		wCAN[1] = usciteDigitaliGiri[1];
		wCAN[2] = usciteDigitaliGiri[2];
		wCAN[3] = usciteDigitaliGiri[3];
		
		can_db_set_value(0, WR_W0_TEST1, (uint32_t) wCAN[0]);
        can_db_set_value(0, WR_W1_TEST1, (uint32_t) wCAN[1]);
        can_db_set_value(0, WR_W2_TEST1, (uint32_t) wCAN[2]);
		can_db_set_value(0, WR_W3_TEST1, (uint32_t) wCAN[3]);

		nCicloWriteCAN = 9;
	}
	else if (nCicloWriteCAN == 9)
	{

		nCicloWriteCAN = 0;
	}
}

void wrDataCANEEprom(void)
{
	can_db_set_value(0, WR_W0_0x261, (uint32_t) ee.p.dCfg.w);
	can_db_set_value(0, WR_W1_0x261, (uint32_t) ee.p.maxGiri);
	can_db_set_value(0, WR_W2_0x261, (uint32_t) ee.p.taccheGiro);
	can_db_set_value(0, WR_W3_0x261, (uint32_t) ee.p.minGiri);
	
	can_db_set_value(0, WR_W0_0x262, (uint32_t) ee.p.fsPressione1);
	can_db_set_value(0, WR_W1_0x262, (uint32_t) ee.p.fsPressione2);
	can_db_set_value(0, WR_W2_0x262, (uint32_t) ee.p.maxPressione1);
	can_db_set_value(0, WR_W3_0x262, (uint32_t) ee.p.maxPressione2);
	
	wCAN[0] = (uint16_t) (ee.p.secLavoroTotale & 0xFFFF);
	wCAN[1] = (uint16_t) (ee.p.secLavoroTotale >> 16);
	wCAN[2] = (uint16_t) (ee.p.secLavoroCliente / 360);
	wCAN[3] = (uint16_t) 0;
		
	can_db_set_value(0, WR_W0_0x263, (uint32_t) wCAN[0]);
	can_db_set_value(0, WR_W1_0x263, (uint32_t) wCAN[1]);
	can_db_set_value(0, WR_W2_0x263, (uint32_t) wCAN[2]);
	can_db_set_value(0, WR_W3_0x263, (uint32_t) wCAN[3]);
	
	can_db_set_value(0, WR_W0_0x264, (ee.p.secLavoroParziale[0] / 360));
	can_db_set_value(0, WR_W1_0x264, (ee.p.secLavoroParziale[1] / 360));
	can_db_set_value(0, WR_W2_0x264, (ee.p.secLavoroParziale[2] / 360));
	can_db_set_value(0, WR_W3_0x264, (ee.p.secLavoroParziale[3] / 360));
	
	can_db_set_value(0, WR_W0_0x265, (uint32_t) ee.p.oreManutenzione[0]);
	can_db_set_value(0, WR_W1_0x265, (uint32_t) ee.p.oreManutenzione[1]);
	can_db_set_value(0, WR_W2_0x265, (uint32_t) ee.p.oreManutenzione[2]);
	can_db_set_value(0, WR_W3_0x265, (uint32_t) ee.p.oreManutenzione[3]);
}

void letturaIngressi(void)
{
	uint16_t uiW = 0;
	int16_t iW = 0;
	uint8_t n;

	dAttrezzo.dMaster.p.iQuadroOn = os_pin_read(inQuadroOn);
	
    statoIngressoC3 = (os_algin_mv(iwPressione1) >= SOGLIA_INGRESSI_MV);
    statoIngressoC4 = (os_algin_mv(iwPressione2) >= SOGLIA_INGRESSI_MV);
    dAttrezzo.dMaster.p.iBraccioDX = statoIngressoC3;
    dAttrezzo.dMaster.p.iBraccioSX = statoIngressoC4;
    dAttrezzo.dMaster.p.iAspoAvantiSX = os_pin_read(inAspoAvantiSX);
    dAttrezzo.dMaster.p.iAspoAvantiDX = os_pin_read(inAspoAvantiDX);

	dAttrezzo.dMaster.p.iAspoBasso = (os_algin_mv(iwAspoBasso) > 7000);
	dAttrezzo.dMaster.p.iAspoAvantiDX = (os_algin_mv(iwAspoAvantiDX) > 7000);
	dAttrezzo.dMaster.p.iAspoAvantiSX = (os_algin_mv(iwAspoAvantiSX) > 7000);
	
	dAttrezzo.dMaster.p.mAEV1 = os_algin_mv(STAT_ANA_HSD4);
	dAttrezzo.dMaster.p.mAEV2 = os_algin_mv(STAT_ANA_HSD5);
	dAttrezzo.dMaster.p.mAEV3 = os_algin_mv(STAT_ANA_HSD6);
	dAttrezzo.dMaster.p.mAEV4 = os_algin_mv(STAT_ANA_HSD7);
	
	if (tmr.imp_Clock100ms)
	{
		if (dAttrezzo.dMaster.p.delayShutDown > 0)
		{
			dAttrezzo.dMaster.p.delayShutDown--;
		}
	}
	if (dAttrezzo.dMaster.p.iQuadroOn)
	{
		dAttrezzo.dMaster.p.delayShutDown = 300;
	}
	
	if (tmr.imp_Clock1000ms)
	{
		uiW = dAttrezzo.dGen.p.iwRPMCoclea;
			
		if ((ee.p.taccheGiro > 1) && (ee.p.taccheGiro < 100))
		{
			uiW /= ee.p.taccheGiro;
		}
		
		dAttrezzo.dMaster.p.rpmGiri = uiW;
		
		dAttrezzo.dGen.p.aMaxGiri = (dAttrezzo.dMaster.p.rpmGiri > ee.p.maxGiri);
		dAttrezzo.dGen.p.aMaxGiri &= dAttrezzo.dMaster.p.reqEV1;
		dAttrezzo.dGen.p.aMaxGiri &= dAttrezzo.dMaster.p.reqEV2;
	}

	if (ee.p.dCfg.p.ePressione1)
	{
		uiW = (uint16_t) os_algin_mv(iwPressione1);
		iW = converti_4_20ma_ToPoint(uiW);
		dAttrezzo.dMaster.p.aPressione1 = (iW < 0);
		if (dAttrezzo.dMaster.p.aPressione1 == 0)
		{
			dAttrezzo.dMaster.p.iwPressione1 = calcolaValoreAssoluto(iW, ee.p.fsPressione1);
		}
		else
		{
			dAttrezzo.dMaster.p.iwPressione1 = 0;
		}
		dAttrezzo.dGen.p.aMaxPressione1 = (dAttrezzo.dMaster.p.iwPressione1 > ee.p.maxPressione1);
	}
	else
	{
		dAttrezzo.dMaster.p.iwPressione1 = 0;
		dAttrezzo.dMaster.p.aPressione1 = 0;
		dAttrezzo.dGen.p.aMaxPressione1 = 0;
	}
	
	if (ee.p.dCfg.p.ePressione2)
	{
		uiW = (uint16_t) os_algin_mv(iwPressione2);
		iW = converti_4_20ma_ToPoint(uiW);
		dAttrezzo.dMaster.p.aPressione2 = (iW < 0);
		if (dAttrezzo.dMaster.p.aPressione2 == 0)
		{
			dAttrezzo.dMaster.p.iwPressione2 = calcolaValoreAssoluto(iW, ee.p.fsPressione2);
		}
		else
		{
			dAttrezzo.dMaster.p.iwPressione2 = 0;
		}
		dAttrezzo.dGen.p.aMaxPressione2 = (dAttrezzo.dMaster.p.iwPressione2 > ee.p.maxPressione2);
	}
	else
	{
		dAttrezzo.dMaster.p.iwPressione2 = 0;
		dAttrezzo.dMaster.p.aPressione2 = 0;
		dAttrezzo.dGen.p.aMaxPressione2 = 0;
	}
}

void comandaUscite(void)
{
	if (can_db_test_dp_value(0, RD_W0_0x22F) != 0)
	{
		t_TimeForzaOut = 500;

		dAttrezzo.dMaster.wCAN.rd_0x22F_W[0] = (uint16_t) can_db_get_value(0, RD_W0_0x22F);
		dAttrezzo.dMaster.wCAN.rd_0x22F_W[1] = (uint16_t) can_db_get_value(0, RD_W1_0x22F);
		dAttrezzo.dMaster.wCAN.rd_0x22F_W[2] = (uint16_t) can_db_get_value(0, RD_W2_0x22F);
		dAttrezzo.dMaster.wCAN.rd_0x22F_W[3] = (uint16_t) can_db_get_value(0, RD_W3_0x22F);
	}
	
	if (t_TimeForzaOut == 0)
	{
		dAttrezzo.dMaster.p.forzaUscite = 0;
	}

	dAttrezzo.dMaster.p.oRitenuta = (dAttrezzo.dMaster.p.delayShutDown > 0);
	
	if (dAttrezzo.dMaster.p.forzaUscite != 0)
	{
		dAttrezzo.dMaster.pB.rdByteOut1 |= (dAttrezzo.dMaster.pB.wrByteOut1 & 0x03);
		dAttrezzo.dMaster.pB.wrByteOut1 = dAttrezzo.dMaster.pB.rdByteOut1;
		dAttrezzo.dMaster.pB.wrByteOut2 = (dAttrezzo.dMaster.pB.rdByteOut2 & 0x7F);
		dAttrezzo.dMaster.p.oRitenuta = 1;
	}
	
	os_pin_write(outRitenuta, dAttrezzo.dMaster.p.oRitenuta);
}

void misuraVelocitaCoclea(void)
{
	if (tmr.imp_Clock1000ms)
	{
		dAttrezzo.dGen.p.clockRPMCoclea++;
		if (dAttrezzo.dGen.p.clockRPMCoclea >= 10)
		{
			dAttrezzo.dGen.p.iwRPMCoclea = ((uint16_t) dAttrezzo.dGen.p.nImpCoclea) * 6;

			dAttrezzo.dGen.p.clockRPMCoclea = 0;
			dAttrezzo.dGen.p.nImpCoclea = 0;
		}
	}
}

void gestioneAllarmi(void)
{
	dAttrezzo.dMaster.p.aManutenzione1 = verificaRichiestaManuatenzione(ee.p.oreManutenzione[0], &ee.p.secLavoroParziale[0]);
	dAttrezzo.dMaster.p.aManutenzione2 = verificaRichiestaManuatenzione(ee.p.oreManutenzione[1], &ee.p.secLavoroParziale[1]);
	dAttrezzo.dMaster.p.aManutenzione3 = verificaRichiestaManuatenzione(ee.p.oreManutenzione[2], &ee.p.secLavoroParziale[2]);
	dAttrezzo.dMaster.p.aManutenzione4 = verificaRichiestaManuatenzione(ee.p.oreManutenzione[3], &ee.p.secLavoroParziale[3]);
	
	if (dAttrezzo.dGen.p.aMaxGiri)
	{
		dAttrezzo.dGen.p.delayMaxGiriOFF = DEALYALLARMI * 2;
	}
	else
	{
		dAttrezzo.dGen.p.delayMaxGiriON = DEALYALLARMI * 2;
	}
	
	if (dAttrezzo.dGen.p.aMaxPressione1)
	{
		dAttrezzo.dGen.p.delayMaxPressione1OFF = DEALYALLARMI;
	}
	else
	{
		dAttrezzo.dGen.p.delayMaxPressione1ON = DEALYALLARMI;
	}

	if (dAttrezzo.dGen.p.aMaxPressione2)
	{
		dAttrezzo.dGen.p.delayMaxPressione2OFF = DEALYALLARMI;
	}
	else
	{
		dAttrezzo.dGen.p.delayMaxPressione2ON = DEALYALLARMI;
	}
	
	if (tmr.imp_Clock100ms)
	{
		dAttrezzo.dGen.p.delayMaxGiriON = decrementaTimerAllarme(dAttrezzo.dGen.p.delayMaxGiriON);
		dAttrezzo.dGen.p.delayMaxGiriOFF = decrementaTimerAllarme(dAttrezzo.dGen.p.delayMaxGiriOFF);
		
		dAttrezzo.dGen.p.delayMaxPressione1ON = decrementaTimerAllarme(dAttrezzo.dGen.p.delayMaxPressione1ON);
		dAttrezzo.dGen.p.delayMaxPressione1OFF = decrementaTimerAllarme(dAttrezzo.dGen.p.delayMaxPressione1OFF);

		dAttrezzo.dGen.p.delayMaxPressione2ON = decrementaTimerAllarme(dAttrezzo.dGen.p.delayMaxPressione2ON);
		dAttrezzo.dGen.p.delayMaxPressione2OFF = decrementaTimerAllarme(dAttrezzo.dGen.p.delayMaxPressione2OFF);
	}

	if (dAttrezzo.dGen.p.delayMaxGiriON == 0)
	{
		dAttrezzo.dMaster.p.aMaxGiri = 1;
	}
	else if (dAttrezzo.dGen.p.delayMaxGiriOFF == 0)
	{
		dAttrezzo.dMaster.p.aMaxGiri = 0;
	}

	if (dAttrezzo.dGen.p.delayMaxPressione1ON == 0)
	{
		dAttrezzo.dMaster.p.aMaxPressione1 = 1;
	}
	else if (dAttrezzo.dGen.p.delayMaxPressione1OFF == 0)
	{
		dAttrezzo.dMaster.p.aMaxPressione1 = 0;
	}

	if (dAttrezzo.dGen.p.delayMaxPressione2ON == 0)
	{
		dAttrezzo.dMaster.p.aMaxPressione2 = 1;
	}
	else if (dAttrezzo.dGen.p.delayMaxPressione2OFF == 0)
	{
		dAttrezzo.dMaster.p.aMaxPressione2 = 0;
	}

	dAttrezzo.dMaster.p.aEV1 = gestioneAllarmeEV(dAttrezzo.dMaster.p.oEV1, dAttrezzo.dMaster.p.mAEV1, &dAttrezzo.dGen.p.chkEV1);
	dAttrezzo.dMaster.p.aEV2 = gestioneAllarmeEV(dAttrezzo.dMaster.p.oEV2, dAttrezzo.dMaster.p.mAEV2, &dAttrezzo.dGen.p.chkEV2);
	dAttrezzo.dMaster.p.aEV3 = gestioneAllarmeEV(dAttrezzo.dMaster.p.oEV3, dAttrezzo.dMaster.p.mAEV3, &dAttrezzo.dGen.p.chkEV3);
	dAttrezzo.dMaster.p.aEV4 = gestioneAllarmeEV(dAttrezzo.dMaster.p.oEV4, dAttrezzo.dMaster.p.mAEV4, &dAttrezzo.dGen.p.chkEV4);
}

uint8_t verificaRichiestaManuatenzione(uint16_t tImp, uint32_t* tTrasc)
{
	uint32_t secMan = (uint32_t) tImp;
	
	if (secMan > 0)
	{
		secMan *= 3600;
		return (*tTrasc > secMan);
	}
	else
	{
		*tTrasc = 0;
	}
	return 0;
}

uint8_t gestioneAllarmeEV(uint8_t ev, uint16_t mA, dataCheckEV* cEV)
{
	cEV->p.delayAlarm = decrementaTimerAllarme(cEV->p.delayAlarm);

	cEV->p.aEV = (mA < 1000);
	cEV->p.aEV |= (mA > 3000);
	cEV->p.aEV &= ev;
	
	if (cEV->p.aEV == 0)
	{
		cEV->p.delayAlarm = 50;
	}
	
	return (cEV->p.delayAlarm == 0);
}

uint8_t decrementaTimerAllarme(uint8_t tmr)
{
	if (tmr > 0)
	{
		tmr--;
	}
	
	return tmr;
}

void segnalazioni(void)
{
}

void cicloDaTastierino(void)
{
	uint8_t datoRicevutoEEprom = 0;
	uint8_t n;
	uint16_t w0 = 0;
	uint16_t w1 = 0;
	uint16_t w2 = 0;
	uint16_t w3 = 0;
	uint32_t secTotaleForza = 0;

	if (can_db_test_dp_value(0, RD_W0_0x22A) != 0)
	{
		t_TimeOPUS = 500;
		
		dAttrezzo.dMaster.wCAN.rd_0x22A_W[0] = (uint16_t) can_db_get_value(0, RD_W0_0x22A);
		dAttrezzo.dMaster.wCAN.rd_0x22A_W[1] = (uint16_t) can_db_get_value(0, RD_W1_0x22A);
		dAttrezzo.dMaster.wCAN.rd_0x22A_W[2] = (uint16_t) can_db_get_value(0, RD_W2_0x22A);
		dAttrezzo.dMaster.wCAN.rd_0x22A_W[3] = (uint16_t) can_db_get_value(0, RD_W3_0x22A);
	}
	
	dAttrezzo.dGen.p.timeOutOPUS = (t_TimeOPUS == 0);
	
	if (dAttrezzo.dGen.p.timeOutOPUS)
	{
		for (n = 0; n < 4; n++)
		{
			dAttrezzo.dMaster.wCAN.rd_0x22A_W[n] = 0;
		}
	}

	if (can_db_test_dp_value(0, RD_W0_0x271) != 0)
	{
		datoRicevutoEEprom = 1;

		ee.p.dCfg.w = (uint16_t) can_db_get_value(0, RD_W0_0x271);
		ee.p.maxGiri = (uint16_t) can_db_get_value(0, RD_W1_0x271);
		ee.p.taccheGiro = (uint16_t) can_db_get_value(0, RD_W2_0x271);
		ee.p.minGiri = (uint16_t) can_db_get_value(0, RD_W3_0x271);
	}

	if (can_db_test_dp_value(0, RD_W0_0x272) != 0)
	{
		datoRicevutoEEprom = 1;

		ee.p.fsPressione1 = (uint16_t) can_db_get_value(0, RD_W0_0x272);
		ee.p.fsPressione2 = (uint16_t) can_db_get_value(0, RD_W1_0x272);
		ee.p.maxPressione1 = (uint16_t) can_db_get_value(0, RD_W2_0x272);
		ee.p.maxPressione2 = (uint16_t) can_db_get_value(0, RD_W3_0x272);
	}

	if (can_db_test_dp_value(0, RD_W0_0x274) != 0)
	{
		datoRicevutoEEprom = 1;

		w0 = (uint16_t) can_db_get_value(0, RD_W0_0x274);
		w1 = (uint16_t) can_db_get_value(0, RD_W1_0x274);
		w2 = (uint16_t) can_db_get_value(0, RD_W2_0x274);
		w3 = (uint16_t) can_db_get_value(0, RD_W3_0x274);

		secTotaleForza = ((uint32_t) w1) << 16;
		secTotaleForza |= ((uint32_t) w0);
		
		ee.p.secLavoroTotale = secTotaleForza;
	}
	
	if (can_db_test_dp_value(0, RD_W0_0x275) != 0)
	{
		datoRicevutoEEprom = 1;

		ee.p.oreManutenzione[0] = (uint16_t) can_db_get_value(0, RD_W0_0x275);
		ee.p.oreManutenzione[1] = (uint16_t) can_db_get_value(0, RD_W1_0x275);
		ee.p.oreManutenzione[2] = (uint16_t) can_db_get_value(0, RD_W2_0x275);
		ee.p.oreManutenzione[3] = (uint16_t) can_db_get_value(0, RD_W3_0x275);
	}
	
	if (datoRicevutoEEprom)
	{
		os_eeprom_write_data(0, &ee.b[0], EEPROM_SIZE);
		wrDataCANEEprom();
	}
}

void cicloDaEvent(void)
{
	uint8_t n;

	if (can_db_test_dp_value(0, RD_W0_0x22B) != 0)
	{
		t_TimeEvent = 500;
		
		dAttrezzo.dMaster.wCAN.rd_0x22B_W[0] = (uint16_t) can_db_get_value(0, RD_W0_0x22B);
		dAttrezzo.dMaster.wCAN.rd_0x22B_W[1] = (uint16_t) can_db_get_value(0, RD_W1_0x22B);
		dAttrezzo.dMaster.wCAN.rd_0x22B_W[2] = (uint16_t) can_db_get_value(0, RD_W2_0x22B);
        dAttrezzo.dMaster.wCAN.rd_0x22B_W[3] = (uint16_t) can_db_get_value(0, RD_W3_0x22B);
	}
	
	dAttrezzo.dGen.p.timeOutEvent = (t_TimeEvent == 0);
	
	if (dAttrezzo.dGen.p.timeOutEvent)
	{
		for (n = 0; n < 4; n++)
		{
			dAttrezzo.dMaster.wCAN.rd_0x22B_W[n] = 0;
		}
	}
}

int16_t calcolaValoreAssoluto(int16_t rLetto, int16_t rFScala)
{
	uint32_t vLetto = (uint32_t) rLetto;
	uint32_t vFScala = (uint32_t) rFScala;
	
	vLetto *= vFScala;
	vLetto /= 1000;
	
	return (int16_t) vLetto;
}

int16_t converti_4_20ma_ToPoint(uint16_t iw)
{
	int32_t w = ((int32_t) iw);
	
	if (iw < 300)
	{
		return -1;
	}
	else if (iw > 2100)
	{
		return -2;
	}

	if (w > 400)
	{
		w -= 400;
		w *= 1000;
		w /= 1600;
	}
	else
	{
		w = 0;
	}
	
	return (int16_t) w;
}

void byteFrontiIO(void)
{
uint8_t i = 0;

	i = dAttrezzo.dMaster.pB.wrByteIn1 ^ dAttrezzo.dGen.pB.sPrecIn1;
	dAttrezzo.dGen.pB.upIn1 = i & dAttrezzo.dMaster.pB.wrByteIn1;
	dAttrezzo.dGen.pB.dnIn1 = i & dAttrezzo.dGen.pB.sPrecIn1;
	dAttrezzo.dGen.pB.sPrecIn1 = dAttrezzo.dMaster.pB.wrByteIn1;

	i = dAttrezzo.dMaster.pB.wrByteIn2 ^ dAttrezzo.dGen.pB.sPrecIn2;
	dAttrezzo.dGen.pB.upIn2 = i & dAttrezzo.dMaster.pB.wrByteIn2;
	dAttrezzo.dGen.pB.dnIn2 = i & dAttrezzo.dGen.pB.sPrecIn2;
	dAttrezzo.dGen.pB.sPrecIn2 = dAttrezzo.dMaster.pB.wrByteIn2;
}



/// \ingroup user
/// \brief           Manually interpret the CAN messages
//--------------------------------------------------------------------------
///                  When a CAN message is received this function will be
///                  automatically called in main-loop, while reading from
///                  the buffered messages. This routine is not called in
///                  CAN-interrupt.
///
/// \param *msg      Pointer to the message with its struct bios_can_msg_typ
/// \return          None
//--------------------------------------------------------------------------

void user_can_message_receive(bios_can_msg_typ* msg)
{
    if (msg == 0)
        return;

    if ((msg->id == 0x401) && (msg->id_ext == 0))
    {
        /* Little Endian: LSB prima, MSB dopo */
        freq1 = ((uint16_t)msg->data[1] << 8) | msg->data[0];
        freq2 = ((uint16_t)msg->data[3] << 8) | msg->data[2];
        freq3 = ((uint16_t)msg->data[5] << 8) | msg->data[4];
        freq4 = ((uint16_t)msg->data[7] << 8) | msg->data[6];
    }
}

    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    //(void)msg;

    /*
    // When 0x400 is received, send an acknowledge
    if( (msg->id == 0x400) && (msg->id_ext == 0) )
    {
        if(msg->data[0] == 0x33)
        os_can_send_message(0x720, 0, 8, 0x66, 0x33, 0x11, 0, 0, 0, 0, msg->data[7]);
    }
    else
    {
        // do nothing
    }

    // Switch the output OUT_HSD2 when bit8 of byte3 of 0x1FF004AB is "1"
    if( (msg->id == 0x1FF004AB) && (msg->id_ext == 1) )
    {
        os_digout(OUT_HSD2, (msg->data[3] & 0x80));
    }
    else
    {
        // do nothing
    }
    */



/// \ingroup user
/// \brief           Receive interrupt of serial communication interface (RS485)
//--------------------------------------------------------------------------
///                  When a message on SCI (RS485) is received, this function is
///                  called directly in the interrupt
///
/// \param data      The received byte
/// \return          None
//--------------------------------------------------------------------------
void user_int_rx_sci(uint8_t module, uint8_t data)
{
    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    (void)module;
    (void)data;
}


/// \ingroup user
/// \brief           Timer tpm2 interrupt every 1ms
//--------------------------------------------------------------------------
///                  Take care not to put much code in here
///
/// \return          None
//--------------------------------------------------------------------------
void user_int_timer_1ms(void)
{
    uint8_t canale;
    uint8_t statoNuovo;
    uint32_t fase;

    for (canale = 0; canale < 4; canale++)
    {
        if (canale != 2)
        {
            fase = (uint32_t) usciteDigitaliFase[canale] + usciteDigitaliGiri[canale];
            if (fase >= 60000UL)
            {
                fase -= 60000UL;
            }
            usciteDigitaliFase[canale] = (uint16_t) fase;
            statoNuovo = (fase < 30000UL);
            if (statoNuovo != usciteDigitaliStato[canale])
            {
                usciteDigitaliStato[canale] = statoNuovo;
                os_pin_write(usciteDigitaliPin[canale], statoNuovo);
            }
        }
    }

	if (tmr.t_Clock100ms > 0)
	{
		tmr.t_Clock100ms--;
	}
	else
	{
		tmr.s_Clock100ms = !tmr.s_Clock100ms;
		tmr.t_Clock100ms = 100;
	}
	
	if (tmr.t_Clock250ms > 0)
	{
		tmr.t_Clock250ms--;
	}
	else
	{
		tmr.s_Clock250ms = !tmr.s_Clock250ms;
		tmr.t_Clock250ms = 250;
	}
	
	if (t_TimeOPUS > 0)
	{
		t_TimeOPUS--;
	}

	if (t_TimeEvent > 0)
	{
		t_TimeEvent--;
	}
	
	if (t_TimeForzaOut > 0)
	{
		t_TimeForzaOut--;
	}

	dAttrezzo.dMaster.p.iGiri = (os_algin_mv(iwGiri) > 7000);

/*
	if (dAttrezzo.dGen.p.tempoImpulsoGiro < 60000)
	{
		dAttrezzo.dGen.p.tempoImpulsoGiro++;
	}
*/	
	if ((dAttrezzo.dMaster.p.iGiri) && (dAttrezzo.dGen.p.iGiriPrec == 0))
	{
		if (dAttrezzo.dGen.p.nImpCoclea < 250)
		{
			dAttrezzo.dGen.p.nImpCoclea += 1;
		}
/*		
		if ((dAttrezzo.dGen.p.tempoImpulsoGiro > 0) && (dAttrezzo.dGen.p.tempoImpulsoGiro < 30000))
		{
			dAttrezzo.dGen.p.RPMCocleaTempo = 60000 / dAttrezzo.dGen.p.tempoImpulsoGiro;
		}
		else
		{
			dAttrezzo.dGen.p.RPMCocleaTempo = 0;
		}
			
		dAttrezzo.dGen.p.tempoImpulsoGiro = 0;
*/		
	}
	
	dAttrezzo.dGen.p.iGiriPrec = dAttrezzo.dMaster.p.iGiri;
}


/// \ingroup user
/// \brief           Timer tpm1 interrupt
//--------------------------------------------------------------------------
///                  If activated this function is called on timer interrupt
///                  For initialization set #define TIMER_CHANNELS_INT_ACTIVE in graph_defines.h and call
///                  os_init_timer_channel(uint8_t channel, uint8_t pulse_type); in usercode_init();.
///                  Possible pulse_type: PWM_READ_RISING, PWM_READ_FALLING, PWM_READ_RISING_FALLING
/// \return          None
//--------------------------------------------------------------------------
void user_int_timer_channels(uint8_t channel)
{

}


/// \ingroup user
/// \brief           Port interrupt
//--------------------------------------------------------------------------
///                  If activated this function is called on port interrupt
///
/// \param k         Index of os_port_int_val[] - structure
/// \return          None
//--------------------------------------------------------------------------
void user_int_port(uint8_t k)
{
    // --------------------------------------------------------------------------------
    // in the case of no using to get no compiler INFO set all as (void)xy;
    // --------------------------------------------------------------------------------
    (void)k;

    // For initialization set #define PORT_INTERRUPT_ACTIVE in graph_defines.h

    // If only one port interrupt is active, you know which one it is (discard parameter k)
    // If several port interrupts are active, they should either be on different ports or
    // you can check logic level with os_pin_read() to decide depending on interrupt edge
    // if this pin generated the port interrupt
}


/// \ingroup user
/// \brief           ADC-interrupt
//--------------------------------------------------------------------------
///                  This procedure is called, when an ADC-Value is
///                  converted and ready to be read.
///
/// \return          None
//--------------------------------------------------------------------------
void user_int_adc(void)
{
  (void)bios_analog_get_channel_single(); // This function returns the 12bit ADC-value
}


/// \ingroup user
/// \brief           Status interrupt of serial communication interface
//--------------------------------------------------------------------------
///                  When a status interrupt on SCI occurs, this function is
///                  called directly in the interrupt
///
/// \return          None
//--------------------------------------------------------------------------
void user_int_stat_sci(void)
{

}



// EOF
