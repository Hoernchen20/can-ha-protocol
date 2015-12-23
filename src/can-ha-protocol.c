﻿/*
 * can-ha-protocol.c
 *
 * Copyright 2015  Felix Horn
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


/* Includes ----------------------------------------------------------*/
#include "can-ha-protocol.h"

/* Private typedef ---------------------------------------------------*/
/* Private define ----------------------------------------------------*/
/** @defgroup CANHA_Data_Length
  * @{
  */
#define LENGTH_MEASURED_VALUE_16      2
#define LENGTH_MV_16                  2
#define LENGTH_MEASURED_VALUE_32      4
#define LENGTH_MV_32                  4
#define LENGTH_SET_POINT_COMMAND_16   2
#define LENGTH_SPC_16                 2 
#define LENGTH_SET_POINT_COMMAND_32   4
#define LENGTH_SPC_32                 4
#define LENGTH_SINGLE_INDICATION      1
#define LENGTH_SI                     1
#define LENGTH_DOUBLE_INDICATION      1
#define LENGTH_DI                     1
#define LENGTH_SINGLE_COMMAND         1
#define LENGTH_SC                     1
#define LENGTH_DOUBLE_COMMAND         1
#define LENGTH_DC                     1
#define LENGTH_CLOCK_SYNC             7
#define LENGTH_HEARTBEAT              1
/**
  * @}
  */
  
/* Private macro -----------------------------------------------------*/
/* Private variables -------------------------------------------------*/
CanHA_MsgTypeDef CAN_RxMsgBuf[CAN_BUFFER_SIZE];
volatile uint_fast8_t CAN_RxMsg_WrIndex = 0;
volatile uint_fast8_t CAN_RxMsg_RdIndex = 0;

CanHA_MsgTypeDef CAN_TxMsgBuf[CAN_BUFFER_SIZE];
volatile uint_fast8_t CAN_TxMsg_WrIndex = 0;
volatile uint_fast8_t CAN_TxMsg_RdIndex = 0;

volatile uint32_t UnixTimestamp = 1000000;

/* Private function prototypes ---------------------------------------*/
static bool isTimestampMatch(uint32_t Timestamp, uint32_t ArrayTimestamp);

/**********************************************************************/
/**                                                                  **/
/**                            Send Data                             **/
/**                                                                  **/
/**********************************************************************/

/**
  * @brief  Periodic send of heartbeat. Should be start every second.
  * @param  None
  * @retval None
  */
void CANHA_Heartbeat(void) {
    static uint_fast8_t Heartbeat_Cnt = 0;
    Heartbeat_Cnt++;
    if (Heartbeat_Cnt > HEARTBEAT_TIME - 1) {
        Heartbeat_Cnt = 0;
        uint_least8_t Status = true;
        CANHA_PutMsgToTxBuf(TYPE_HEARTBEAT, NODE_ID, LENGTH_HEARTBEAT, &Status);
    }
}

/**
  * @brief  This function change state of the single indications and send them.
  * @param  State: Can be TRUE or FALSE.
  * @retval None
  */
void CANHA_WriteSingleIndication(SingleIndication_TypeDef *Array, bool NewState) {
    if (Array->State != NewState) {
        Array->State = NewState;
        Array->Timestamp = UnixTimestamp - 1;
        CANHA_PutMsgToTxBuf(TYPE_SINGLE_INDICATION, Array->Identifier,
                LENGTH_SINGLE_INDICATION, (uint_least8_t*)Array->State);
    }
}

void CANHA_RefreshSingleIndication(SingleIndication_TypeDef *Array, uint_fast8_t Size) {
    uint32_t tmp_RTC_Counter = UnixTimestamp;
    for (uint_fast8_t i = 0; i < Size; i++) {
        if (isTimestampMatch(tmp_RTC_Counter, Array->Timestamp)) {
            CANHA_PutMsgToTxBuf(TYPE_SINGLE_INDICATION, Array->Identifier, LENGTH_SINGLE_INDICATION, (uint_least8_t*)Array->State);
        }
        Array++;
    }
}

void CANHA_WriteDoubleIndication(DoubleIndication_TypeDef *Array, uint_least8_t NewState) {
    if (Array->State != NewState) {
        Array->State = NewState;
        Array->Timestamp = UnixTimestamp - 1;
        CANHA_PutMsgToTxBuf(TYPE_DOUBLE_INDICATION, Array->Identifier,
                LENGTH_DOUBLE_INDICATION, &Array->State);
    }
}

