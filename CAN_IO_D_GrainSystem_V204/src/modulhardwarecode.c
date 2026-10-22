//--------------------------------------------------------------------------
/// \file     modulhardwarecode.c
/// \brief    Multiplexen der HSD-Statuswerte. Dazu muss per CS_DIS vom HSD
///           so umgeschalten werden, dass die Werte des HSD1 statt HSD0 
///           gelesen werden.
/// \author   dr/
/// \date     26.04.10
/// \version  1.00
//--------------------------------------------------------------------------
#include "bios.h"
#include "os.h"

void modulhardwarecode (void) {    
     
}


void modulhardwarecode_adc_multiplex(uint8 multiplex_nr) {

    bios_disable_interrupts();  
    if (multiplex_nr == 1)
      os_pin_write(STAT_DIS,1); 
    else
      os_pin_write(STAT_DIS,0);
    bios_enable_interrupts();   

}


void modulhardwarecode_timer(void) { }


void modulhardwarecode_sleep(void) 
{     
    DisableInterrupts;                            
    
    (void)Start_wait(30); // 1ms warten // os_wait() nicht möglich da Interrupts deaktiviert.
         
    os_pin_write(CAN_STB, 0);     
      
    // Only for TJA1054:                                
    (void)Start_wait(30); // 1ms warten
    os_pin_write(CAN_EN, 0);     
    
    os_pin_write(POWER, 0);
    
    (void)Start_wait(7000); // Zur Sicherheit ca. 200ms auf Sleep warten

    // Notstrategie, falls nicht erfolgreich
    os_pin_write(POWER, 1);
    
    os_pin_write(CAN_STB, 1);     
    os_pin_write(CAN_EN, 1);         
    
    EnableInterrupts;		      
}


void modulhardwarecode_init(void) { }
