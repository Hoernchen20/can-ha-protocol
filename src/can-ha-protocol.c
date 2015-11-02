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
#include "../inc/can-ha-protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Message Types */
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

/* Data length of message types */
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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SingleIndication_TypeDef TX_Single_Indication[TX_SINGLE_INDICATION_SIZE];
SingleIndication_TypeDef RX_Single_Indication[RX_SINGLE_INDICATION_SIZE];
MeasuredValue16_TypeDef  TX_Measured_Value_16[TX_MEASURED_VALUE_16_SIZE];
MeasuredValue16_TypeDef  RX_Measured_Value_16[RX_MEASURED_VALUE_16_SIZE];
MeasuredValue32_TypeDef  TX_Measured_Value_32[TX_MEASURED_VALUE_32_SIZE];
MeasuredValue32_TypeDef  RX_Measured_Value_32[RX_MEASURED_VALUE_32_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Single_Indication_Init(void);
static void Measured_Value_16_Init(void);
static void Measured_Value_32_Init(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function configure the different object types.
  * @param  None
  * @retval None
  */
void Config_CAN_HA_Protocol(void) {
    Single_Indication_Init();
    Measured_Value_16_Init();
    Measured_Value_32_Init();
}

/**
  * @brief  Periodic send of Data. Should be start every second.
  * @param  None
  * @retval None
  */
void CAN_HA_Refresh(void) {
    uint_fast8_t i;
    uint32_t tmp_RTC_Counter = RTC_GetUnixTime();

    /* Single Indication */
    for (i = TX_SINGLE_INDICATION_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Single_Indication[i-1].Timestamp) % 60 == 0) {
            CAN_TxMsgHandle(TYPE_SINGLE_INDICATION, CAN_RTR_DATA, LENGTH_SINGLE_INDICATION,
                TX_Single_Indication[i-1].Identifier, &TX_Single_Indication[i-1].State);
        }
    }

    /* Double Indication */


    /* Measured Value 16bit */
    for (i = TX_MEASURED_VALUE_16_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Measured_Value_16[i-1].Timestamp) % 60 == 0 ) {
            uint_least8_t tmp[2];
            tmp[1] = (uint8_t)(TX_Measured_Value_16[i-1].Value);
            tmp[0] = (uint8_t)(TX_Measured_Value_16[i-1].Value>>8);
            CAN_TxMsgHandle(TYPE_MEASURED_VALUE_16, CAN_RTR_DATA, LENGTH_MEASURED_VALUE_16,
                TX_Measured_Value_16[i-1].Identifier, tmp);
        }
    }

    /* Measured Value 32bit */
    for (i = TX_MEASURED_VALUE_32_SIZE; i; i--) {
        if ( (tmp_RTC_Counter - TX_Measured_Value_32[i-1].Timestamp) % 60 == 0 ) {
            uint_least8_t tmp[4];
            tmp[3] = (uint8_t)(TX_Measured_Value_32[i-1].Value);
            tmp[2] = (uint8_t)(TX_Measured_Value_32[i-1].Value>>8);
            tmp[1] = (uint8_t)(TX_Measured_Value_32[i-1].Value>>16);
            tmp[0] = (uint8_t)(TX_Measured_Value_32[i-1].Value>>24);
            CAN_TxMsgHandle(TYPE_MEASURED_VALUE_32, CAN_RTR_DATA, LENGTH_MEASURED_VALUE_32,
                TX_Measured_Value_32[i-1].Identifier, tmp);
        }
    }
}

/**
  * @brief  This function configure the identifier of the single indications.
  * @param  None
  * @retval None
  */
