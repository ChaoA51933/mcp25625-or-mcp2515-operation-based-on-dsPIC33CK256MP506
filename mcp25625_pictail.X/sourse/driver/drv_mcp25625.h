/*******************************************************************************
* File Name       : drv_mcp25625.h
* Description     : 
* Original Author : Chao Wang
* Created on      : July 29, 2020, 4:40 PM
*******************************************************************************/

#ifndef DRV_MCP25625_H
#define	DRV_MCP25625_H

#ifdef	__cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Conditional Definitions
#ifdef DRV_MCP25625_C
    #define DRV_MCP25625_PUBLIC
    #define DRV_MCP25625_CONST
#else
    #define DRV_MCP25625_PUBLIC extern
    #define DRV_MCP25625_CONST  const
#endif

//------------------------------------------------------------------------------    
// Include
#include "../../mcc_generated_files/pin_manager.h"
#include "../../mcc_generated_files/spi1.h"
    
//------------------------------------------------------------------------------
// Public Constants and Macro Definitions
/* Register addresses */
#define cREGADDR_CANCTRL	0x0F
#define cREGADDR_CANSTAT	0x0E

#define cREGADDR_CNF1   	0x2A
#define cREGADDR_CNF2   	0x29
#define cREGADDR_CNF3    	0x28

#define cREGADDR_TXB0CTRL	0x30
#define cREGADDR_TXB0ID 	0x31
#define cREGADDR_TXB0DATA 	0x36

#define cREGADDR_TXB1CTRL	0x40
#define cREGADDR_TXB1ID  	0x41
#define cREGADDR_TXB1DATA 	0x46

#define cREGADDR_TXB2CTRL	0x50
#define cREGADDR_TXB2ID 	0x51
#define cREGADDR_TXB2DATA 	0x56

#define cREGADDR_RXF0       0x00
#define cREGADDR_RXF1       0x04
#define cREGADDR_RXF2       0x08
#define cREGADDR_RXF3       0x10
#define cREGADDR_RXF4       0x14
#define cREGADDR_RXF5       0x18

#define cREGADDR_RXM0       0x20
#define cREGADDR_RXM1       0x24

#define cREGADDR_RXB0CTRL	0x60
#define cREGADDR_RXB0ID 	0x61
#define cREGADDR_RXB0DATA 	0x66

#define cREGADDR_RXB1CTRL	0x70
#define cREGADDR_RXB1ID 	0x71
#define cREGADDR_RXB1DATA 	0x76

#define cREGADDR_CANINTE	0x2B
#define cREGADDR_CANINTF	0x2C   
    
//------------------------------------------------------------------------------
// Public Enumerated and Structure Definitions
//! Number of TX Channels */
typedef enum {
    DRV_MCP25625_TX_CH0 = 0,
    DRV_MCP25625_TX_CH1,
    DRV_MCP25625_TX_CH2,
    DRV_MCP25625_TX_TOTAL_CHANNELS
} DRV_MCP25625_TX_CHANNEL;

//! Number of RX Channels */
typedef enum {
    DRV_MCP25625_RX_CH0 = 0,
    DRV_MCP25625_RX_CH1,
    DRV_MCP25625_RX_TOTAL_CHANNELS
} DRV_MCP25625_RX_CHANNEL;

//! Number of Filters
typedef enum {
    DRV_MCP25625_FILTER0 = 0,
    DRV_MCP25625_FILTER1,
    DRV_MCP25625_FILTER2,
    DRV_MCP25625_FILTER3,
    DRV_MCP25625_FILTER4,
    DRV_MCP25625_FILTER5,
    DRV_MCP25625_FILTER_TOTAL
} DRV_MCP25625_FILTER;

//! Number of Masks
typedef enum {
    DRV_MCP25625_MASK0 = 0,
    DRV_MCP25625_MASK1,
    DRV_MCP25625_MASK_TOTAL
} DRV_MCP25625_MASK;

//! Operation Modes */
typedef enum {
    DRV_MCP25625_NORMAL_MODE = 0x00,
    DRV_MCP25625_SLEEP_MODE = 0x01,
    DRV_MCP25625_LOOPBACK_MODE = 0x02,
    DRV_MCP25625_LISTEN_ONLY_MODE = 0x03,
    DRV_MCP25625_CONFIGURATION_MODE = 0x04,
    DRV_MCP25625_INVALID_MODE = 0xFF
} DRV_MCP25625_OPERATION_MODE;