void CANHA_RefreshDoubleIndication(DoubleIndication_TypeDef *Array, uint_fast8_t Size) {
    uint32_t tmp_RTC_Counter = UnixTimestamp;
    for (uint_fast8_t i = 0; i < Size; i++) {
        if (isTimestampMatch(tmp_RTC_Counter, Array->Timestamp)) {
            CANHA_PutMsgToTxBuf(TYPE_DOUBLE_INDICATION, Array->Identifier, LENGTH_DOUBLE_INDICATION, &Array->State);
        }
        Array++;
    }
}

/**
  * @brief  This function change value of the 16bit measured value and send them.
  * @param  New Value: Fix Point Value e.g. 21,53°C = 2153
  * @retval None
  */
void CANHA_WriteMeasuredValue16(MeasuredValue16_TypeDef *Array, int16_t NewValue) {
    if (Array->Value != NewValue) {
        Array->Value = NewValue;
        Array->Timestamp = UnixTimestamp - 1;

        uint_least8_t tmp[2];
        tmp[1] = (uint8_t)(Array->Value);
        tmp[0] = (uint8_t)(Array->Value>>8);
        CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_16, Array->Identifier, LENGTH_MEASURED_VALUE_16, tmp);
    }
}

void CANHA_RefreshMeasuredValue16(MeasuredValue16_TypeDef *Array, uint_fast8_t Size) {
    uint32_t tmp_RTC_Counter = UnixTimestamp;
    for (uint_fast8_t i = 0; i < Size; i++) {
        if (isTimestampMatch(tmp_RTC_Counter, Array->Timestamp)) {
            uint_least8_t tmp[2];
            tmp[1] = (uint8_t)(Array->Value);
            tmp[0] = (uint8_t)(Array->Value>>8);
            CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_16, Array->Identifier, LENGTH_MEASURED_VALUE_16, tmp);
        }
        Array++;
    }
}

/**
  * @brief  This function change value of the 32bit measured value and send them.
  * @param  New Value: Floating Point Value
  * @retval None
  */
void CANHA_WriteMeasuredValue32(MeasuredValue32_TypeDef *Array, int32_t NewValue) {
    if (Array->Value != NewValue) {
        Array->Value = NewValue;
        Array->Timestamp = UnixTimestamp - 1;

        uint_least8_t tmp[4];
        tmp[3] = (uint8_t)(Array->Value);
        tmp[2] = (uint8_t)(Array->Value>>8);
        tmp[1] = (uint8_t)(Array->Value>>16);
        tmp[0] = (uint8_t)(Array->Value>>24);
        CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_32, Array->Identifier, LENGTH_MEASURED_VALUE_32, tmp);
    }
}

void CANHA_RefreshMeasuredValue32(MeasuredValue32_TypeDef *Array, uint_fast8_t Size) {
    uint32_t tmp_RTC_Counter = UnixTimestamp;
    for (uint_fast8_t i = 0; i < Size; i++) {
        if (isTimestampMatch(tmp_RTC_Counter, Array->Timestamp)) {
            uint_least8_t tmp[4];
            tmp[3] = (uint8_t)(Array->Value);
            tmp[2] = (uint8_t)(Array->Value>>8);
            tmp[1] = (uint8_t)(Array->Value>>16);
            tmp[0] = (uint8_t)(Array->Value>>24);
            CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_32, Array->Identifier, LENGTH_MEASURED_VALUE_32, tmp);
        }
        Array++;
    }
}

static bool isTimestampMatch(uint32_t Timestamp, uint32_t ArrayTimestamp) {
    if ( (Timestamp - ArrayTimestamp) % (REFRESH_TIME + 1) == 0) {
        return true;
    } else {
        return false;
    }
}

/**********************************************************************/
/**                                                                  **/
/**                           Receive Data                           **/
/**                                                                  **/
/**********************************************************************/
/**
  * @brief  Periodic read of Data.
  * @param  None
  * @retval None
  */