static void Single_Indication_Init(void) {
#if TX_SINGLE_INDICATION_SIZE >= 1
    TX_Single_Indication[0].Identifier = TX_SINGLE_INDICATION_0_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 2
    TX_Single_Indication[1].Identifier = TX_SINGLE_INDICATION_1_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 3
    TX_Single_Indication[2].Identifier = TX_SINGLE_INDICATION_2_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 4
    TX_Single_Indication[3].Identifier = TX_SINGLE_INDICATION_3_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 5
    TX_Single_Indication[4].Identifier = TX_SINGLE_INDICATION_4_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 6
    TX_Single_Indication[5].Identifier = TX_SINGLE_INDICATION_5_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 7
    TX_Single_Indication[6].Identifier = TX_SINGLE_INDICATION_6_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 8
    TX_Single_Indication[7].Identifier = TX_SINGLE_INDICATION_7_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 9
    TX_Single_Indication[8].Identifier = TX_SINGLE_INDICATION_8_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 10
    TX_Single_Indication[9].Identifier = TX_SINGLE_INDICATION_9_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 11
    TX_Single_Indication[10].Identifier = TX_SINGLE_INDICATION_10_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 12
    TX_Single_Indication[11].Identifier = TX_SINGLE_INDICATION_11_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 13
    TX_Single_Indication[12].Identifier = TX_SINGLE_INDICATION_12_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 14
    TX_Single_Indication[13].Identifier = TX_SINGLE_INDICATION_13_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 15
    TX_Single_Indication[14].Identifier = TX_SINGLE_INDICATION_14_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 16
    TX_Single_Indication[15].Identifier = TX_SINGLE_INDICATION_15_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 17
    TX_Single_Indication[16].Identifier = TX_SINGLE_INDICATION_16_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 18
    TX_Single_Indication[17].Identifier = TX_SINGLE_INDICATION_17_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 19
    TX_Single_Indication[18].Identifier = TX_SINGLE_INDICATION_18_ID;
#endif
#if TX_SINGLE_INDICATION_SIZE >= 20
    TX_Single_Indication[19].Identifier = TX_SINGLE_INDICATION_19_ID;
#endif
  }

/**
  * @brief  This function change state of the single indications and send them.
  * @param  State: Can be TRUE or FALSE.
  * @retval None
  */
void Single_Indication_Write(uint_least32_t ObjectNumber, bool NewState) {
    if (TX_Single_Indication[ObjectNumber].State != NewState) {
        TX_Single_Indication[ObjectNumber].State = NewState;
        TX_Single_Indication[ObjectNumber].Timestamp = RTC_GetUnixTime();
        CAN_TxMsgHandle(TYPE_SINGLE_INDICATION, CAN_RTR_DATA, LENGTH_SINGLE_INDICATION,
            TX_Single_Indication[ObjectNumber].Identifier, &TX_Single_Indication[ObjectNumber].State);
    }
}

/**
  * @brief  This function configure the identifier of the 16bit measured values.
  * @param  None
  * @retval None
  */
static void Measured_Value_16_Init(void) {
  /* Send */
#if TX_MEASURED_VALUE_16_SIZE >= 1
    TX_Measured_Value_16[0].Identifier = TX_MEASURED_VALUE_16_0_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 2
    TX_Measured_Value_16[1].Identifier = TX_MEASURED_VALUE_16_1_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 3
    TX_Measured_Value_16[2].Identifier = TX_MEASURED_VALUE_16_2_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 4
    TX_Measured_Value_16[3].Identifier = TX_MEASURED_VALUE_16_3_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 5
    TX_Measured_Value_16[4].Identifier = TX_MEASURED_VALUE_16_4_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 6
    TX_Measured_Value_16[5].Identifier = TX_MEASURED_VALUE_16_5_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 7
    TX_Measured_Value_16[6].Identifier = TX_MEASURED_VALUE_16_6_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 8
    TX_Measured_Value_16[7].Identifier = TX_MEASURED_VALUE_16_7_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 9
    TX_Measured_Value_16[8].Identifier = TX_MEASURED_VALUE_16_8_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 10
    TX_Measured_Value_16[9].Identifier = TX_MEASURED_VALUE_16_9_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 11
    TX_Measured_Value_16[10].Identifier = TX_MEASURED_VALUE_16_10_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 12
    TX_Measured_Value_16[11].Identifier = TX_MEASURED_VALUE_16_11_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 13
    TX_Measured_Value_16[12].Identifier = TX_MEASURED_VALUE_16_12_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 14
    TX_Measured_Value_16[13].Identifier = TX_MEASURED_VALUE_16_13_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 15
    TX_Measured_Value_16[14].Identifier = TX_MEASURED_VALUE_16_14_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 16
    TX_Measured_Value_16[15].Identifier = TX_MEASURED_VALUE_16_15_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 17
    TX_Measured_Value_16[16].Identifier = TX_MEASURED_VALUE_16_16_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 18
    TX_Measured_Value_16[17].Identifier = TX_MEASURED_VALUE_16_17_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 19
    TX_Measured_Value_16[18].Identifier = TX_MEASURED_VALUE_16_18_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 20
    TX_Measured_Value_16[19].Identifier = TX_MEASURED_VALUE_16_19_ID;
#endif

  /* Receive */
#if RX_MEASURED_VALUE_16_SIZE >= 1
    RX_Measured_Value_16[0].Identifier = RX_MEASURED_VALUE_16_0_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 2
    RX_Measured_Value_16[1].Identifier = RX_MEASURED_VALUE_16_1_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 3
    RX_Measured_Value_16[2].Identifier = RX_MEASURED_VALUE_16_2_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 4
    RX_Measured_Value_16[3].Identifier = RX_MEASURED_VALUE_16_3_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 5
    RX_Measured_Value_16[4].Identifier = RX_MEASURED_VALUE_16_4_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 6
    RX_Measured_Value_16[5].Identifier = RX_MEASURED_VALUE_16_5_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 7
    RX_Measured_Value_16[6].Identifier = RX_MEASURED_VALUE_16_6_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 8
    RX_Measured_Value_16[7].Identifier = RX_MEASURED_VALUE_16_7_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 9
    RX_Measured_Value_16[8].Identifier = RX_MEASURED_VALUE_16_8_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 10
    RX_Measured_Value_16[9].Identifier = RX_MEASURED_VALUE_16_9_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 11
    RX_Measured_Value_16[10].Identifier = RX_MEASURED_VALUE_16_10_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 12
    RX_Measured_Value_16[11].Identifier = RX_MEASURED_VALUE_16_11_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 13
    RX_Measured_Value_16[12].Identifier = RX_MEASURED_VALUE_16_12_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 14
    RX_Measured_Value_16[13].Identifier = RX_MEASURED_VALUE_16_13_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 15
    RX_Measured_Value_16[14].Identifier = RX_MEASURED_VALUE_16_14_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 16
    RX_Measured_Value_16[15].Identifier = RX_MEASURED_VALUE_16_15_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 17
    RX_Measured_Value_16[16].Identifier = RX_MEASURED_VALUE_16_16_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 18
    RX_Measured_Value_16[17].Identifier = RX_MEASURED_VALUE_16_17_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 19
    RX_Measured_Value_16[18].Identifier = RX_MEASURED_VALUE_16_18_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 20
    RX_Measured_Value_16[19].Identifier = RX_MEASURED_VALUE_16_19_ID;
#endif
}

