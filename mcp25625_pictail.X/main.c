/*******************************************************************************
* File Name       : main.c
* Description     : 
* Original Author : Chao Wang
* Created on      : July 29, 2020, 5:06 PM
*******************************************************************************/

#ifndef MAIN_C
#define	MAIN_C

//------------------------------------------------------------------------------
// Include
#include "mcc_generated_files/system.h"
#include "./sourse/driver/drv_extend_mcc.h"
#include "./sourse/misc/delay.h"
#include "./sourse/driver/drv_mcp25625.h"

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
DRV_MCP25625_RX_MSGOBJ rxObj;
DRV_MCP25625_TX_MSGOBJ txObj;
DRV_MCP25625_REG_BUFFER_STATUS canStatus;
DRV_MCP25625_MODULE_EVENT events;
uint16_t i = 0;
////////////////////////////////////////////////////////////////////////////////
// Public Function Implementation
/*******************************************************************************
* Public Function : main
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    Drv_MCP25625_Initialize();
    
    /**************************************************************************/
    // CAN data buffers
    unsigned char txd[8];
    unsigned char rxd[8];

    for (i=0; i<8; i++) {
        txd[i] = 0;
        rxd[i] = 0;
    }

    /* CAN Messages */
    // Button status
    
    txObj.id.SID = 0x100;
    txObj.id.EID = 0;
    txObj.ctrl.DLC = 1;
    txObj.ctrl.IDE = 0;
    txObj.ctrl.RTR = 0;
    /**************************************************************************/    
    
    while (1)
    {
        // Add your application code              
        __delay_ms(10);
        /**********************************************************************/
        // Receive message
        // Check for received message, and update LEDs
        events = Drv_MCP25625_ModuleEventGet();
        Nop(); Nop();  
        
        // Check RXB1
        if (events & DRV_MCP25625_RXB1_EVENT) {
            // Get message
            Drv_MCP25625_ReceiveMessageGet(DRV_MCP25625_RX_CH1, &rxObj, rxd, 1);
            Nop(); Nop(); Nop();

            // Clear flag
            Drv_MCP25625_ModuleEventClear(DRV_MCP25625_RXB1_EVENT);
            
            txd[0] = rxd[0];

            // Wait till transmit complete
            do {
                canStatus = Drv_MCP25625_BufferStatusGet();
                Nop(); Nop();
            } while (canStatus.TX0REQ);
            Nop(); Nop(); Nop();

            Drv_MCP25625_TransmitChannelLoad(DRV_MCP25625_TX_CH0, &txObj, txd, txObj.ctrl.DLC);
            Nop(); Nop();

            // Request to send
            Drv_MCP25625_TransmitChannelFlush(DRV_MCP25625_TX_CH0);
            Nop(); Nop();   
          
        }
        Nop(); Nop();        
        // Check RXB0
        if (events & DRV_MCP25625_RXB0_EVENT) {
            // Get message
            Drv_MCP25625_ReceiveMessageGet(DRV_MCP25625_RX_CH0, &rxObj, rxd, 1);
            Nop(); Nop(); Nop();
            
            // Clear flag
            Drv_MCP25625_ModuleEventClear(DRV_MCP25625_RXB0_EVENT);
            
            txd[0] = rxd[0];

            // Wait till transmit complete
            do {
                canStatus = Drv_MCP25625_BufferStatusGet();
                Nop(); Nop();
            } while (canStatus.TX0REQ);
            Nop(); Nop(); Nop();

            Drv_MCP25625_TransmitChannelLoad(DRV_MCP25625_TX_CH0, &txObj, txd, txObj.ctrl.DLC);
            Nop(); Nop();

            // Request to send
            Drv_MCP25625_TransmitChannelFlush(DRV_MCP25625_TX_CH0);
            Nop(); Nop();    
            
        }
        Nop(); Nop();    
    }
    
    return 1;     
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
#undef MAIN_C
#endif  /* MAIN_C */
