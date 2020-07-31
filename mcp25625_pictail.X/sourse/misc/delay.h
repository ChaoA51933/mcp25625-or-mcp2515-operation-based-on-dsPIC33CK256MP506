/*******************************************************************************
* File Name       : delay.h
* Description     : 
* Original Author : Chao Wang
* Created on      : July 14, 2020, 2:42 PM
*******************************************************************************/

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Conditional Definitions
#ifdef DELAY_C
    #define DELAY_PUBLIC
    #define DELAY_CONST
#else
    #define DELAY_PUBLIC extern
    #define DELAY_CONST  const
#endif

//------------------------------------------------------------------------------
// Macro Definitions shoud be before include file.
   
//------------------------------------------------------------------------------    
// Include
#include "../driver/drv_extend_mcc.h"
#include <libpic30.h>    
//------------------------------------------------------------------------------
// Public Constants and Macro Definitions   
    
//------------------------------------------------------------------------------
// Public Enumerated and Structure Definitions

//------------------------------------------------------------------------------
// Public Variables

//------------------------------------------------------------------------------
// Public Function Prototypes
    

/******************************************************************************/    
#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */
