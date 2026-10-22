/*--------------------------------------------------------------------------*/
/** \file     can_db_tables.c
*   \brief    CAN bus database
*
*   \date     19/10/2026    \author   DS/
*
*   \platform HCS08DZ / HCS12XE / HCS12XD / HCS12P
* --------------------------------------------------------------------------*/


#include "can_db_tables.h" 
#include "can_db.h"



/** \ingroup can_tables
*   \brief           Table of all CAN datapoints
* --------------------------------------------------------------------------*
*                    Every line defines a datapoint within a CAN block.
*                    Every datapoint is assigned to a CAN block ID
*                    #can_block_id, the order of the IDs has to be the same as
*                    the CAN block table #can_block_db_const.
*                    The table contains bit position, bit length and datatype.
*                    The datapoints are used with #can_db_set_value() or #can_db_get_value().
*                    As parameter use the name from the list #can_dp_id verwendet,
*                    the order has to be the same as in this table.
* --------------------------------------------------------------------------*/
 volatile const can_datenpunkt_db_const_typ can1_datenpunkt_db_const[CAN_DP_MAX+1] = { // Array can1_....  with the dp of one CAN interface
 //CAN block index        Pos bit0             Length,     Data type   Data format           LineNr: datapoint-ID  
 //-----------------------------------------------------------------------------------------------------------------
 //(16 Bit)              (0-63)                (1-32)                  (0=Intel, 1=Motorola)
 //-----------------------------------------------------------------------------------------------------------------
{  RD_0x22A              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x22A           
{  RD_0x22A              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x22A           
{  RD_0x22A              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x22A           
{  RD_0x22A              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x22A           
{  RD_0x22B              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x22B           
{  RD_0x22B              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x22B           
{  RD_0x22B              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x22B           
{  RD_0x22B              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x22B           
{  RD_0x22F              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x22F           
{  RD_0x22F              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x22F           
{  RD_0x22F              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x22F           
{  RD_0x22F              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x22F           
{  RD_0x271              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x271           
{  RD_0x271              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x271           
{  RD_0x271              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x271           
{  RD_0x271              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x271           
{  RD_0x272              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x272           
{  RD_0x272              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x272           
{  RD_0x272              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x272           
{  RD_0x272              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x272           
{  RD_0x274              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x274           
{  RD_0x274              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x274           
{  RD_0x274              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x274           
{  RD_0x274              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x274           
{  RD_0x275              ,0                    ,16         ,UINT       ,0          },  //RD_W0_0x275           
{  RD_0x275              ,16                   ,16         ,UINT       ,0          },  //RD_W1_0x275           
{  RD_0x275              ,32                   ,16         ,UINT       ,0          },  //RD_W2_0x275           
{  RD_0x275              ,48                   ,16         ,UINT       ,0          },  //RD_W3_0x275           
{  WR_0x221              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x221           
{  WR_0x221              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x221           
{  WR_0x221              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x221           
{  WR_0x221              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x221           
{  WR_0x222              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x222           
{  WR_0x222              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x222           
{  WR_0x222              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x222           
{  WR_0x222              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x222           
{  WR_0x223              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x223           
{  WR_0x223              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x223           
{  WR_0x223              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x223           
{  WR_0x223              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x223           
{  WR_0x224              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x224           
{  WR_0x224              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x224           
{  WR_0x224              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x224           
{  WR_0x224              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x224           
{  WR_0x261              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x261           
{  WR_0x261              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x261           
{  WR_0x261              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x261           
{  WR_0x261              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x261           
{  WR_0x262              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x262           
{  WR_0x262              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x262           
{  WR_0x262              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x262           
{  WR_0x262              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x262           
{  WR_0x263              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x263           
{  WR_0x263              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x263           
{  WR_0x263              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x263           
{  WR_0x263              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x263           
{  WR_0x264              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x264           
{  WR_0x264              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x264           
{  WR_0x264              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x264           
{  WR_0x264              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x264           
{  WR_0x265              ,0                    ,16         ,UINT       ,0          },  //WR_W0_0x265           
{  WR_0x265              ,16                   ,16         ,UINT       ,0          },  //WR_W1_0x265           
{  WR_0x265              ,32                   ,16         ,UINT       ,0          },  //WR_W2_0x265           
{  WR_0x265              ,48                   ,16         ,UINT       ,0          },  //WR_W3_0x265           
{  WR_TEST1              ,0                    ,16         ,UINT       ,0          },  //WR_W0_TEST1           
{  WR_TEST1              ,16                   ,16         ,UINT       ,0          },  //WR_W1_TEST1           
{  WR_TEST1              ,32                   ,16         ,UINT       ,0          },  //WR_W2_TEST1           
{  WR_TEST1              ,48                   ,16         ,UINT       ,0          },  //WR_W3_TEST1           
};

//--------------------------------------------------------------------------------------------------------
// Pointer array to datapoint_db arrays per CAN interface
//--------------------------------------------------------------------------------------------------------
volatile const can_datenpunkt_db_const_typ *can_datenpunkt_db_const[CAN_BUS_MAX+1] =
{
                     can1_datenpunkt_db_const ,  // can1_....  CAN interface 1
};


/** \ingroup can_tables
*   \brief           Table of all CAN blocks
* --------------------------------------------------------------------------*
*                    Every line defines a CAN block and sets the CAN-ID,
*                    the minimal and maximal time between to messages for sending,
*                    the length of a message and the direction (0=Rx, 1=Tx).
*                    The CAN-IDs must be defined in can_db.h.
*                    Every CAN block is assigned to a ID, the order has to be
*                    the same as in this table.
* --------------------------------------------------------------------------*/
 volatile const can_block_db_const_typ can1_block_db_const[CAN_BLOCK_MAX+1] = { // Array can1_....  with the CAN blocks of one CAN interface
 //CAN-ID                EXT-ID                Max,                  Min                   Msg-Len DLC           TX Flag               CAN-ID-Mask           
 //----------------------------------------------------------------------------------------------------------------------------------------------------------
 //(32 Bit)              (0,1)                 (32 Bit)              (32 Bit)              (0-8)                 (0,1)                 (29bit)               
 //----------------------------------------------------------------------------------------------------------------------------------------------------------
{  0x221                 ,0                    ,100                  ,100                  ,8                    ,1                    ,0                    } ,  //WR_0x221              
{  0x222                 ,0                    ,100                  ,100                  ,8                    ,1                    ,0                    } ,  //WR_0x222              
{  0x223                 ,0                    ,100                  ,100                  ,8                    ,1                    ,0                    } ,  //WR_0x223              
{  0x224                 ,0                    ,100                  ,100                  ,8                    ,1                    ,0                    } ,  //WR_0x224              
{  0x22A                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x22A              
{  0x22F                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x22F              
{  0x261                 ,0                    ,250                  ,250                  ,8                    ,1                    ,0                    } ,  //WR_0x261              
{  0x262                 ,0                    ,250                  ,250                  ,8                    ,1                    ,0                    } ,  //WR_0x262              
{  0x263                 ,0                    ,1000                 ,1000                 ,8                    ,1                    ,0                    } ,  //WR_0x263              
{  0x264                 ,0                    ,1000                 ,1000                 ,8                    ,1                    ,0                    } ,  //WR_0x264              
{  0x271                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x271              
{  0x272                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x272              
{  0x701                 ,0                    ,100                  ,100                  ,8                    ,1                    ,0                    } ,  //WR_TEST1              
{  0x22B                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x22B              
{  0x274                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x274              
{  0x265                 ,0                    ,1000                 ,1000                 ,8                    ,1                    ,0                    } ,  //WR_0x265              
{  0x275                 ,0                    ,0                    ,0                    ,8                    ,0                    ,0                    } ,  //RD_0x275              
};


//--------------------------------------------------------------------------------------------
// Pointer array to datapoint_db arrays per CAN interface
//--------------------------------------------------------------------------------------------
volatile const can_block_db_const_typ *can_block_db_const[CAN_BUS_MAX+1] =
{
                                 can1_block_db_const,      // Array can1_....  CAN interface 1
};




