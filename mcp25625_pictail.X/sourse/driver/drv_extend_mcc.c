/*******************************************************************************
* File Name       : drv_extend_mcc.c
* Description     : 
* Original Author : Chao Wang
* Created on      : July 9, 2020, 2:08 PM
*******************************************************************************/

#ifndef DRV_EXTEND_MCC_C
#define	DRV_EXTEND_MCC_C

//------------------------------------------------------------------------------
// Include
#include "xc.h"
#include "../../mcc_generated_files/spi1.h"

//------------------------------------------------------------------------------
// Constants and Macro Definitions

//------------------------------------------------------------------------------
// Public Variables

//------------------------------------------------------------------------------
// Private Enumerated and Structure Definitions

//------------------------------------------------------------------------------
// Private Function Prototypes (i.e. static)

//------------------------------------------------------------------------------
// Private Variables (i.e. static)

////////////////////////////////////////////////////////////////////////////////
// Public Function Implementation
/*******************************************************************************
* Public Function : SPITransfer
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
uint8_t SPITransfer(uint8_t txByte)
{
    static uint8_t dataTx, dataRx;
    
    dataTx = txByte;
    
    SPI1_Exchange8bitBuffer(&dataTx, 1, &dataRx);      

    return dataRx;
}

////////////////////////////////////////////////////////////////////////////////
// Private Function Implementation
/*******************************************************************************
* Private Function: TempFunc
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/

/******************************************************************************/
#undef DRV_EXTEND_MCC_C
#endif  /* DRV_EXTEND_MCC_C */
