/*******************************************************************************
* File Name       : drv_mcp25625.c
* Description     : 
* Original Author : Chao Wang
* Created on      : July 29, 2020, 4:40 PM
*******************************************************************************/

#ifndef DRV_MCP25625_C
#define	DRV_MCP25625_C

//------------------------------------------------------------------------------
// Include
#include "drv_mcp25625.h"
#include "drv_extend_mcc.h"

//------------------------------------------------------------------------------
// Constants and Macro Definitions
//-------------MCP25625 SPI commands------------------------
#define CAN_RESET       0xC0  //Reset
#define CAN_READ        0x03  //Read
#define CAN_WRITE       0x02  //Write
#define CAN_RTS         0x80  //Request to Send
#define CAN_RTS_TXB0    0x81  //RTS TX buffer 0
#define CAN_RTS_TXB1    0x82  //RTS TX buffer 1
#define CAN_RTS_TXB2    0x84  //RTS TX buffer 2
#define CAN_RD_STATUS   0xA0  //Read Status
#define CAN_BIT_MODIFY  0x05  //Bit modify
#define CAN_RX_STATUS   0xB0  //Receive status

#define MSG_ID_0X1XX    0x100
#define MSG_ID_0X200    0x200

//------------------------------------------------------------------------------
// Public Variables

//------------------------------------------------------------------------------
// Private Enumerated and Structure Definitions

//------------------------------------------------------------------------------
// Private Function Prototypes (i.e. static)
//void IOInitFunc(char *gpio, char *dir_path, char *dir);
//void IOExitFunc(char *gpio);

//------------------------------------------------------------------------------
// Private Variables (i.e. static)
void Drv_MCP25625_Reset();

void Drv_MCP25625_RegisterWrite(uint8_t address, uint8_t data);
void Drv_MCP25625_BufferWrite(uint8_t address, uint8_t *data, uint8_t nBytes);
void Drv_MCP25625_RegisterBitModify(uint8_t address, uint8_t mask, uint8_t data);
uint8_t Drv_MCP25625_RegisterRead(uint8_t address);
void Drv_MCP25625_BufferRead(uint8_t address, uint8_t *data, uint8_t nBytes);

void Drv_MCP25625_ConfigurationSet(DRV_MCP25625_BIT_TIME_CONFIG *config);
void Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER filter, DRV_MCP25625_FILTEROBJ_ID* fObj);
void Drv_MCP25625_MaskConfigure(DRV_MCP25625_MASK mask, DRV_MCP25625_MASKOBJ_ID* mObj);
void Drv_MCP25625_ReceiveConfigurationSet(DRV_MCP25625_RX_CONFIG* config);
void Drv_MCP25625_ModuleEventEnable(DRV_MCP25625_MODULE_EVENT flags);
void Drv_MCP25625_ModuleEventDisable(DRV_MCP25625_MODULE_EVENT flags);
void Drv_MCP25625_OperationModeSelect(DRV_MCP25625_OPERATION_MODE opMode);
DRV_MCP25625_OPERATION_MODE Drv_MCP25625_OperationModeGet();

