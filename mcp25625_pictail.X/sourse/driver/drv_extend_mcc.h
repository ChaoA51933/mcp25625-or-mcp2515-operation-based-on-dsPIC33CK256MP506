/*******************************************************************************
* File Name       : drv_extend_mcc.h
* Description     : 
* Original Author : Chao Wang
* Created on      : July 9, 2020, 2:08 PM
*******************************************************************************/

#ifndef DRV_EXTEND_MCC_H
#define	DRV_EXTEND_MCC_H

#ifdef	__cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Conditional Definitions
#ifdef DRV_EXTEND_MCC_C
    #define DRV_EXTEND_MCC_PUBLIC
    #define DRV_EXTEND_MCC_CONST
#else
    #define DRV_EXTEND_MCC_PUBLIC extern
    #define DRV_EXTEND_MCC_CONST  const
#endif

//------------------------------------------------------------------------------    
// Include
#include <stdint.h>
#include <stdbool.h>
#include "../../mcc_generated_files/clock.h"
    
//------------------------------------------------------------------------------
// Public Constants and Macro Definitions
#define FCY  (_XTAL_FREQ/2)

//------------------------------------------------------------------------------
// Public Enumerated and Structure Definitions

//------------------------------------------------------------------------------
// Public Variables

//------------------------------------------------------------------------------
// Public Function Prototypes
DRV_EXTEND_MCC_PUBLIC uint8_t SPITransfer(uint8_t txByte);   

/******************************************************************************/    
#ifdef	__cplusplus
}
#endif

#endif	/* DRV_EXTEND_MCC_H */