//! Propagation Segment Time Quanta
typedef enum {
    DRV_MCP25625_PROP_1TQ = 0,
    DRV_MCP25625_PROP_2TQ,
    DRV_MCP25625_PROP_3TQ,
    DRV_MCP25625_PROP_4TQ,
    DRV_MCP25625_PROP_5TQ,
    DRV_MCP25625_PROP_6TQ,
    DRV_MCP25625_PROP_7TQ,
    DRV_MCP25625_PROP_8TQ
} DRV_MCP25625_PROPSEG_TQ;

//! Phase Segment1 Time Quanta
typedef enum {
    DRV_MCP25625_PHSEG1_1TQ = 0,
    DRV_MCP25625_PHSEG1_2TQ,
    DRV_MCP25625_PHSEG1_3TQ,
    DRV_MCP25625_PHSEG1_4TQ,
    DRV_MCP25625_PHSEG1_5TQ,
    DRV_MCP25625_PHSEG1_6TQ,
    DRV_MCP25625_PHSEG1_7TQ,
    DRV_MCP25625_PHSEG1_8TQ
} DRV_MCP25625_PHSEG1_TQ;

//! Phase Segment2 Time Quanta
typedef enum {
    DRV_MCP25625_PHSEG2_2TQ = 1,
    DRV_MCP25625_PHSEG2_3TQ,
    DRV_MCP25625_PHSEG2_4TQ,
    DRV_MCP25625_PHSEG2_5TQ,
    DRV_MCP25625_PHSEG2_6TQ,
    DRV_MCP25625_PHSEG2_7TQ,
    DRV_MCP25625_PHSEG2_8TQ
} DRV_MCP25625_PHSEG2_TQ;

//! Synchronization Jump Width
typedef enum {
    DRV_MCP25625_SJW_1TQ = 0,
    DRV_MCP25625_SJW_2TQ,
    DRV_MCP25625_SJW_3TQ,
    DRV_MCP25625_SJW_4TQ,
} DRV_MCP25625_SJW_TQ;

//! Sample Point Configuration
typedef enum {
    DRV_MCP25625_SAMPLE_ONCE = 0,
    DRV_MCP25625_SAMPLE_TRICE
} DRV_MCP25625_SAM_CONFIG;

//! Phase Seg2 Bit Time Length
typedef enum {
    DRV_MCP25625_PHSEG2_MIN = 0,
    DRV_MCP25625_PHSEG2_CNF3
} DRV_MCP25625_PHSEG2_CONFIG;

//! Start of Frame Output
typedef enum {
    DRV_MCP25625_CLKO_PIN = 0,
    DRV_MCP25625_SOF_PIN
} DRV_MCP25625_CLKO_SOF_CONFIG;

//! Wake Up Filter Enable
typedef enum {
    DRV_MCP25625_WAKFIL_DISABLE = 0,
    DRV_MCP25625_WAKFIL_ENABLE
} DRV_MCP25625_WAKFIL_CONFIG;

typedef struct _DRV_MCP25625_BIT_TIME_CONFIG {
    unsigned BRP : 6;
    unsigned PropSeg : 3;
    unsigned PhaseSeg1 : 3;
    unsigned PhaseSeg2 : 3;
    unsigned SJW : 2;
    unsigned BusSampling : 1;
    unsigned PhaseSeg2Config : 1;
    unsigned SOFPinConfig : 1;
    unsigned WakeUpFilterEnable : 1;
} DRV_MCP25625_BIT_TIME_CONFIG;

/* CAN Message Objects */
//! CAN Message Object ID
typedef struct _DRV_MCP25625_MSGOBJ_ID {
    unsigned SID:11;
    unsigned unused0:5;
    uint32_t EID:18;
    uint32_t unused1:14;
} DRV_MCP25625_MSGOBJ_ID;

//! CAN TX Message Object Control
typedef struct _DRV_MCP25625_TX_MSGOBJ_CTRL {
    unsigned DLC:4;
    unsigned IDE:1;
    unsigned RTR:1;
} DRV_MCP25625_TX_MSGOBJ_CTRL;

//! CAN TX Message Object
typedef struct _DRV_MCP25625_TX_MSGOBJ {
    DRV_MCP25625_MSGOBJ_ID id;
    DRV_MCP25625_TX_MSGOBJ_CTRL ctrl;
} DRV_MCP25625_TX_MSGOBJ;

