#define EEPROM_SIZE			128

typedef union
{
	uint16_t w;
	
	struct 
	{
		unsigned ePressione1: 1;
		unsigned ePressione2: 1;
		unsigned b2_0: 1;
		unsigned b3_0: 1;
		unsigned b4_0: 1;
		unsigned b5_0: 1;
		unsigned b6_0: 1;
		unsigned b7_0: 1;

		unsigned b0_1: 1;
		unsigned b1_1: 1;
		unsigned b2_1: 1;
		unsigned b3_1: 1;
		unsigned b4_1: 1;
		unsigned b5_1: 1;
		unsigned b6_1: 1;
		unsigned b7_1: 1;
	} p;
} dataConfig;

// Mappa eeprom
typedef union
{
	int8_t b[EEPROM_SIZE];
		
	struct 
	{
		uint8_t test;						//000
		uint8_t by001;						//001

		dataConfig dCfg;					//002 - 003
		uint16_t maxGiri;					//004 - 005
		uint16_t taccheGiro;				//006 - 007
		uint16_t minGiri;					//008 - 009
		
		uint16_t fsPressione1;				//010 - 011
		uint16_t fsPressione2;				//012 - 013
		uint16_t maxPressione1;				//014 - 015
		uint16_t maxPressione2;				//016 - 017
		
		uint32_t secLavoroTotale;			//018 - 021
		uint32_t secLavoroParziale[4];		//022 - 037
		
		uint16_t oreManutenzione[4];		//038 - 045
		
		uint32_t secLavoroCliente;			//046 - 047
	} p;
} eeProm;