void CANHA_ReadRefresh(void) {
    CanHA_MsgTypeDef Msg;
    while (CANHA_GetMsgFromRxBuf(&Msg)) {
        switch (Msg.MessageType) {
#if NUMBER_RECEIVE_SINGLE_INDICATION > 0
            case TYPE_SINGLE_INDICATION: {
                for (uint_fast8_t i = 0; i < NUMBER_RECEIVE_SINGLE_INDICATION; i++) {
                    if (Msg.Identifier == RX_Single_Indication[i].Identifier) {
                        RX_Single_Indication[i].Timestamp = UnixTimestamp;
                        RX_Single_Indication[i].State = Msg.Data[0];
                    }
                }
                break;
            }
#endif /* NUMBER_RECEIVE_SINGLE_INDICATION > 0 */

#if NUMBER_RECEIVE_DOUBLE_INDICATION > 0
            case TYPE_DOUBLE_INDICATION: {
                for (uint_fast8_t i = 0; i < NUMBER_RECEIVE_DOUBLE_INDICATION; i++) {
                    if (Msg.Identifier == RX_Double_Indication[i].Identifier) {
                        RX_Double_Indication[i].Timestamp = UnixTimestamp;
                        RX_Double_Indication[i].State = Msg.Data[0];
                    }
                }
                break;
            }
#endif /* NUMBER_RECEIVE_DOUBLE_INDICATION > 0 */
            
#if NUMBER_RECEIVE_MEASURED_VALUE_16 > 0
            case TYPE_MEASURED_VALUE_16: {
                
                for (uint_fast8_t i = 0; i < NUMBER_RECEIVE_MEASURED_VALUE_16; i++) {
                    if (Msg.Identifier == RX_Measured_Value_16[i].Identifier) {
                        RX_Measured_Value_16[i].Timestamp = UnixTimestamp;
                        RX_Measured_Value_16[i].Value = (int_least16_t)((Msg.Data[0] << 8) | Msg.Data[1]);
                    }
                }
                break;
            }
#endif /* NUMBER_RECEIVE_MEASURED_VALUE_16 > 0 */
            
#if NUMBER_RECEIVE_SETPOINT_16 > 0
            case TYPE_SETPOINT_COMMAND_16: {
                for (uint_fast8_t i = 0; i < NUMBER_RECEIVE_SETPOINT_16; i++) {
                    if (Msg.Identifier == RX_SetPoint16[i].Identifier) {
                        int16_t tmp_setpoint = (int16_t)((Msg.Data[0] << 8) | Msg.Data[1]);
                        RX_SetPoint16[i].Function (tmp_setpoint);
                    }
                }
                break;
            }
#endif /* NUMBER_RECEIVE_SETPOINT_16 > 0 */

#if NUMBER_RECEIVE_SETPOINT_32 > 0
            case TYPE_SETPOINT_COMMAND_32: {
                for (uint_fast8_t i = 0; i < NUMBER_RECEIVE_SETPOINT_32; i++) {
                    if (Msg.Identifier == RX_SetPoint32[i].Identifier) {
                        int32_t tmp_setpoint = ((Msg.Data[0] << 24) | (Msg.Data[1] << 16) | (Msg.Data[2] << 8) | Msg.Data[3]);
                        RX_SetPoint32[i].Function (tmp_setpoint);
                    }
                }
                break;
            }
#endif /* NUMBER_RECEIVE_SETPOINT_32 > 0 */
            default: break;
        }
    }
}

#if NUMBER_RECEIVE_SINGLE_INDICATION > 0
bool CANHA_ReadSingleIndication(uint_fast8_t ObjectNumber) {
    return RX_Single_Indication[ObjectNumber].State;
}
#endif /* NUMBER_RECEIVE_SINGLE_INDICATION > 0 */

bool CANHA_ReadMeasuredValue16(uint_fast8_t ObjectNumber, int_least16_t *Value) {
    if (RX_Measured_Value_16[ObjectNumber].Timestamp > 0) {
        *Value = RX_Measured_Value_16[ObjectNumber].Value;
        return true;
    } else {
        return false;
    }
}

/**********************************************************************/
/**                                                                  **/
/**                            Ringbuffer                            **/
/**                                                                  **/
/**********************************************************************/

/**
  * @brief  Copy message from transmit buffer if there is a message to send.
  * @param  GetMessage: Pointer of message
  * @retval FALSE = No message to send
  *         TRUE = New message is copied into struct of pointer
  */