////////////////////////////////////////////////////////////////////////////////
// Public Function Implementation
/*******************************************************************************
* Public Function : Drv_MCP25625_Initialize
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
void Drv_MCP25625_Initialize(void)
{
    CAN_CS_SetHigh();
    
    /* Reset MCP25625; this makes sure device is in Configuration Mode */
    Drv_MCP25625_Reset();
    
    /* Configure Bit time; 20 MHz crystal; 500kbps */
    Nop(); Nop();
    DRV_MCP25625_BIT_TIME_CONFIG btConfig;

    btConfig.BRP = 0;
    btConfig.PropSeg = DRV_MCP25625_PROP_7TQ;
    btConfig.PhaseSeg1 = DRV_MCP25625_PHSEG1_8TQ;
    btConfig.PhaseSeg2 = DRV_MCP25625_PHSEG2_4TQ;
    btConfig.SJW = DRV_MCP25625_SJW_4TQ;

    btConfig.BusSampling = DRV_MCP25625_SAMPLE_TRICE;
    btConfig.PhaseSeg2Config = DRV_MCP25625_PHSEG2_CNF3;
    btConfig.SOFPinConfig = DRV_MCP25625_CLKO_PIN;
    btConfig.WakeUpFilterEnable = DRV_MCP25625_WAKFIL_ENABLE;

    Drv_MCP25625_ConfigurationSet(&btConfig);

    /* Configure Filter objects */
    DRV_MCP25625_FILTEROBJ_ID fObj;
    /* Allow CAN Base Frame with ID 0x1xx */
    fObj.SID = MSG_ID_0X1XX;
    fObj.EID = 0x0;
    fObj.EXIDE = 0;

    Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER0, &fObj);
    Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER1, &fObj);
    Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER2, &fObj);
    Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER3, &fObj);
    Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER4, &fObj);
    Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER5, &fObj);

    /* Configure mask objects */
    DRV_MCP25625_MASKOBJ_ID mObj;
    mObj.MSID = 0x3FF;
    mObj.MEID = 0x0;
    Drv_MCP25625_MaskConfigure(DRV_MCP25625_MASK0, &mObj);
    Drv_MCP25625_MaskConfigure(DRV_MCP25625_MASK1, &mObj);
    
    /* Configure receive mode */
    DRV_MCP25625_RX_CONFIG rxConfig;
    rxConfig.RXB0_ReceiveMode = DRV_MCP25625_RXM_USE_FILTER;
    rxConfig.RXB1_ReceiveMode = DRV_MCP25625_RXM_USE_FILTER;
    rxConfig.RXB_RollOver = DRV_MCP25625_RXB_ROLLOVER_ENABLED;
    Drv_MCP25625_ReceiveConfigurationSet(&rxConfig);

    /* Clear all flags */
    Drv_MCP25625_ModuleEventClear(DRV_MCP25625_ALL_EVENTS);

    /* Enable receive interrupt */
    Drv_MCP25625_ModuleEventEnable(DRV_MCP25625_RXB0_EVENT);

    /* Set Normal/Loopback mode */
    Drv_MCP25625_OperationModeSelect(DRV_MCP25625_NORMAL_MODE);        
}

