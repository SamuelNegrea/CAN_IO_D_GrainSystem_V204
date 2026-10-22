#ifndef _OS_IO_TABLES_H_
#define _OS_IO_TABLES_H_


/*--------------------------------------------------------------------------*/
/** \file     os_io_tables.h
*   \brief    Tables for definitions of I/O ports and their properties
*
*   \date     19/10/2026    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------
*   \defgroup Name      io
* --------------------------------------------------------------------------*/


#include "bios_base.h"
#include "os_io.h"

#define OSC_CLK_4MHZ 0
#define OSC_CLK_8MHZ 1


/** \ingroup io
*   \brief           List of used pin names
* --------------------------------------------------------------------------*
*                    All the used pin names are inserted in this table.
*                    IMPORTANT: The order of the pins has to be the same as
*                    in #os_port_pin!
* --------------------------------------------------------------------------*/
enum {
D_IN0                 =0,
inQuadroOn            ,
inBraccioDX           ,
inBraccioSX           ,
D_IN4                 ,
D_IN5                 ,
D_IN6                 ,
D_IN7                 ,
inAspoAvantiDX        ,
inAspoAvantiSX        ,
inAspoBasso           ,
D_ANA3                ,
D_ANA4                ,
inGiri                ,
KL15                  ,
CAN_ERR               ,
DI_VREF               ,
OS_PORT_INPUT_PIN_MAX ,
outRitenuta           ,
OUT_HSD1              ,
OUT_HSD2              ,
OUT_HSD3              ,
outEV1                ,
outEV2                ,
outEV3                ,
outEV4                ,
POWER                 ,
CAN_WAKE              ,
CAN_EN                ,
CAN_STB               ,
STAT_DIS              ,
MOD_ASC1              ,
OS_PORT_PIN_MAX       
};




typedef struct {
  uint16_t name;
  uint16_t port;
  uint8_t bit;
  uint8_t direction;
  uint8_t pullup;
  uint8_t pull_enable;
  uint8_t int_edge;
  uint8_t int_enable;
  uint8_t value;
  uint8_t hi_entprell_cnt;
  uint8_t lo_entprell_cnt;
  uint8_t multiplex;
  uint8_t inverted;
}os_port_pin_def;

extern const os_port_pin_def os_port_pin[];




/** \ingroup io
*   \brief           List of analog pins
* --------------------------------------------------------------------------*
*                    All the used analog pins are inserted in this table.
*                    IMPORTANT: The order of the pins has to be the same as
*                    in #os_analog_pin!
* --------------------------------------------------------------------------*/
enum {
iwAspoAvantiDX        =0,
iwAspoAvantiSX        ,
iwAspoBasso           ,
iwPressione2          ,
iwPressione1          ,
iwGiri                ,
ANA_IO0               ,
ANA_IO1               ,
ANA_IO2               ,
ANA_IO3               ,
ANA_IO4               ,
ANA_IO5               ,
STAT_ANA_HSD0         ,
STAT_ANA_HSD1         ,
STAT_ANA_HSD2         ,
STAT_ANA_HSD3         ,
STAT_ANA_HSD4         ,
STAT_ANA_HSD5         ,
STAT_ANA_HSD6         ,
STAT_ANA_HSD7         ,
ANA_TEMP              ,
OS_ANALOG_PIN_MAX     
};




typedef struct {
  uint16_t name;
  uint8_t  adc_channel;
  uint8_t  direction;
  uint16_t value;
  uint16_t min;
  uint16_t max;
  uint8_t  error_filter;
  uint8_t  entprell_cnt;
  uint16_t adc_max;
  uint8_t  multiplex;
  uint8_t  multiplex_type;
  uint8_t  parameter1;   // e.g. HSD number to calculate calibration value of current flow
  uint8_t  samples;   
}os_analog_pin_def;

extern const os_analog_pin_def os_analog_pin[];


/** \ingroup io
*   \brief           List of PWM pins
* --------------------------------------------------------------------------*
*                    All the used PWM pins are inserted in this table.
*                    IMPORTANT: The order of the pins has to be the same as
*                    in #os_pwm_pin!
* --------------------------------------------------------------------------*/
enum {
PWM_IO2               =0,
PWM_IO3               ,
PWM_IO4               ,
PWM_IO5               ,
PWM_IO6               ,
PWM_IO7               ,
OS_PWM_PIN_MAX        
};



typedef struct {
  uint8_t  name;
  uint8_t  timer_channel;
  uint8_t  pulse_type;
  uint8_t  init_type;
  uint8_t  adc_active;
  uint8_t  adc_channel;
  uint16_t adc_max_volt;
  uint8_t  adc_filter;
  uint8_t  adc_startbed;
  uint8_t  digout_vorw;
  uint8_t  digout_rueckw;
  uint16_t current_max;
  uint8_t  digout_index;
}os_pwm_pin_def;

extern const os_pwm_pin_def os_pwm_pin[];



/** \ingroup io
*   \brief           List of EEPROM_INDEX
* --------------------------------------------------------------------------*
*                    Enum for EEPROM_INDEX instead of numbers
* --------------------------------------------------------------------------*/

// Nothing defined


/** \ingroup io
*   \brief           List of USERVAR_INDEX
* --------------------------------------------------------------------------*
*                    Enum for USERVAR_INDEX instead of numbers
* --------------------------------------------------------------------------*/

// Nothing defined


extern const uint8_t bios_osc_clk;

extern const uint8_t modul_geraetetyp_nr;

#define MACRO_MODUL_GERAETETYP_NR 0

#define HSDDATENSATZ_SIZE_DYNAMIC 57

//#define USER_VARIABLE_SIZE_DYNAMIC 10

#define IO_DEF_FILENAME "CAN_IO_V2"

#endif

