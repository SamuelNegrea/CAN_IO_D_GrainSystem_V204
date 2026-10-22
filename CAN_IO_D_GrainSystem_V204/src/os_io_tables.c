/*--------------------------------------------------------------------------*/
/** \file     os_io_tables.c
*   \brief    Functions of operating system for initialization
*             and access to ports and pins of processor
*
*   \date     19/10/2026    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------*/


#include "os_io_tables.h" 
#include "bios_io.h" 



/** \ingroup io
*   \brief           Definition of port pins
* --------------------------------------------------------------------------*
*                    All the used pins with their initialization values are
*                    inserted in this table. The write- and read-functions
*                    #os_digin() and #os_digout() use the pin name as parameter.
*                    Output pins have to be inserted below the line
*                    #OS_PORT_INPUT_PIN_MAX
*                    Inputs and outputs could be debounced as high-low as well as
*                    low-high edges. One cycle is one call of #os_update_digin()
*                    or #os_update_digout().
*
*                    IMPORTANT: After inserting new lines the enum in os_io_tables.h
*                    has to be updated!
* --------------------------------------------------------------------------*/
const os_port_pin_def os_port_pin[] = {
//                                                                                                             Debounce cycles:
//  Pin-Name              Port                  Bit                   Direction             Pull-Dir              Pull-Enable           Interrupt Edge        Interrupt-Enable      Value                 Lo-Hi                 Hi-Lo                 Multiplex             Inverted              
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                              (0-7)                 (PD_IN,               (PU_UP                (PE_ON                (PI_RISE              (PI_ON                (0,1)                 (0-255)               (0-255)               (0,1)                 (0,1)                 
//                                                                    PD_OUT),              PU_DOWN)              PE_OFF)               PI_FALL)              PI_OFF)                                                                                                                                                   
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{   D_IN0                 ,PORT_A               ,6                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A1 Digital Input I/O0
{   inQuadroOn            ,PORT_A               ,5                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A2 Digital Input I/O1
{   inBraccioDX           ,PORT_A               ,4                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A3 Digital Input I/O2
{   inBraccioSX           ,PORT_B               ,3                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A4 Digital Input I/O3
{   D_IN4                 ,PORT_B               ,2                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A5 Digital Input I/O4
{   D_IN5                 ,PORT_B               ,1                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A6 Digital Input I/O5
{   D_IN6                 ,PORT_E               ,2                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A7 Digital Input I/O6
{   D_IN7                 ,PORT_E               ,3                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN A8 Digital Input I/O7
{   inAspoAvantiDX        ,PORT_B               ,7                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN C7 Digital Input AI0
{   inAspoAvantiSX        ,PORT_A               ,1                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN C6 Digital Input AI1
{   inAspoBasso           ,PORT_A               ,2                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN C5 Digital Input AI2
{   D_ANA3                ,PORT_A               ,3                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN C4 Digital Input AI3
{   D_ANA4                ,PORT_B               ,4                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN C3 Digital Input AI4
{   inGiri                ,PORT_B               ,5                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // PIN C2 Digital Input AI5
{   KL15                  ,PORT_F               ,4                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,1                    ,1                    ,0                    ,0                    }, // PIN C8 Digital Input Kl15
{   CAN_ERR               ,PORT_F               ,1                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // Error PIN of the CAN-LS Transceiver
{   DI_VREF               ,PORT_E               ,1                    ,PD_IN                ,DONT_CARE            ,PE_OFF               ,0                    ,DONT_CARE            ,DONT_CARE            ,0                    ,0                    ,0                    ,0                    }, // Pin B7 Digital input DI_VREF
{   OS_PORT_INPUT_PIN_MAX ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    },
{   outRitenuta           ,PORT_F               ,3                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A1 Digital Output I/O0
{   OUT_HSD1              ,PORT_D               ,1                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A2 Digital Output I/O1
{   OUT_HSD2              ,PORT_D               ,7                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A3 Digital Output I/O2
{   OUT_HSD3              ,PORT_D               ,6                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A4 Digital Output I/O3
{   outEV1                ,PORT_D               ,5                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A5 Digital Output I/O4
{   outEV2                ,PORT_D               ,4                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A6 Digital Output I/O5
{   outEV3                ,PORT_D               ,3                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A7 Digital Output I/O6
{   outEV4                ,PORT_D               ,2                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // PIN A8 Digital Output I/O7
{   POWER                 ,PORT_F               ,5                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // Self-holding, when Kl15 is removed the module holds itself awake
{   CAN_WAKE              ,PORT_E               ,4                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,0                    ,0                    ,0                    ,0                    ,0                    }, // Wake Pin of the CAN-LS Transceiver
{   CAN_EN                ,PORT_E               ,5                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,1                    ,0                    ,0                    ,0                    ,0                    }, // EN Pin of the CAN-LS Transceiver
{   CAN_STB               ,PORT_F               ,0                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,1                    ,0                    ,0                    ,0                    ,0                    }, // STB Pin of the CAN-LS Transceiver
{   STAT_DIS              ,PORT_F               ,2                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,MAGIC_NUMBER_INIT    ,0                    ,0                    ,0                    ,0                    }, // HSD multiplexing
{   MOD_ASC1              ,PORT_D               ,0                    ,PD_OUT               ,DONT_CARE            ,PE_OFF               ,DONT_CARE            ,PI_OFF               ,MAGIC_NUMBER_RS485_DE,0                    ,0                    ,0                    ,0                    }, // Modbus Transceiver or 1-Wire Communication
{   OS_PORT_PIN_MAX       ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    },
};




/** \ingroup io
*   \brief           Definition of analog inputs and outputs
* --------------------------------------------------------------------------*
*                    This is the list of analog inputs and outputs.
*                    You could enter a value range from Min to Max to generate
*                    an error if the value is not within the range.
*                    The errors could be masked by error filters
*                    ( #ERR_OFF, #ERR_MIN, #ERR_MAX, #ERR_MIN_MAX )
* --------------------------------------------------------------------------*/
const os_analog_pin_def os_analog_pin[] = {
//  Pin-Name              ADC-Channel           Direction             Value                 Min                   Max                   Error-Filter          Entprell-Takte        ADC-Max mV            Multiplex             Multiplex Type        Parameter 1           Samples               
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                              (PD_IN                (0-                   (0-                   (0-                   (ERR_OFF              (0-255)               (0-65535)             (0,1)                 (0-255)               (0-255)               (0-255)               
//                                              PD_OUT)               32767)                32767)                32767)                ERR_MIN                                                                                                                             0 Standard            
//                                                                                                                                      ERR_MAX,                                                                                                                            1-255 ms oversample   
//                                                                                                                                      ERR_MIN_MAX                                                                                                                                               
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{   iwAspoAvantiDX        ,15                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN C7 Analog Input AI0
{   iwAspoAvantiSX        ,1                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN C6 Analog Input AI1
{   iwAspoBasso           ,2                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN C5 Analog Input AI2
{   iwPressione2          ,3                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN C4 Analog Input AI3
{   iwPressione1          ,12                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN C3 Analog Input AI4
{   iwGiri                ,13                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 33670               ,0                    ,0                    ,0                    ,0                    }, // PIN C2 Analog Input AI5
{   ANA_IO0               ,6                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN A1 Analog Input I/O0
{   ANA_IO1               ,5                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN A2 Analog Input I/O1
{   ANA_IO2               ,4                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN A3 Analog Input I/O2
{   ANA_IO3               ,11                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN A4 Analog Input I/O3
{   ANA_IO4               ,10                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN A5 Analog Input I/O4
{   ANA_IO5               ,9                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 11400               ,0                    ,0                    ,0                    ,0                    }, // PIN A6 Analog Input I/O5
{   STAT_ANA_HSD0         ,8                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,0                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A1, I/O0
{   STAT_ANA_HSD1         ,0                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,0                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A2, I/O1
{   STAT_ANA_HSD2         ,14                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,0                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A3, I/O2
{   STAT_ANA_HSD3         ,7                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,0                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A4, I/O3
{   STAT_ANA_HSD4         ,8                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,1                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A5, I/O4
{   STAT_ANA_HSD5         ,0                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,1                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A6, I/O5
{   STAT_ANA_HSD6         ,14                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,1                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A7, I/O6
{   STAT_ANA_HSD7         ,7                    ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 9250                ,1                    ,1                    ,STAT_ANA_HSD0        ,0                    }, // Current flow on PIN A8, I/O7
{   ANA_TEMP              ,26                   ,PD_IN                ,DONT_CARE            ,0                    ,0                    ,ERR_OFF              ,0                    , 0                   ,0                    ,0                    ,224                  ,0                    }, // Temperature value
{   OS_ANALOG_PIN_MAX     ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    , 0                   ,0                    ,0                    ,0                    ,0                    }, // 0
};



/** \ingroup io
*   \brief           Definition of PWM outputs
* --------------------------------------------------------------------------*
*                    This table lists the properties of the PWM outputs.
* --------------------------------------------------------------------------*/
const os_pwm_pin_def os_pwm_pin[] = {
//  Pin-Name              Timer-Channel         Pulse Type            Init-Type             ADC active            ADC-Channel           ADC max volt          ADC Filter            ADC Startbed          Digout vorw           Digout rueckw         Current max           Digout index          
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                        (0-5)                 (HIGH_TRUE,           (0,1)                 (0,1)                 (0-23)                (0-                   (1-100)               (0-23)                (0-n)                 (0-n)                 (0-n)                 (0-n)                 
//                                              LOW_TRUE)                                                                               65535)                                                                                                                                                    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{   PWM_IO2               ,5                    ,HIGH_TRUE            ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,OUT_HSD2             }, // PIN A3 PWM Output I/O2
{   PWM_IO3               ,4                    ,HIGH_TRUE            ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,OUT_HSD3             }, // PIN A4 PWM Output I/O3
{   PWM_IO4               ,3                    ,HIGH_TRUE            ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,outEV1               }, // PIN A5 PWM Output I/O4
{   PWM_IO5               ,2                    ,HIGH_TRUE            ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,outEV2               }, // PIN A6 PWM Output I/O5
{   PWM_IO6               ,1                    ,HIGH_TRUE            ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,outEV3               }, // PIN A7 PWM Output I/O6
{   PWM_IO7               ,0                    ,HIGH_TRUE            ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,outEV4               }, // PIN A8 PWM Output I/O7
{   OS_PWM_PIN_MAX        ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    ,0                    },
};


// 4MHz or 8MHz oscillator => Adapt PLL multiplicator
const uint8_t bios_osc_clk = OSC_CLK_4MHZ;


// Device type number: This could activate module specific initialization code
const uint8_t modul_geraetetyp_nr = 0;