/*******************************************************************************
* Public Function : Drv_MCP25625_ModuleEventGet
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
DRV_MCP25625_MODULE_EVENT Drv_MCP25625_ModuleEventGet()
{
    // Read Interrupt register
    DRV_MCP25625_MODULE_EVENT flags;
    flags = Drv_MCP25625_RegisterRead(cREGADDR_CANINTF);

    return flags;
}

/*******************************************************************************
* Public Function : Drv_MCP25625_BufferStatusGet
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
DRV_MCP25625_REG_BUFFER_STATUS Drv_MCP25625_BufferStatusGet()
{
    DRV_MCP25625_REG_BUFFER_STATUS status;

    CAN_CS_SetLow();
    SPITransfer(CAN_RD_STATUS);
    status.byte = SPITransfer(0);
    SPITransfer(0);
    CAN_CS_SetHigh();

    return status;
}

/*******************************************************************************
* Public Function : Drv_MCP25625_ReceiveMessageGet
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
void Drv_MCP25625_ReceiveMessageGet(DRV_MCP25625_RX_CHANNEL channel, DRV_MCP25625_RX_MSGOBJ* rxObj, uint8_t *rxd, uint8_t nBytes)
{
#define RX_BUFFER_SIZE 13

    uint8_t d[RX_BUFFER_SIZE];
    uint8_t n=0;

    // Get address
    uint8_t address;
    switch(channel) {
        case DRV_MCP25625_RX_CH0: address = cREGADDR_RXB0ID; break;
        case DRV_MCP25625_RX_CH1: address = cREGADDR_RXB1ID; break;
        default: address = cREGADDR_RXB0ID; break;
    }

    // Read buffer
    n = nBytes + 5;
    if (n>RX_BUFFER_SIZE) {
        n = RX_BUFFER_SIZE;

    }
    Drv_MCP25625_BufferRead(address, d, n);
    Nop(); Nop();

    
    // SID
    DRV_MCP25625_REG_RXBnSIDL sidl;
    sidl.byte = d[1];
    rxObj->id.SID = sidl.SIDL;
    rxObj->id.SID += (unsigned int)d[0] <<3;

    // EID
    rxObj->id.EID = d[3];
    rxObj->id.EID += (uint32_t)d[2] <<8;
    rxObj->id.EID += (uint32_t)sidl.EIDHH <<16;

    // Control
    DRV_MCP25625_REG_RXBnDLC dlc;
    dlc.byte = d[4];
    rxObj->ctrl.IDE = sidl.IDE;
    rxObj->ctrl.SRR = sidl.SRR;
    rxObj->ctrl.DLC = dlc.DLC;
    rxObj->ctrl.RTR = dlc.RTR;
    rxObj->ctrl.RB0 = dlc.RB0;
    rxObj->ctrl.RB1 = dlc.RB1;

    // Data
    uint16_t i;
    for (i=0; i<(n-5); i++) {
        rxd[i] = d[i+5];
    }
    Nop(); Nop();

    return;
}

/*******************************************************************************
* Public Function : Drv_MCP25625_TransmitChannelLoad
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
void Drv_MCP25625_TransmitChannelLoad(DRV_MCP25625_TX_CHANNEL channel, DRV_MCP25625_TX_MSGOBJ* txObj, const uint8_t *txd, uint8_t nBytes)
{
#define TX_BUFFER_SIZE 13

    uint8_t d[TX_BUFFER_SIZE];
    uint32_t reg = 0;
    uint8_t n=0;

    // TXBnSIDH
    Nop(); Nop();
    reg = txObj->id.SID>>3;
    d[n] = reg & 0xff;
    n++;

    // TXBnSIDL
    DRV_MCP25625_REG_TXBnSIDL sidl;
    sidl.byte = 0;
    sidl.SIDL = txObj->id.SID & 0x7;
    sidl.IDE = txObj->ctrl.IDE;
    reg = (txObj->id.EID>>16);
    sidl.EIDHH = reg & 0x3;
    d[n] = sidl.byte;
    n++;

    // TXBnEID8
    reg = (txObj->id.EID>>8);
    d[n] = reg & 0xff;
    n++;

    // TXBnEID0
    d[n] = txObj->id.EID & 0xff;
    n++;

    // TXBnDLC
    DRV_MCP25625_REG_TXBnDLC dlc;
    dlc.byte = 0;
    dlc.DLC = txObj->ctrl.DLC;
    dlc.RTR = txObj->ctrl.RTR;
    d[n] = dlc.byte;
    n++;

    // Data Bytes
    uint8_t i=0;
    unsigned max = nBytes;
    if (max>8) {
        max = 8;
    }
    for(i=0; i<max; i++) {
        d[n] = txd[i];
        n++;
    }

    // Copy data
    uint8_t address;
    switch(channel) {
        case DRV_MCP25625_TX_CH0: address = cREGADDR_TXB0ID; break;
        case DRV_MCP25625_TX_CH1: address = cREGADDR_TXB1ID; break;
        case DRV_MCP25625_TX_CH2: address = cREGADDR_TXB2ID; break;
        default: address = cREGADDR_TXB0ID; break;
    }

    // Write buffer
    Drv_MCP25625_BufferWrite(address, d, n);
}

/*******************************************************************************
* Public Function : Drv_MCP25625_TransmitChannelFlush
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  : 
*******************************************************************************/
void Drv_MCP25625_TransmitChannelFlush(DRV_MCP25625_TX_CHANNEL channel)
{
    uint8_t cmd = 0;

    // Get command
    Nop(); Nop();
    
    switch(channel) {
        case DRV_MCP25625_TX_CH0: cmd = CAN_RTS_TXB0; break;
        case DRV_MCP25625_TX_CH1: cmd = CAN_RTS_TXB1; break;
        case DRV_MCP25625_TX_CH2: cmd = CAN_RTS_TXB2; break;
        default: cmd = CAN_RTS_TXB0; break;
    }

    CAN_CS_SetLow();
    SPITransfer(cmd);
    CAN_CS_SetHigh();
    Nop();
    Nop();
}