/**
  * @brief  This function change value of the 16bit measured value and send them.
  * @param  New Value: Fix Point Value e.g. 21,53°C = 2153
  * @retval None
  */
void Measured_Value_16_Write(uint_least32_t ObjectNumber, int16_t NewValue) {
    if (TX_Measured_Value_16[ObjectNumber].Value != NewValue) {
        TX_Measured_Value_16[ObjectNumber].Value = NewValue;
        TX_Measured_Value_16[ObjectNumber].Timestamp = RTC_GetUnixTime();

        uint_least8_t tmp[2];
        tmp[1] = (uint8_t)(TX_Measured_Value_16[ObjectNumber].Value);
        tmp[0] = (uint8_t)(TX_Measured_Value_16[ObjectNumber].Value>>8);
        CAN_TxMsgHandle(TYPE_MEASURED_VALUE_16, CAN_RTR_DATA, LENGTH_MEASURED_VALUE_16,
            TX_Measured_Value_16[ObjectNumber].Identifier, tmp);
    }
}

/**
  * @brief  This function configure the identifier of the 32bit measured values.
  * @param  None
  * @retval None
  */
static void Measured_Value_32_Init(void) {
    /* Send */
#if TX_MEASURED_VALUE_32_SIZE >= 1
    TX_Measured_Value_32[0].Identifier = TX_MEASURED_VALUE_32_0_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 2
    TX_Measured_Value_32[1].Identifier = TX_MEASURED_VALUE_32_1_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 3
    TX_Measured_Value_32[2].Identifier = TX_MEASURED_VALUE_32_2_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 4
    TX_Measured_Value_32[3].Identifier = TX_MEASURED_VALUE_32_3_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 5
    TX_Measured_Value_32[4].Identifier = TX_MEASURED_VALUE_32_4_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 6
    TX_Measured_Value_32[5].Identifier = TX_MEASURED_VALUE_32_5_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 7
    TX_Measured_Value_32[6].Identifier = TX_MEASURED_VALUE_32_6_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 8
    TX_Measured_Value_32[7].Identifier = TX_MEASURED_VALUE_32_7_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 9
    TX_Measured_Value_32[8].Identifier = TX_MEASURED_VALUE_32_8_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 10
    TX_Measured_Value_32[9].Identifier = TX_MEASURED_VALUE_32_9_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 11
    TX_Measured_Value_32[10].Identifier = TX_MEASURED_VALUE_32_10_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 12
    TX_Measured_Value_32[11].Identifier = TX_MEASURED_VALUE_32_11_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 13
    TX_Measured_Value_32[12].Identifier = TX_MEASURED_VALUE_32_12_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 14
    TX_Measured_Value_32[13].Identifier = TX_MEASURED_VALUE_32_13_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 15
    TX_Measured_Value_32[14].Identifier = TX_MEASURED_VALUE_32_14_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 32
    TX_Measured_Value_32[15].Identifier = TX_MEASURED_VALUE_32_15_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 17
    TX_Measured_Value_32[32].Identifier = TX_MEASURED_VALUE_32_32_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 18
    TX_Measured_Value_32[17].Identifier = TX_MEASURED_VALUE_32_17_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 19
    TX_Measured_Value_32[18].Identifier = TX_MEASURED_VALUE_32_18_ID;
#endif
#if TX_MEASURED_VALUE_32_SIZE >= 20
    TX_Measured_Value_32[19].Identifier = TX_MEASURED_VALUE_32_19_ID;
#endif

  /* Receive */
#if RX_MEASURED_VALUE_32_SIZE >= 1
    RX_Measured_Value_32[0].Identifier = RX_MEASURED_VALUE_32_0_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 2
    RX_Measured_Value_32[1].Identifier = RX_MEASURED_VALUE_32_1_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 3
    RX_Measured_Value_32[2].Identifier = RX_MEASURED_VALUE_32_2_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 4
    RX_Measured_Value_32[3].Identifier = RX_MEASURED_VALUE_32_3_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 5
    RX_Measured_Value_32[4].Identifier = RX_MEASURED_VALUE_32_4_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 6
    RX_Measured_Value_32[5].Identifier = RX_MEASURED_VALUE_32_5_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 7
    RX_Measured_Value_32[6].Identifier = RX_MEASURED_VALUE_32_6_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 8
    RX_Measured_Value_32[7].Identifier = RX_MEASURED_VALUE_32_7_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 9
    RX_Measured_Value_32[8].Identifier = RX_MEASURED_VALUE_32_8_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 10
    RX_Measured_Value_32[9].Identifier = RX_MEASURED_VALUE_32_9_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 11
    RX_Measured_Value_32[10].Identifier = RX_MEASURED_VALUE_32_10_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 12
    RX_Measured_Value_32[11].Identifier = RX_MEASURED_VALUE_32_11_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 13
    RX_Measured_Value_32[12].Identifier = RX_MEASURED_VALUE_32_12_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 14
    RX_Measured_Value_32[13].Identifier = RX_MEASURED_VALUE_32_13_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 15
    RX_Measured_Value_32[14].Identifier = RX_MEASURED_VALUE_32_14_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 32
    RX_Measured_Value_32[15].Identifier = RX_MEASURED_VALUE_32_15_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 17
    RX_Measured_Value_32[32].Identifier = RX_MEASURED_VALUE_32_32_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 18
    RX_Measured_Value_32[17].Identifier = RX_MEASURED_VALUE_32_17_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 19
    RX_Measured_Value_32[18].Identifier = RX_MEASURED_VALUE_32_18_ID;
#endif
#if RX_MEASURED_VALUE_32_SIZE >= 20
    RX_Measured_Value_32[19].Identifier = RX_MEASURED_VALUE_32_19_ID;
#endif
}

/**
  * @brief  This function change value of the 32bit measured value and send them.
  * @param  New Value: Floating Point Value
  * @retval None
  */
void Measured_Value_32_Write(uint_least32_t ObjectNumber, int32_t NewValue) {
    if (TX_Measured_Value_32[ObjectNumber].Value != NewValue) {
        TX_Measured_Value_32[ObjectNumber].Value = NewValue;
        TX_Measured_Value_32[ObjectNumber].Timestamp = RTC_GetUnixTime();

        uint_least8_t tmp[4];
        tmp[3] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value);
        tmp[2] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value>>8);
        tmp[1] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value>>16);
        tmp[0] = (uint8_t)(TX_Measured_Value_32[ObjectNumber].Value>>24);
        CAN_TxMsgHandle(TYPE_MEASURED_VALUE_32, CAN_RTR_DATA, LENGTH_MEASURED_VALUE_32,
            TX_Measured_Value_32[ObjectNumber].Identifier, tmp);
    }
}
