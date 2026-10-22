//Mappa globale macchina
typedef union
{
	uint16_t w;

	struct 
	{
		uint8_t b[2];
	} pB;
	
	struct 
	{
		unsigned iBraccioDX: 1;
		unsigned iBraccioSX: 1;
		unsigned iAspoBasso: 1;
		unsigned iAspoAvantiDX: 1;
		unsigned iAspoAvantiSX: 1;
		unsigned iGiri: 1;
		unsigned b6_0: 1;
		unsigned b7_0: 1;
		
		unsigned resetContaOre1Event: 1;
		unsigned resetContaOre2Event: 1;
		unsigned resetContaOre3Event: 1;
		unsigned resetContaOre4Event: 1;
		unsigned b4_1: 1;
		unsigned b5_1: 1;
		unsigned b6_1: 1;
		unsigned b7_1: 1;
	} p;
} dataInput;

typedef union
{
	uint8_t b;
	
	struct 
	{
		unsigned delayAlarm: 7;
		unsigned aEV: 1;
	} p;
} dataCheckEV;

typedef union
{
	uint16_t w[32];

	struct 
	{
		uint8_t b[64];
	} pBB;
		
	struct 
	{
		uint8_t upIn1;				//00
		uint8_t upIn2;				//01
		
		uint8_t dnIn1;				//02
		uint8_t dnIn2;				//03
		
		uint8_t sPrecIn1;			//04
		uint8_t sPrecIn2;			//05

		uint8_t stato1;				//06
		uint8_t stato2;				//07
	} pB;
	
	struct 
	{
		dataInput upIn;						//00 - 01
		dataInput dnIn;						//02 - 03
		dataInput spIn;						//04 - 05
		
		unsigned timeOutOPUS: 1;			//06
		unsigned timeOutEvent: 1;
		unsigned b2_06: 1;
		unsigned b3_06: 1;
		unsigned microBracciON: 1;
		unsigned microAspoON: 1;
		unsigned eEV4: 1;
		unsigned b7_06: 1;

		unsigned aMaxGiri: 1;				//07
		unsigned aMaxPressione1: 1;
		unsigned aMaxPressione2: 1;
		unsigned b3_07: 1;
		unsigned b4_07: 1;
		unsigned b5_07: 1;
		unsigned b6_07: 1;
		unsigned iGiriPrec: 1;
		
		uint32_t secLavoroTotalePrec;		//08 - 11
		
		uint8_t delayMaxGiriON;				//12
		uint8_t delayMaxGiriOFF;			//13

		uint8_t delayMaxPressione1ON;		//14
		uint8_t delayMaxPressione1OFF;		//15

		uint8_t delayMaxPressione2ON;		//16
		uint8_t delayMaxPressione2OFF;		//17
		
		uint8_t clockRPMCoclea;				//18
		uint8_t nImpCoclea;					//19
		
		uint16_t iwRPMCoclea;				//20 - 21
		
		uint16_t tempoImpulsoGiro;			//22 - 23
		uint16_t RPMCocleaTempo;			//24 - 25
		
		uint8_t delayBracciON;				//26
		uint8_t delayAspoOn;				//27
		
		dataCheckEV chkEV1;					//28
		dataCheckEV chkEV2;					//29
		dataCheckEV chkEV3;					//30
		dataCheckEV chkEV4;					//31
	} p;
} dataGenMRS;

typedef struct 
{
	dataMasterMRS dMaster;
	
	dataGenMRS dGen;
	
	uint16_t delayStart;
	uint16_t clockWriteEE;
} dataVehicle;
