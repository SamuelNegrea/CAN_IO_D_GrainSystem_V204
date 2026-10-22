#ifndef _CAN_DB_TABLES_H_
#define _CAN_DB_TABLES_H_


/*--------------------------------------------------------------------------*/
/** \file     can_db_tables.h
*   \brief    CAN bus database
*
*   \date     19/10/2026    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------
*   \defgroup Name      can_tables
* --------------------------------------------------------------------------*/



// Change the Baudrate either to extended mode or a fix rate from the list below:
#define CAN_BAUDRATE BIOS_CAN_EXT_BAUDRATE
// einstellbare BAUDRATEN
// BIOS_CAN_EXT_BAUDRATE
// BIOS_CAN_1MBIT
// BIOS_CAN_800KBIT
// BIOS_CAN_500KBIT
// BIOS_CAN_250KBIT
// BIOS_CAN_125KBIT
// BIOS_CAN_100KBIT
// BIOS_CAN_83_33KBIT
// BIOS_CAN_50KBIT
// BIOS_CAN_33_33KBIT
// BIOS_CAN_20KBIT
// BIOS_CAN_10KBIT


//SETTING FOR BIOS_CAN_EXT_BAUDRATE 
#define  CAN_BAUDRATE_def_BEZEICHNUNG  250 kBit
#define  CAN_BAUDRATE_def_sj           2
#define  CAN_BAUDRATE_def_pre          1
#define  CAN_BAUDRATE_def_t_seg1       13
#define  CAN_BAUDRATE_def_t_seg2       2
#define  CAN_BAUDRATE_def_source       0



// Activate the user-defined filter settings:
#define CAN_USER_SETTINGS_ACTIVE

#ifdef CAN_USER_SETTINGS_ACTIVE

    // Settings of CAN filters and masks:
    #define CAN_FILTER_SETTING BIOS_CAN_ID_NORM_A_AND_B
    // Possibilities:
    // BIOS_CAN_ID_NORM_A_AND_B  // Both masks 11bit
    // BIOS_CAN_ID_NORM_A_EXT_B  // One mask 11bit, the other 29bit
    // BIOS_CAN_ID_EXT_A_AND_B   // Both masks 29bit
    // BIOS_CAN_ID_ALL_CLOSED    // All filters closed
    // BIOS_CAN_ID_ALL_OPEN      // All filters open


    // Filter A
    #define CAN_FILTER_A   0x22F
    #define CAN_MASK_A     0x005

    // Filter B
    #define CAN_FILTER_B   0x275
    #define CAN_MASK_B     0x007

    #define CAN_4_FILTERS_ACTIVE
    #ifdef CAN_4_FILTERS_ACTIVE

        // Filter A_4
        #define CAN_FILTER_A_4   0x22A
        #define CAN_MASK_A_4     0x000
        // Filter B_4
        #define CAN_FILTER_B_4   0x22F
        #define CAN_MASK_B_4     0x004
        // Filter C_4
        #define CAN_FILTER_C_4   0x272
        #define CAN_MASK_C_4     0x000
        // Filter D_4
        #define CAN_FILTER_D_4   0x275
        #define CAN_MASK_D_4     0x005
    #endif

#endif

/// Define CAN busses: All the possible CAN busses. Only use these busses in table #can_block_db_const
typedef enum
{
    CAN_BUS_1 = 0,
    CAN_BUS_MAX
} can_bus_id;



/// Define datapoints: Order must be the same as in table#can_datenpunkt_db_const
typedef enum
{
    RD_W0_0x22A             = 0,
    RD_W1_0x22A                ,
    RD_W2_0x22A                ,
    RD_W3_0x22A                ,
    RD_W0_0x22B                ,
    RD_W1_0x22B                ,
    RD_W2_0x22B                ,
    RD_W3_0x22B                ,
    RD_W0_0x22F                ,
    RD_W1_0x22F                ,
    RD_W2_0x22F                ,
    RD_W3_0x22F                ,
    RD_W0_0x271                ,
    RD_W1_0x271                ,
    RD_W2_0x271                ,
    RD_W3_0x271                ,
    RD_W0_0x272                ,
    RD_W1_0x272                ,
    RD_W2_0x272                ,
    RD_W3_0x272                ,
    RD_W0_0x274                ,
    RD_W1_0x274                ,
    RD_W2_0x274                ,
    RD_W3_0x274                ,
    RD_W0_0x275                ,
    RD_W1_0x275                ,
    RD_W2_0x275                ,
    RD_W3_0x275                ,
    WR_W0_0x221                ,
    WR_W1_0x221                ,
    WR_W2_0x221                ,
    WR_W3_0x221                ,
    WR_W0_0x222                ,
    WR_W1_0x222                ,
    WR_W2_0x222                ,
    WR_W3_0x222                ,
    WR_W0_0x223                ,
    WR_W1_0x223                ,
    WR_W2_0x223                ,
    WR_W3_0x223                ,
    WR_W0_0x224                ,
    WR_W1_0x224                ,
    WR_W2_0x224                ,
    WR_W3_0x224                ,
    WR_W0_0x261                ,
    WR_W1_0x261                ,
    WR_W2_0x261                ,
    WR_W3_0x261                ,
    WR_W0_0x262                ,
    WR_W1_0x262                ,
    WR_W2_0x262                ,
    WR_W3_0x262                ,
    WR_W0_0x263                ,
    WR_W1_0x263                ,
    WR_W2_0x263                ,
    WR_W3_0x263                ,
    WR_W0_0x264                ,
    WR_W1_0x264                ,
    WR_W2_0x264                ,
    WR_W3_0x264                ,
    WR_W0_0x265                ,
    WR_W1_0x265                ,
    WR_W2_0x265                ,
    WR_W3_0x265                ,
    WR_W0_TEST1                ,
    WR_W1_TEST1                ,
    WR_W2_TEST1                ,
    WR_W3_TEST1                ,
    CAN_DP_MAX            
}can_dp_id;




/// Define CAN blocks: Order must be the same as in table #can_block_db_const
typedef enum
{
    WR_0x221                = 0,
    WR_0x222                = 1,
    WR_0x223                = 2,
    WR_0x224                = 3,
    RD_0x22A                = 4,
    RD_0x22F                = 5,
    WR_0x261                = 6,
    WR_0x262                = 7,
    WR_0x263                = 8,
    WR_0x264                = 9,
    RD_0x271                = 10,
    RD_0x272                = 11,
    WR_TEST1                = 12,
    RD_0x22B                = 13,
    RD_0x274                = 14,
    WR_0x265                = 15,
    RD_0x275                = 16,
    CAN_BLOCK_MAX           = 17
} can_block_id;




#endif


