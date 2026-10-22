#include "src/bios_PE_Types.h"

#include "header\ee_struct.h"
#include "header\Local_struct.h"
#include "header\veicolo_struct.h"
#include "header\timer_struct.h"

eeProm ee;

dataVehicle dAttrezzo;

myTimer tmr;

uint16_t t_TimeOPUS = 0;
uint16_t t_TimeEvent = 0;
uint16_t t_TimeForzaOut = 0;