bool CANHA_GetMsgFromTxBuf(CanHA_MsgTypeDef *GetMessage) {
    if (CAN_TxMsg_WrIndex != CAN_TxMsg_RdIndex) {
        GetMessage->MessageType = CAN_TxMsgBuf[CAN_TxMsg_RdIndex].MessageType;
        GetMessage->Identifier = CAN_TxMsgBuf[CAN_TxMsg_RdIndex].Identifier;
        GetMessage->DataLength = CAN_TxMsgBuf[CAN_TxMsg_RdIndex].DataLength;

        for (uint_fast8_t i = 0; i < CAN_TxMsgBuf[CAN_TxMsg_RdIndex].DataLength; i++) {
            GetMessage->Data[i] = CAN_TxMsgBuf[CAN_TxMsg_RdIndex].Data[i];
        }

        return true;
    } else {
        return false;
    }
}

/**
  * @brief  Increase read index of transmite buffer if message sent.
  * @param  None
  * @retval None
  */
void CANHA_MsgSent(void) {
    CAN_TxMsg_RdIndex++;
    CAN_TxMsg_RdIndex %= CAN_BUFFER_SIZE;
}

/**
  * @brief  Put new message in transmite buffer.
  * @param  MessageType: This parameter can be a value of @ref CANHA_Message_Types
  * @param  Identifier:
  * @param  DataLength: This parameter can be a value of @ref CANHA_Data_Length
  * @param  Data: Pointer to data array
  * @retval None
  */
void CANHA_PutMsgToTxBuf(uint_least16_t MessageType, uint_least32_t Identifier, uint_least8_t DataLength, uint_least8_t *Data) {
    CAN_TxMsgBuf[CAN_TxMsg_WrIndex].MessageType = MessageType;
    CAN_TxMsgBuf[CAN_TxMsg_WrIndex].Identifier = Identifier;
    CAN_TxMsgBuf[CAN_TxMsg_WrIndex].DataLength = DataLength;

    for (uint_fast8_t i = 0; i < DataLength; i++) {
        CAN_TxMsgBuf[CAN_TxMsg_WrIndex].Data[i] = *Data;
        Data++;
    }

    CAN_TxMsg_WrIndex++;
    CAN_TxMsg_WrIndex %= CAN_BUFFER_SIZE;
}

/**
  * @brief  Copy message from receive buffer if there is a new message.
  * @param  GetMessage: Pointer of messsage
  * @retval FALSE = No new message
  *         TRUE = New message is copied into struct of pointer
  */
bool CANHA_GetMsgFromRxBuf(CanHA_MsgTypeDef *GetMessage) {
    if (CAN_RxMsg_WrIndex != CAN_RxMsg_RdIndex) {
        GetMessage->MessageType = CAN_RxMsgBuf[CAN_RxMsg_RdIndex].MessageType;
        GetMessage->Identifier = CAN_RxMsgBuf[CAN_RxMsg_RdIndex].Identifier;
        GetMessage->DataLength = CAN_RxMsgBuf[CAN_RxMsg_RdIndex].DataLength;
        
        for (uint_fast8_t i = 0; i < CAN_RxMsgBuf[CAN_RxMsg_RdIndex].DataLength; i++) {
            GetMessage->Data[i] = CAN_RxMsgBuf[CAN_RxMsg_RdIndex].Data[i];
        }
        
        CAN_RxMsg_RdIndex++;
        CAN_RxMsg_RdIndex %= CAN_BUFFER_SIZE;

        return true;
    } else {
        return false;
    }
}

/**
  * @brief  Put new message in receive buffer.
  * @param  GetMessage: Pointer to new message
  * @retval None
  */
void CANHA_PutMsgToRxBuf(CanHA_MsgTypeDef *GetMessage) {
    /* Copy received message into buffer */
    CAN_RxMsgBuf[CAN_RxMsg_WrIndex].MessageType = GetMessage->MessageType;
    CAN_RxMsgBuf[CAN_RxMsg_WrIndex].Identifier = GetMessage->Identifier;
    CAN_RxMsgBuf[CAN_RxMsg_WrIndex].DataLength = GetMessage->DataLength;
    
    for (uint_fast8_t i = 0; i < GetMessage->DataLength; i++) {
        CAN_RxMsgBuf[CAN_RxMsg_WrIndex].Data[i] = GetMessage->Data[i];
    }
    
    /* Move buffer index to next entry */
    CAN_RxMsg_WrIndex++;
    CAN_RxMsg_WrIndex %= CAN_BUFFER_SIZE;
}
