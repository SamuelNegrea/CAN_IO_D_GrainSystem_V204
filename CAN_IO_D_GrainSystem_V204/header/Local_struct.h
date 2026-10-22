//Mappa MRS

typedef union
{
	uint16_t w[64];

	struct 
	{
		uint16_t wr_0x221_W[4];
		uint16_t wr_0x222_W[4];
		uint16_t wr_0x223_W[4];
		uint16_t wr_0x224_W[4];
		uint16_t wr_0x225_W[4];
		uint16_t wr_0x226_W[4];
		
		uint16_t rd_0x22A_W[4];
		uint16_t rd_0x22B_W[4];
		uint16_t rd_0x22F_W[4];
	} wCAN;

	struct 
	{
		uint8_t wr_0x221_Byte[8];
		uint8_t wr_0x222_Byte[8];
		uint8_t wr_0x223_Byte[8];
		uint8_t wr_0x224_Byte[8];
		uint8_t wr_0x225_Byte[8];
		uint8_t wr_0x226_Byte[8];

		uint8_t rd_0x22A_Byte[8];
		uint8_t rd_0x22B_Byte[8];
		uint8_t rd_0x22F_Byte[8];
	} pBA;
	
	struct 
	{
		uint8_t wrByteIn1;
		uint8_t wrByteIn2;
		uint8_t wrByteOut1;
		uint8_t wrByteOut2;
		uint8_t wrByteStato1;
		uint8_t wrByteStato2;
		uint8_t wrByteAlarm1;
		uint8_t wrByteAlarm2;

		uint8_t wrByte0_0x222;
		uint8_t wrByte1_0x222;
		uint8_t wrByte2_0x222;
		uint8_t wrByte3_0x222;
		uint8_t wrByte4_0x222;
		uint8_t wrByte5_0x222;
		uint8_t wrByte6_0x222;
		uint8_t wrByte7_0x222;

		uint8_t wrByte0_0x223;
		uint8_t wrByte1_0x223;
		uint8_t wrByte2_0x223;
		uint8_t wrByte3_0x223;
		uint8_t wrByte4_0x223;
		uint8_t wrByte5_0x223;
		uint8_t wrByte6_0x223;
		uint8_t wrByte7_0x223;

		uint8_t wrByte0_0x224;
		uint8_t wrByte1_0x224;
		uint8_t wrByte2_0x224;
		uint8_t wrByte3_0x224;
		uint8_t wrByte4_0x224;
		uint8_t wrByte5_0x224;
		uint8_t wrByte6_0x224;
		uint8_t wrByte7_0x224;

		uint8_t wrByte0_0x225;
		uint8_t wrByte1_0x225;
		uint8_t wrByte2_0x225;
		uint8_t wrByte3_0x225;
		uint8_t wrByte4_0x225;
		uint8_t wrByte5_0x225;
		uint8_t wrByte6_0x225;
		uint8_t wrByte7_0x225;

		uint8_t wrByte0_0x226;
		uint8_t wrByte1_0x226;
		uint8_t wrByte2_0x226;
		uint8_t wrByte3_0x226;
		uint8_t wrByte4_0x226;
		uint8_t wrByte5_0x226;
		uint8_t wrByte6_0x226;
		uint8_t wrByte7_0x226;

		uint8_t rdByte0_0x22A;
		uint8_t rdByte1_0x22A;
		uint8_t rdByte2_0x22A;
		uint8_t rdByte3_0x22A;
		uint8_t rdByte4_0x22A;
		uint8_t rdByte5_0x22A;
		uint8_t rdByte6_0x22A;
		uint8_t rdByte7_0x22A;

		uint8_t rdByte0_0x22B;
		uint8_t rdByte1_0x22B;
		uint8_t rdByte2_0x22B;
		uint8_t rdByte3_0x22B;
		uint8_t rdByte4_0x22B;
		uint8_t rdByte5_0x22B;
		uint8_t rdByte6_0x22B;
		uint8_t rdByte7_0x22B;

		uint8_t rdByteOut1;
		uint8_t rdByteOut2;
		uint8_t rdByte2_0x22F;
		uint8_t rdByte3_0x22F;
		uint8_t rdByteOut1Slave2;
		uint8_t rdByteOut2Slave2;
		uint8_t rdByte6_0x22F;
		uint8_t rdByte7_0x22F;
	} pB;
	
	struct 
	{
		//Write 0x221
		unsigned iBraccioDX: 1;
		unsigned iBraccioSX: 1;
		unsigned iAspoBasso: 1;
		unsigned iAspoAvantiDX: 1;
		unsigned iAspoAvantiSX: 1;
		unsigned iGiri: 1;
		unsigned iQuadroOn: 1;
		unsigned b7_0_0x221: 1;
		
		unsigned b0_1_0x221: 1;
		unsigned b1_1_0x221: 1;
		unsigned b2_1_0x221: 1;
		unsigned b3_1_0x221: 1;
		unsigned b4_1_0x221: 1;
		unsigned b5_1_0x221: 1;
		unsigned b6_1_0x221: 1;
		unsigned b7_1_0x221: 1;
		
		unsigned oRitenuta: 1;
		unsigned b1_2_0x221: 1;
		unsigned oEV1: 1;
		unsigned oEV2: 1;
		unsigned oEV3: 1;
		unsigned oEV4: 1;
		unsigned b6_2_0x221: 1;
		unsigned b7_2_0x221: 1;
		
		unsigned b0_3_0x221: 1;
		unsigned b1_3_0x221: 1;
		unsigned b2_3_0x221: 1;
		unsigned b3_3_0x221: 1;
		unsigned b4_3_0x221: 1;
		unsigned b5_3_0x221: 1;
		unsigned b6_3_0x221: 1;
		unsigned b7_3_0x221: 1;

		unsigned startUP: 1;
		unsigned okSistema: 1;
		unsigned microBracciON: 1;
		unsigned microBracciOFF: 1;
		unsigned microAspoON: 1;
		unsigned microAspoOFF: 1;
		unsigned inLavoro: 1;
		unsigned b7_4_0x221: 1;

		unsigned b0_5_0x221: 1;
		unsigned b1_5_0x221: 1;
		unsigned b2_5_0x221: 1;
		unsigned b3_5_0x221: 1;
		unsigned b4_5_0x221: 1;
		unsigned b5_5_0x221: 1;
		unsigned b6_5_0x221: 1;
		unsigned vivo: 1;

		unsigned aEV1: 1;
		unsigned aEV2: 1;
		unsigned aEV3: 1;
		unsigned aEV4: 1;
		unsigned aManutenzione1: 1;
		unsigned aManutenzione2: 1;
		unsigned aManutenzione3: 1;
		unsigned aManutenzione4: 1;
		
		unsigned timeOutTast: 1;
		unsigned aMaxGiri: 1;
		unsigned aPressione1: 1;
		unsigned aPressione2: 1;
		unsigned aMaxPressione1: 1;
		unsigned aMaxPressione2: 1;
		unsigned b6_7_0x221: 1;
		unsigned b7_7_0x221: 1;

		//Write 0x222
		uint16_t delayShutDown;
		uint16_t word1_0x222;
		uint16_t word2_0x222;
		uint16_t word3_0x222;
		
		//Write 0x223
		int16_t iwPressione1;
		int16_t iwPressione2;
		int16_t rpmGiri;
		int16_t versionSW;
		
		//Write 0x224
		uint16_t mAEV1;
		uint16_t mAEV2;
		uint16_t mAEV3;
		uint16_t mAEV4;

		//Write 0x225
		uint16_t word0_0x225;
		uint16_t word1_0x225;
		uint16_t word2_0x225;
		uint16_t word3_0x225;

		//Write 0x226
		uint16_t word0_0x226;
		uint16_t word1_0x226;
		uint16_t word2_0x226;
		uint16_t word3_0x226;
		
		//Read 0x22A comandi da tastierino
		unsigned reqEV1: 1;
		unsigned reqEV2: 1;
		unsigned reqEV3: 1;
		unsigned reqEV4: 1;
		unsigned b4_0_0x22A: 1;
		unsigned b5_0_0x22A: 1;
		unsigned b6_0_0x22A: 1;
		unsigned b7_0_0x22A: 1;
		
		unsigned resetContaOre1Opus: 1;
		unsigned resetContaOre2Opus: 1;
		unsigned resetContaOre3Opus: 1;
		unsigned resetContaOre4Opus: 1;
		unsigned resetContaOreCliente: 1;
		unsigned b5_1_0x22A: 1;
		unsigned b6_1_0x22A: 1;
		unsigned vivoOPUS: 1;
		
		uint8_t byte2_0x22A;
		uint8_t byte3_0x22A;
		uint8_t byte4_0x22A;
		uint8_t byte5_0x22A;
		uint8_t byte6_0x22A;
		uint8_t byte7_0x22A;
		
		//Read 0x22B comandi da event
		unsigned disabilitaEV1EV2: 1;
		unsigned b1_0_0x22B: 1;
		unsigned b2_0_0x22B: 1;
		unsigned b3_0_0x22B: 1;
		unsigned b4_0_0x22B: 1;
		unsigned b5_0_0x22B: 1;
		unsigned b6_0_0x22B: 1;
		unsigned b7_0_0x22B: 1;
		
		unsigned resetContaOre1Event: 1;
		unsigned resetContaOre2Event: 1;
		unsigned resetContaOre3Event: 1;
		unsigned resetContaOre4Event: 1;
		unsigned b4_1_0x22B: 1;
		unsigned b5_1_0x22B: 1;
		unsigned b6_1_0x22B: 1;
		unsigned b7_1_0x22B: 1;
		
		uint8_t byte2_0x22B;
		uint8_t byte3_0x22B;
		uint8_t byte4_0x22B;
		uint8_t byte5_0x22B;
		uint8_t byte6_0x22B;
		uint8_t byte7_0x22B;
		
		//Read 0x22F Forzature uscite
		unsigned foRitenuta: 1;
		unsigned foSpiaRossa: 1;
		unsigned foEV1: 1;
		unsigned foEV2: 1;
		unsigned foEV3: 1;
		unsigned foEV4: 1;
		unsigned b6_0_0x22F: 1;
		unsigned b7_0_0x22F: 1;
		
		unsigned b0_1_0x22F: 1;
		unsigned b1_1_0x22F: 1;
		unsigned b2_1_0x22F: 1;
		unsigned b3_1_0x22F: 1;
		unsigned b4_1_0x22F: 1;
		unsigned b5_1_0x22F: 1;
		unsigned b6_1_0x22F: 1;
		unsigned forzaUscite: 1;

		uint16_t word1_0x22F;
		uint16_t word2_0x22F;
		uint16_t word3_0x22F;
	} p;
} dataMasterMRS;