//! CAN RX Message Object Control
typedef struct _DRV_MCP25625_RX_MSGOBJ_CTRL {
    unsigned DLC:4;
    unsigned IDE:1;
    unsigned RTR:1;
    unsigned SRR:1;
    unsigned RB0:1;
    unsigned RB1:1;
} DRV_MCP25625_RX_MSGOBJ_CTRL;

//! CAN RX Message Object
typedef struct _DRV_MCP25625_RX_MSGOBJ {
    DRV_MCP25625_MSGOBJ_ID id;
    DRV_MCP25625_RX_MSGOBJ_CTRL ctrl;
} DRV_MCP25625_RX_MSGOBJ;

//! Read Status
typedef union _DRV_MCP25625_REG_BUFFER_STATUS {
    struct {
        unsigned RX0IF : 1;
        unsigned RX1IF : 1;
        unsigned TX0REQ : 1;
        unsigned TX0IF : 1;
        unsigned TX1REQ : 1;
        unsigned TX1IF : 1;
        unsigned TX2REQ : 1;
        unsigned TX3IF : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_BUFFER_STATUS;

//! CAN Filter Object ID
typedef struct _DRV_MCP25625_FILTEROBJ_ID {
    unsigned SID:11;
    uint32_t EID:18;
    unsigned EXIDE:1;
} DRV_MCP25625_FILTEROBJ_ID;

//! CAN Mask Object ID
typedef struct _DRV_MCP25625_MASKOBJ_ID {
    unsigned MSID:11;
    uint32_t MEID:18;
} DRV_MCP25625_MASKOBJ_ID;

//! Receive Modes */
typedef enum {
    DRV_MCP25625_RXM_USE_FILTER = 0x0,
    DRV_MCP25625_RXM_RECEIVE_ALL = 0x3
} DRV_MCP25625_RXM_MODE;

//! Rollover to RXB1 if RXB0 is full
typedef enum {
    DRV_MCP25625_RXB_ROLLOVER_DISABLED = 0,
    DRV_MCP25625_RXB_ROLLOVER_ENABLED
} DRV_MCP25625_RXB_ROLLOVER;

//! Receive Mode configuration
typedef struct _DRV_MCP25625_RX_CONFIG {
    unsigned RXB0_ReceiveMode : 2;
    unsigned RXB1_ReceiveMode : 2;
    unsigned RXB_RollOver : 1;
} DRV_MCP25625_RX_CONFIG;

//! CAN Module Event (Interrupts); flags can be or'ed
typedef enum {
    DRV_MCP25625_NO_EVENT = 0,
    DRV_MCP25625_ALL_EVENTS = 0xFF,
    DRV_MCP25625_RXB0_EVENT = 0x01,
    DRV_MCP25625_RXB1_EVENT = 0x02,
    DRV_MCP25625_TXB0_EVENT = 0x04,
    DRV_MCP25625_TXB1_EVENT = 0x08,
    DRV_MCP25625_TXB2_EVENT = 0x10,
    DRV_MCP25625_ERROR_EVENT = 0x20,
    DRV_MCP25625_WAKEUP_EVENT = 0x40,
    DRV_MCP25625_MESSAGE_ERROR_EVENT = 0x80
} DRV_MCP25625_MODULE_EVENT;

/* Register bit fields */
//! CANCTRL
typedef union {
    struct {
        unsigned ClockOutPrescaler : 2;
        unsigned ClockOutEnable : 1;
        unsigned OneShotMode : 1;
        unsigned TxAbortAll : 1;
        unsigned OpModeRequest : 3;
    };
    unsigned char byte;
} DRV_MCP25625_REG_CANCTRL;

//! CANSTAT
typedef union {
    struct {
        unsigned Unimplemented0 : 1;
        unsigned InterruptCode : 3;
        unsigned Unimplemented1 : 1;
        unsigned OpMode : 3;
    };
    unsigned char byte;
} DRV_MCP25625_REG_CANSTAT;

//! CNF1
typedef union {
    struct {
        unsigned BRP : 6;
        unsigned SJW : 2;
    };
    unsigned char byte;
} DRV_MCP25625_REG_CNF1;

//! CNF2
typedef union {
    struct {
        unsigned PropSeg : 3;
        unsigned PhaseSeg1 : 3;
        unsigned SAM : 1;
        unsigned BTLMode : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_CNF2;

//! CNF3
typedef union {
    struct {
        unsigned PhaseSeg2 : 3;
        unsigned Unimplemented0 : 3;
        unsigned WakeUpFilterEnable : 1;
        unsigned SOF : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_CNF3;

//! TXBNCTRL
typedef union {
    struct {
        unsigned TxPriority : 2;
        unsigned Unimplemented0 : 1;
        unsigned TxRequest : 1;
        unsigned TxError : 1;
        unsigned ArbitrationLost : 1;
        unsigned MessageAborted : 1;
        unsigned Unimplemented1 : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_TXBNCTRL;

//! TXRTSCTRL
typedef union {
    struct {
        unsigned B0RTSM : 1;
        unsigned B1RTSM : 1;
        unsigned B2RTSM : 1;
        unsigned B0RTS : 1;
        unsigned B1RTS : 1;
        unsigned B2RTS : 1;
        unsigned Unimplemented0 : 2;
    };
    unsigned char byte;
} DRV_MCP25625_REG_TXRTSCTRL;

//! TXBnSIDL
typedef union {
    struct {
        unsigned EIDHH : 2;
        unsigned Unimplemented0 : 1;
        unsigned IDE : 1;
        unsigned Unimplemented1 : 1;
        unsigned SIDL : 3;
    };
    unsigned char byte;
} DRV_MCP25625_REG_TXBnSIDL;

//! TXBnDLC
typedef union {
    struct {
        unsigned DLC : 4;
        unsigned Unimplemented0 : 2;
        unsigned RTR : 1;
        unsigned Unimplemented1 : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_TXBnDLC;

//! RXBnSIDL
typedef union {
    struct {
        unsigned EIDHH : 2;
        unsigned Unimplemented0 : 1;
        unsigned IDE : 1;
        unsigned SRR : 1;
        unsigned SIDL : 3;
    };
    unsigned char byte;
} DRV_MCP25625_REG_RXBnSIDL;

//! RXBnDLC
typedef union {
    struct {
        unsigned DLC : 4;
        unsigned RB0 : 1;
        unsigned RB1 : 1;
        unsigned RTR : 1;
        unsigned Unimplemented1 : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_RXBnDLC;

//! RXB0CTRL
typedef union {
    struct {
        unsigned FILTHIT : 1;
        unsigned Unimplemented1 : 1;
        unsigned BUKT : 1;
        unsigned RXRTR : 1;
        unsigned Unimplemented2 : 1;
        unsigned RXM :2;
        unsigned Unimplemented3 : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_RXB0CTRL;

///! RXB1CTRL
typedef union {
    struct {
        unsigned FILTHIT : 3;
        unsigned RXRTR : 1;
        unsigned Unimplemented1 : 1;
        unsigned RXM :2;
        unsigned Unimplemented3 : 1;
    };
    unsigned char byte;
} DRV_MCP25625_REG_RXB1CTRL;

//------------------------------------------------------------------------------
// Public Variables

//------------------------------------------------------------------------------
// Public Function Prototypes
DRV_MCP25625_PUBLIC void Drv_MCP25625_Initialize(void);
DRV_MCP25625_PUBLIC DRV_MCP25625_MODULE_EVENT Drv_MCP25625_ModuleEventGet();
DRV_MCP25625_PUBLIC DRV_MCP25625_REG_BUFFER_STATUS Drv_MCP25625_BufferStatusGet();
DRV_MCP25625_PUBLIC void Drv_MCP25625_ModuleEventClear(DRV_MCP25625_MODULE_EVENT flags);

DRV_MCP25625_PUBLIC void Drv_MCP25625_ReceiveMessageGet(DRV_MCP25625_RX_CHANNEL channel, DRV_MCP25625_RX_MSGOBJ* rxObj, uint8_t *rxd, uint8_t nBytes);
DRV_MCP25625_PUBLIC void Drv_MCP25625_TransmitChannelLoad(DRV_MCP25625_TX_CHANNEL channel, DRV_MCP25625_TX_MSGOBJ* txObj, const uint8_t *txd, uint8_t nBytes);
DRV_MCP25625_PUBLIC void Drv_MCP25625_TransmitChannelFlush(DRV_MCP25625_TX_CHANNEL channel);    

/******************************************************************************/    
#ifdef	__cplusplus
}
#endif

#endif	/* DRV_MCP25625_H */
