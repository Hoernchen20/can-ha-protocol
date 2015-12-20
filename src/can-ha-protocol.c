/*
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


/* Includes ------------------------------------------------------------------*/
#include "can-ha-protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup CANHA_Message_Types
  * @{
  */
#define TYPE_MEASURED_VALUE_16       2030    //0b0111 1110 1110, 0x7EE
#define TYPE_MV_16                   2030    //0b0111 1110 1110, 0x7EE
#define TYPE_MEASURED_VALUE_32       2020    //0b0111 1110 0100, 0x7E4
#define TYPE_MV_32                   2020    //0b0111 1110 0100, 0x7E4
#define TYPE_SETPOINT_COMMAND_16     2010    //0b0111 1101 1010, 0x7DA
#define TYPE_SPC_16                  2010    //0b0111 1101 1010, 0x7DA
#define TYPE_SETPOINT_COMMAND_32     2000    //0b0111 1101 0000, 0x7D0
#define TYPE_SPC_32                  2000    //0b0111 1101 0000, 0x7D0
#define TYPE_SINGLE_INDICATION       1990    //0b0111 1100 0110, 0x7C6
#define TYPE_SI                      1990    //0b0111 1100 0110, 0x7C6
#define TYPE_DOUBLE_INDICATION       1980    //0b0111 1011 1100, 0x7BC
#define TYPE_DI                      1980    //0b0111 1011 1100, 0x7BC
#define TYPE_SINGLE_COMMAND          1970    //0b0111 1011 0010, 0x7B2
#define TYPE_SC                      1970    //0b0111 1011 0010, 0x7B2
#define TYPE_DOUBLE_COMMAND          1960    //0b0111 1010 1000, 0x7A8
#define TYPE_DC                      1960    //0b0111 1010 1000, 0x7A8
#define TYPE_CLOCK_SYNC              1950    //0b0111 1001 1110, 0x79E
#define TYPE_HEARTBEAT               1940    //0b0111 1001 0100, 0x794
/**
  * @}
  */

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
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SingleIndication_TypeDef TX_Single_Indication[TX_SINGLE_INDICATION_SIZE] = {
#if TX_SINGLE_INDICATION_SIZE >= 1
    {TX_SINGLE_INDICATION_0_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 2
    ,{TX_SINGLE_INDICATION_1_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 3
    ,{TX_SINGLE_INDICATION_2_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 4
    ,{TX_SINGLE_INDICATION_3_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 5
    ,{TX_SINGLE_INDICATION_4_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 6
    ,{TX_SINGLE_INDICATION_5_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 7
    ,{TX_SINGLE_INDICATION_6_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 8
    ,{TX_SINGLE_INDICATION_7_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 9
    ,{TX_SINGLE_INDICATION_8_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 10
    ,{TX_SINGLE_INDICATION_9_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 11
    ,{TX_SINGLE_INDICATION_10_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 12
    ,{TX_SINGLE_INDICATION_11_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 13
    ,{TX_SINGLE_INDICATION_12_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 14
    ,{TX_SINGLE_INDICATION_13_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 15
    ,{TX_SINGLE_INDICATION_14_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 16
    ,{TX_SINGLE_INDICATION_15_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 17
    ,{TX_SINGLE_INDICATION_16_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 18
    ,{TX_SINGLE_INDICATION_17_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 19
    ,{TX_SINGLE_INDICATION_18_ID, 0, 2, {0}}
#endif
#if TX_SINGLE_INDICATION_SIZE >= 20
    ,{TX_SINGLE_INDICATION_19_ID, 0, 2, {0}}
#endif
};
SingleIndication_TypeDef RX_Single_Indication[RX_SINGLE_INDICATION_SIZE] = {
#if RX_SINGLE_INDICATION_SIZE >= 1
    {RX_SINGLE_INDICATION_0_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 2
    ,{RX_SINGLE_INDICATION_1_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 3
    ,{RX_SINGLE_INDICATION_2_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 4
    ,{RX_SINGLE_INDICATION_3_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 5
    ,{RX_SINGLE_INDICATION_4_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 6
    ,{RX_SINGLE_INDICATION_5_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 7
    ,{RX_SINGLE_INDICATION_6_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 8
    ,{RX_SINGLE_INDICATION_7_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 9
    ,{RX_SINGLE_INDICATION_8_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 10
    ,{RX_SINGLE_INDICATION_9_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 11
    ,{RX_SINGLE_INDICATION_10_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 12
    ,{RX_SINGLE_INDICATION_11_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 13
    ,{RX_SINGLE_INDICATION_12_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 14
    ,{RX_SINGLE_INDICATION_13_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 15
    ,{RX_SINGLE_INDICATION_14_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 16
    ,{RX_SINGLE_INDICATION_15_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 17
    ,{RX_SINGLE_INDICATION_16_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 18
    ,{RX_SINGLE_INDICATION_17_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 19
    ,{RX_SINGLE_INDICATION_18_ID, 0, 2, {0}}
#endif
#if RX_SINGLE_INDICATION_SIZE >= 20
    ,{RX_SINGLE_INDICATION_19_ID, 0, 2, {0}}
#endif
};
DoubleIndication_TypeDef TX_Double_Indication[TX_DOUBLE_INDICATION_SIZE];
DoubleIndication_TypeDef RX_Double_Indication[RX_DOUBLE_INDICATION_SIZE];
MeasuredValue16_TypeDef  TX_Measured_Value_16[TX_MEASURED_VALUE_16_SIZE] = {
#if TX_MEASURED_VALUE_16_SIZE >= 1
    {TX_MEASURED_VALUE_16_0_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 2
    ,{TX_MEASURED_VALUE_16_1_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 3
    ,{TX_MEASURED_VALUE_16_2_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 4
    ,{TX_MEASURED_VALUE_16_3_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 5
    ,{TX_MEASURED_VALUE_16_4_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 6
    ,{TX_MEASURED_VALUE_16_5_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 7
    ,{TX_MEASURED_VALUE_16_6_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 8
    ,{TX_MEASURED_VALUE_16_7_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 9
    ,{TX_MEASURED_VALUE_16_8_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 10
    ,{TX_MEASURED_VALUE_16_9_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 11
    ,{TX_MEASURED_VALUE_16_10_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 12
    ,{TX_MEASURED_VALUE_16_11_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 13
    ,{TX_MEASURED_VALUE_16_12_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 14
    ,{TX_MEASURED_VALUE_16_13_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 15
    ,{TX_MEASURED_VALUE_16_14_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 16
    ,{TX_MEASURED_VALUE_16_15_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 17
    ,{TX_MEASURED_VALUE_16_16_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 18
    ,{TX_MEASURED_VALUE_16_17_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 19
    ,{TX_MEASURED_VALUE_16_18_ID, 0, (int16_t)0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 20
    ,{TX_MEASURED_VALUE_16_19_ID, 0, (int16_t)0xFFFF, {0}}
#endif
};
MeasuredValue16_TypeDef  RX_Measured_Value_16[RX_MEASURED_VALUE_16_SIZE] = {
#if RX_MEASURED_VALUE_16_SIZE >= 1
    {RX_MEASURED_VALUE_16_0_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 2
    ,{RX_MEASURED_VALUE_16_1_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 3
    ,{RX_MEASURED_VALUE_16_2_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 4
    ,{RX_MEASURED_VALUE_16_3_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 5
    ,{RX_MEASURED_VALUE_16_4_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 6
    ,{RX_MEASURED_VALUE_16_5_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 7
    ,{RX_MEASURED_VALUE_16_6_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 8
    ,{RX_MEASURED_VALUE_16_7_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 9
    ,{RX_MEASURED_VALUE_16_8_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 10
    ,{RX_MEASURED_VALUE_16_9_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 11
    ,{RX_MEASURED_VALUE_16_10_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 12
    ,{RX_MEASURED_VALUE_16_11_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 13
    ,{RX_MEASURED_VALUE_16_12_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 14
    ,{RX_MEASURED_VALUE_16_13_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 15
    ,{RX_MEASURED_VALUE_16_14_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 16
    ,{RX_MEASURED_VALUE_16_15_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 17
    ,{RX_MEASURED_VALUE_16_16_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 18
    ,{RX_MEASURED_VALUE_16_17_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 19
    ,{RX_MEASURED_VALUE_16_18_ID, 0, 0, {0}}
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 20
    ,{RX_MEASURED_VALUE_16_19_ID, 0, 0, {0}}
#endif
};
MeasuredValue32_TypeDef  TX_Measured_Value_32[TX_MEASURED_VALUE_32_SIZE] = {
#if TX_MEASURED_VALUE_32_SIZE >= 1
    {TX_MEASURED_VALUE_32_0_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 2
    ,{TX_MEASURED_VALUE_32_1_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 3
    ,{TX_MEASURED_VALUE_32_2_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 4
    ,{TX_MEASURED_VALUE_32_3_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 5
    ,{TX_MEASURED_VALUE_32_4_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 6
    ,{TX_MEASURED_VALUE_32_5_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 7
    ,{TX_MEASURED_VALUE_32_6_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 8
    ,{TX_MEASURED_VALUE_32_7_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 9
    ,{TX_MEASURED_VALUE_32_8_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 10
    ,{TX_MEASURED_VALUE_32_9_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 11
    ,{TX_MEASURED_VALUE_32_10_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 12
    ,{TX_MEASURED_VALUE_32_11_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 13
    ,{TX_MEASURED_VALUE_32_12_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 14
    ,{TX_MEASURED_VALUE_32_13_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 15
    ,{TX_MEASURED_VALUE_32_14_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 16
    ,{TX_MEASURED_VALUE_32_15_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 17
    ,{TX_MEASURED_VALUE_32_16_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 18
    ,{TX_MEASURED_VALUE_32_17_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 19
    ,{TX_MEASURED_VALUE_32_18_ID, 0, 0xFFFF, {0}}
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 20
    ,{TX_MEASURED_VALUE_32_19_ID, 0, 0xFFFF, {0}}
#endif
};
MeasuredValue32_TypeDef  RX_Measured_Value_32[RX_MEASURED_VALUE_32_SIZE] = {
#if RX_MEASURED_VALUE_32_SIZE >= 1
    {RX_MEASURED_VALUE_32_0_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 2
    ,{RX_MEASURED_VALUE_32_1_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 3
    ,{RX_MEASURED_VALUE_32_2_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 4
    ,{RX_MEASURED_VALUE_32_3_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 5
    ,{RX_MEASURED_VALUE_32_4_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 6
    ,{RX_MEASURED_VALUE_32_5_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 7
    ,{RX_MEASURED_VALUE_32_6_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 8
    ,{RX_MEASURED_VALUE_32_7_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 9
    ,{RX_MEASURED_VALUE_32_8_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 10
    ,{RX_MEASURED_VALUE_32_9_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 11
    ,{RX_MEASURED_VALUE_32_10_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 12
    ,{RX_MEASURED_VALUE_32_11_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 13
    ,{RX_MEASURED_VALUE_32_12_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 14
    ,{RX_MEASURED_VALUE_32_13_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 15
    ,{RX_MEASURED_VALUE_32_14_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 16
    ,{RX_MEASURED_VALUE_32_15_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 17
    ,{RX_MEASURED_VALUE_32_16_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 18
    ,{RX_MEASURED_VALUE_32_17_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 19
    ,{RX_MEASURED_VALUE_32_18_ID, 0, 0xFFFF, {0}}
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 20
    ,{RX_MEASURED_VALUE_32_19_ID, 0, 0xFFFF, {0}}
#endif
};

CanHA_MsgTypeDef CAN_RxMsgBuf[CAN_BUFFER_SIZE];
volatile uint_fast8_t CAN_RxMsg_WrIndex = 0;
volatile uint_fast8_t CAN_RxMsg_RdIndex = 0;

CanHA_MsgTypeDef CAN_TxMsgBuf[CAN_BUFFER_SIZE];
volatile uint_fast8_t CAN_TxMsg_WrIndex = 0;
volatile uint_fast8_t CAN_TxMsg_RdIndex = 0;

volatile uint32_t UnixTimestamp = 1000000;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Periodic send of Data. Should be start every second.
  * @param  None
  * @retval None
  */
void CANHA_WriteRefresh(void) {
    uint_fast8_t i;
    uint32_t tmp_RTC_Counter = UnixTimestamp;

    /* Single Indication */
    for (i = TX_SINGLE_INDICATION_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Single_Indication[i-1].Timestamp) % (REFRESH_TIME + 1) == 0) {
            CANHA_PutMsgToTxBuf(TYPE_SINGLE_INDICATION, TX_Single_Indication[i-1].Identifier, LENGTH_SINGLE_INDICATION, &TX_Single_Indication[i-1].State);
        }
    }

    /* Double Indication */
    for (i = TX_DOUBLE_INDICATION_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Double_Indication[i-1].Timestamp) % (REFRESH_TIME + 1) == 0) {
            CANHA_PutMsgToTxBuf(TYPE_DOUBLE_INDICATION, TX_Double_Indication[i-1].Identifier, LENGTH_DOUBLE_INDICATION, &TX_Double_Indication[i-1].State);
        }
    }

    /* Measured Value 16bit */
    for (i = TX_MEASURED_VALUE_16_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Measured_Value_16[i-1].Timestamp) % (REFRESH_TIME + 1) == 0 ) {
            uint_least8_t tmp[2];
            tmp[1] = (uint8_t)(TX_Measured_Value_16[i-1].Value);
            tmp[0] = (uint8_t)(TX_Measured_Value_16[i-1].Value>>8);
            CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_16, TX_Measured_Value_16[i-1].Identifier, LENGTH_MEASURED_VALUE_16, tmp);
        }
    }

    /* Measured Value 32bit */
    for (i = TX_MEASURED_VALUE_32_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Measured_Value_32[i-1].Timestamp) % (REFRESH_TIME + 1) == 0 ) {
            uint_least8_t tmp[4];
            tmp[3] = (uint8_t)(TX_Measured_Value_32[i-1].Value);
            tmp[2] = (uint8_t)(TX_Measured_Value_32[i-1].Value>>8);
            tmp[1] = (uint8_t)(TX_Measured_Value_32[i-1].Value>>16);
            tmp[0] = (uint8_t)(TX_Measured_Value_32[i-1].Value>>24);
            CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_32, TX_Measured_Value_32[i-1].Identifier, LENGTH_MEASURED_VALUE_32, tmp);
        }
    }

    /* Heartbeat */
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
void CANHA_WriteSingleIndication(uint_least32_t ObjectNumber, bool NewState) {
    if (TX_Single_Indication[ObjectNumber].State != NewState) {
        TX_Single_Indication[ObjectNumber].State = NewState;
        TX_Single_Indication[ObjectNumber].Timestamp = UnixTimestamp - 1;
        CANHA_PutMsgToTxBuf(TYPE_SINGLE_INDICATION, TX_Single_Indication[ObjectNumber].Identifier, LENGTH_SINGLE_INDICATION, &TX_Single_Indication[ObjectNumber].State);
    }
}

/**
  * @brief  This function change value of the 16bit measured value and send them.
  * @param  New Value: Fix Point Value e.g. 21,53°C = 2153
  * @retval None
  */
void CANHA_WriteMeasuredValue16(uint_least32_t ObjectNumber, int16_t NewValue) {
    if (TX_Measured_Value_16[ObjectNumber].Value != NewValue) {
        TX_Measured_Value_16[ObjectNumber].Value = NewValue;
        TX_Measured_Value_16[ObjectNumber].Timestamp = UnixTimestamp - 1;

        uint_least8_t tmp[2];
        tmp[1] = (uint8_t)(TX_Measured_Value_16[ObjectNumber].Value);
        tmp[0] = (uint8_t)(TX_Measured_Value_16[ObjectNumber].Value>>8);
        CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_16, TX_Measured_Value_16[ObjectNumber].Identifier, LENGTH_MEASURED_VALUE_16, tmp);
    }
}

/**
  * @brief  This function change value of the 32bit measured value and send them.
  * @param  New Value: Floating Point Value
  * @retval None
  */
void CANHA_WriteMeasuredValue32(uint_least32_t ObjectNumber, int32_t NewValue) {
    if (TX_Measured_Value_32[ObjectNumber].Value != NewValue) {
        TX_Measured_Value_32[ObjectNumber].Value = NewValue;
        TX_Measured_Value_32[ObjectNumber].Timestamp = UnixTimestamp - 1;

        uint_least8_t tmp[4];
        tmp[3] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value);
        tmp[2] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value>>8);
        tmp[1] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value>>16);
        tmp[0] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value>>24);
        CANHA_PutMsgToTxBuf(TYPE_MEASURED_VALUE_32, TX_Measured_Value_32[ObjectNumber].Identifier, LENGTH_MEASURED_VALUE_32, tmp);
    }
}

/**
  * @brief  Copy message from transmite buffer if there is a message to send.
  * @param  GetMessage: Pointer of messsage
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
  * @brief  Periodic read of Data.
  * @param  None
  * @retval None
  */
void CANHA_ReadRefresh(void) {
    CanHA_MsgTypeDef Msg;
    while (CANHA_GetMsgFromRxBuf(&Msg)) {
        switch (Msg.MessageType) {
            case TYPE_SINGLE_INDICATION: {
                for (uint_fast8_t i = 0; i < RX_SINGLE_INDICATION_SIZE; i++) {
                    if (Msg.Identifier == RX_Single_Indication[i].Identifier) {
                        RX_Single_Indication[i].Timestamp = UnixTimestamp;
                        RX_Single_Indication[i].State = Msg.Data[0];
                    }
                }

                #ifdef FIND_IDENTIFIER
                int_fast16_t TmpId = FindIdentifier(Msg.Identifier, sizeof(SingleIndication_TypeDef),
                                                    &RX_Single_Indication[0].Identifier, RX_SINGLE_INDICATION_SIZE);
                if (TmpId >= 0) {
                    RX_Single_Indication[TmpId].Timestamp = UnixTimestamp;
                    RX_Single_Indication[TmpId].State = Msg.Data[0];
                }
                #endif /* FIND_IDENTIFIER */
                break;
            }
            
            case TYPE_DOUBLE_INDICATION: {
                for (uint_fast8_t i = 0; i < RX_DOUBLE_INDICATION_SIZE; i++) {
                    if (Msg.Identifier == RX_Double_Indication[i].Identifier) {
                        RX_Double_Indication[i].Timestamp = UnixTimestamp;
                        RX_Double_Indication[i].State = Msg.Data[0];
                    }
                }
                
                #ifdef FIND_IDENTIFIER
                int_fast16_t TmpId = FindIdentifier(Msg.Identifier, sizeof(DoubleIndication_TypeDef),
                                                    &RX_Double_Indication[0].Identifier, RX_DOUBLE_INDICATION_SIZE);
                if (TmpId >= 0) {
                    RX_Single_Indication[TmpId].Timestamp = UnixTimestamp;
                    RX_Single_Indication[TmpId].State = Msg.Data[0];
                }
                #endif /* FIND_IDENTIFIER */
                break;
            }
            
            case TYPE_MEASURED_VALUE_16: {
                
                for (uint_fast8_t i = 0; i < RX_MEASURED_VALUE_16_SIZE; i++) {
                    if (Msg.Identifier == RX_Measured_Value_16[i].Identifier) {
                        RX_Measured_Value_16[i].Timestamp = UnixTimestamp;
                        RX_Measured_Value_16[i].Value = (int_least16_t)((Msg.Data[0] << 8) | Msg.Data[1]);
                    }
                }
                #ifdef FIND_IDENTIFIER
                int_fast16_t TmpId = FindIdentifier(Msg.Identifier, sizeof(MeasuredValue16_TypeDef),
                                                    &RX_Measured_Value_16[0].Identifier, RX_MEASURED_VALUE_16_SIZE);
                if (TmpId >= 0) {
                    RX_Measured_Value_16[TmpId].Timestamp = UnixTimestamp;
                    RX_Measured_Value_16[TmpId].Value = (Msg.Data[0] << 8) | Msg.Data[1];
                }
                #endif /* FIND_IDENTIFIER */
                break;
            }
            
            default: break;
        }
    }
}

bool CANHA_ReadSingleIndication(uint_fast8_t ObjectNumber) {
    return RX_Single_Indication[ObjectNumber].State;
}

bool CANHA_ReadMeasuredValue16(uint_fast8_t ObjectNumber, int_least16_t *Value) {
    if (RX_Measured_Value_16[ObjectNumber].Timestamp > 0) {
        *Value = RX_Measured_Value_16[ObjectNumber].Value;
        return true;
    } else {
        return false;
    }
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
    #if 0
    memcpy(&CAN_RxMsgBuf[CAN_RxMsg_WrIndex], GetMessage, sizeof(CanHA_MsgTypeDef));
    #endif /* 0 */
    
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