/*******************************************************************************
* Public Function : Drv_MCP25625_ModuleEventClear
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_ModuleEventClear(DRV_MCP25625_MODULE_EVENT flags)
{
    // Write Interrupt Flag Register
    Drv_MCP25625_RegisterBitModify(cREGADDR_CANINTF, flags, 0x00);
}


////////////////////////////////////////////////////////////////////////////////
// Private Function Implementation
/*******************************************************************************
* Private Function: Drv_MCP25625_Reset
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_Reset()
{
    
    CAN_CS_SetLow();
    SPITransfer(CAN_RESET);
    CAN_CS_SetHigh();
    
    long i = 1000;
    while (i--);
}

/*******************************************************************************
* Private Function: Drv_MCP25625_ConfigurationSet
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_ConfigurationSet(DRV_MCP25625_BIT_TIME_CONFIG *config)
{
    DRV_MCP25625_REG_CNF1 cnf1;
    DRV_MCP25625_REG_CNF2 cnf2;
    DRV_MCP25625_REG_CNF3 cnf3;

    cnf1.BRP = config->BRP;
    cnf1.SJW = config->SJW;

    cnf2.PropSeg = config->PropSeg;
    cnf2.PhaseSeg1 = config->PhaseSeg1;
    cnf2.SAM = config->BusSampling;
    cnf2.BTLMode = config->PhaseSeg2Config;

    cnf3.PhaseSeg2 = config->PhaseSeg2;
    cnf3.WakeUpFilterEnable = config->WakeUpFilterEnable;
    cnf3.SOF = config->SOFPinConfig;

    Drv_MCP25625_RegisterWrite(cREGADDR_CNF1, cnf1.byte);
    Drv_MCP25625_RegisterWrite(cREGADDR_CNF2, cnf2.byte);
    Drv_MCP25625_RegisterWrite(cREGADDR_CNF3, cnf3.byte);
}

/*******************************************************************************
* Private Function: Drv_MCP25625_FilterConfigure
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_FilterConfigure(DRV_MCP25625_FILTER filter, DRV_MCP25625_FILTEROBJ_ID* fObj)
{
#define FILTER_SIZE 4

    uint8_t d[FILTER_SIZE];
    uint32_t reg = 0;
    uint8_t n=0;

    // RXFnSIDH
    Nop(); Nop();
    reg = fObj->SID>>3;
    d[n] = reg & 0xff;
    n++;

    // RXFnSIDL
    DRV_MCP25625_REG_TXBnSIDL sidl;
    sidl.byte = 0;
    sidl.SIDL = fObj->SID & 0x7;
    sidl.IDE = fObj->EXIDE;
    reg = (fObj->EID>>16);
    sidl.EIDHH = reg & 0x3;
    d[n] = sidl.byte;
    n++;

    // RXFnEID8
    reg = (fObj->EID>>8);
    d[n] = reg & 0xff;
    n++;

    // RXFnEID0
    d[n] = fObj->EID & 0xff;
    n++;

        // Copy data
    uint8_t address;
    switch(filter) {
        case DRV_MCP25625_FILTER0: address = cREGADDR_RXF0; break;
        case DRV_MCP25625_FILTER1: address = cREGADDR_RXF1; break;
        case DRV_MCP25625_FILTER2: address = cREGADDR_RXF2; break;
        case DRV_MCP25625_FILTER3: address = cREGADDR_RXF3; break;
        case DRV_MCP25625_FILTER4: address = cREGADDR_RXF4; break;
        case DRV_MCP25625_FILTER5: address = cREGADDR_RXF5; break;
        default: address = cREGADDR_RXF0; break;
    }

    // Write buffer
    Drv_MCP25625_BufferWrite(address, d, FILTER_SIZE);
}


/*******************************************************************************
* Private Function: Drv_MCP25625_MaskConfigure
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_MaskConfigure(DRV_MCP25625_MASK mask, DRV_MCP25625_MASKOBJ_ID* mObj)
{
#define MASK_SIZE 4

    uint8_t d[MASK_SIZE];
    uint32_t reg = 0;
    uint8_t n=0;

    // RXMnSIDH
    Nop(); Nop();
    reg = mObj->MSID>>3;
    d[n] = reg & 0xff;
    n++;

    // RXMnSIDL
    DRV_MCP25625_REG_TXBnSIDL sidl;
    sidl.byte = 0;
    sidl.SIDL = mObj->MSID & 0x7;
    sidl.IDE = 0; // Not implemented
    reg = (mObj->MEID>>16);
    sidl.EIDHH = reg & 0x3;
    d[n] = sidl.byte;
    n++;

    // RXMnEID8
    reg = (mObj->MEID>>8);
    d[n] = reg & 0xff;
    n++;

    // RXMnEID0
    d[n] = mObj->MEID & 0xff;
    n++;

        // Copy data
    uint8_t address;
    switch(mask) {
        case DRV_MCP25625_MASK0: address = cREGADDR_RXM0; break;
        case DRV_MCP25625_MASK1: address = cREGADDR_RXM1; break;
        default: address = cREGADDR_RXM0; break;
    }

    // Write buffer
    Drv_MCP25625_BufferWrite(address, d, MASK_SIZE);
}

/*******************************************************************************
* Private Function: Drv_MCP25625_ReceiveConfigurationSet
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_ReceiveConfigurationSet(DRV_MCP25625_RX_CONFIG* config)
{
    // Configure RB0
    DRV_MCP25625_REG_RXB0CTRL rxb0Ctrl;
    rxb0Ctrl.byte = 0;
    rxb0Ctrl.RXM = config->RXB0_ReceiveMode;
    rxb0Ctrl.BUKT = config->RXB_RollOver;
    Drv_MCP25625_RegisterWrite(cREGADDR_RXB0CTRL, rxb0Ctrl.byte);

    // Configure RB1
    DRV_MCP25625_REG_RXB1CTRL rxb1Ctrl;
    rxb1Ctrl.byte = 0;
    rxb1Ctrl.RXM = config->RXB1_ReceiveMode;
    Drv_MCP25625_RegisterWrite(cREGADDR_RXB1CTRL, rxb1Ctrl.byte);
}

/*******************************************************************************
* Private Function: Drv_MCP25625_ModuleEventEnable
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_ModuleEventEnable(DRV_MCP25625_MODULE_EVENT flags)
{
    // Write Interrupt Enable Register
    Drv_MCP25625_RegisterBitModify(cREGADDR_CANINTE, flags, 0xFF);
}

/*******************************************************************************
* Private Function: Drv_MCP25625_ModuleEventDisable
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_ModuleEventDisable(DRV_MCP25625_MODULE_EVENT flags)
{
    // Write Interrupt Enable Register
    Drv_MCP25625_RegisterBitModify(cREGADDR_CANINTE, flags, 0x00);
}

/*******************************************************************************
* Private Function: Drv_MCP25625_OperationModeSelect
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_OperationModeSelect(DRV_MCP25625_OPERATION_MODE opMode)
{
    DRV_MCP25625_REG_CANCTRL canCtrl;

    // Read
    Nop();
    canCtrl.byte = Drv_MCP25625_RegisterRead(cREGADDR_CANCTRL);


    //Modify
    canCtrl.OpModeRequest = opMode;

    // Write
    Drv_MCP25625_RegisterWrite(cREGADDR_CANCTRL, canCtrl.byte);

    // Wait till mode has changed
    DRV_MCP25625_REG_CANSTAT canStat;
    canStat.byte = Drv_MCP25625_RegisterRead(cREGADDR_CANSTAT);

    while (canStat.OpMode != canCtrl.OpModeRequest) {
        canStat.byte = Drv_MCP25625_RegisterRead(cREGADDR_CANSTAT);
        Nop();
        Nop();
    }
}

/*******************************************************************************
* Private Function: Drv_MCP25625_OperationModeGet
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
DRV_MCP25625_OPERATION_MODE Drv_MCP25625_OperationModeGet()
{
    DRV_MCP25625_REG_CANSTAT canStat;

    Nop();
    canStat.byte = Drv_MCP25625_RegisterRead(cREGADDR_CANSTAT);

    return canStat.OpMode;
}

/*******************************************************************************
* Private Function: Drv_MCP25625_RegisterWrite
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_RegisterWrite(uint8_t address, uint8_t data)
{
    CAN_CS_SetLow();
    SPITransfer(CAN_WRITE);
    SPITransfer(address);
    SPITransfer(data);
    CAN_CS_SetHigh();
}

/*******************************************************************************
* Private Function: Drv_MCP25625_BufferWrite
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_BufferWrite(uint8_t address, uint8_t *data, uint8_t nBytes)
{
    uint8_t n, d;

    CAN_CS_SetLow();
    SPITransfer(CAN_WRITE);
    SPITransfer(address);
    for (n=0; n<nBytes; n++) {
        d = data[n];
        Nop();
        Nop();
        SPITransfer(d);
    }
    CAN_CS_SetHigh();
}

/*******************************************************************************
* Private Function: Drv_MCP25625_RegisterBitModify
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_RegisterBitModify(uint8_t address, uint8_t mask, uint8_t data)
{
    CAN_CS_SetLow();
    SPITransfer(CAN_BIT_MODIFY);
    SPITransfer(address);
    SPITransfer(mask);
    SPITransfer(data);
    CAN_CS_SetHigh();
}

/*******************************************************************************
* Private Function: Drv_MCP25625_RegisterRead
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
uint8_t Drv_MCP25625_RegisterRead(uint8_t address)
{
    uint8_t d;

    CAN_CS_SetLow();
    SPITransfer(CAN_READ);
    SPITransfer(address);
    d = SPITransfer(0);
    CAN_CS_SetHigh();

    return d;
}

/*******************************************************************************
* Private Function: Drv_MCP25625_BufferRead
* Description     : 
* Purpose         :
* Input           : 
* Return          : 
* Calls           :    
* Called by       :
* Note			  :
*******************************************************************************/
void Drv_MCP25625_BufferRead(uint8_t address, uint8_t *data, uint8_t nBytes)
{
    uint8_t n, d;

    CAN_CS_SetLow();
    SPITransfer(CAN_READ);
    SPITransfer(address);
    for (n=0; n<nBytes; n++) {
        d = SPITransfer(0);
        Nop();
        Nop();
        data[n] = d;
    }
    CAN_CS_SetHigh();
}

/******************************************************************************/
#undef DRV_MCP25625_C
#endif  /* DRV_MCP25625_C */
