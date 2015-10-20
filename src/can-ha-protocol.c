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
#include "can-ha-protocol-conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SingleIndication_TypeDef TX_Single_Indication[TX_SINGLE_INDICATION_SIZE];

MeasuredValue16_TypeDef TX_Measured_Value_16[TX_MEASURED_VALUE_16_SIZE];
MeasuredValue16_TypeDef RX_Measured_Value_16[RX_MEASURED_VALUE_16_SIZE];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configure the different object types.
  * @param  None
  * @retval None
  */
void Config_CAN_HA_Protocol(void) {
  Single_Indication_Init();
  //Measured_Value_16_Init();
}

/**
  * @brief  This function configure the identifier of the single indications.
  * @param  None
  * @retval None
  */
void Single_Indication_Init(void) {
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
  * @brief  This function sends the single indications and should be run every second.
  * @param  None
  * @retval None
  */
void Single_Indication_Refresh(void) {
  uint_fast8_t i;
  uint32_t tmp_RTC_Counter = RTC_GetCounter();

  for (i = TX_SINGLE_INDICATION_SIZE; i; i--) {
    if ( (tmp_RTC_Counter - TX_Single_Indication[i-1].Timestamp) % 60 == 0) {
      CAN_TxMsgHandle(TYPE_SINGLE_INDICATION, CAN_RTR_DATA, LENGTH_SINGLE_INDICATION,
          TX_Single_Indication[i-1].Identifier, &TX_Single_Indication[i-1].State);
    }
  }
}

/**
  * @brief  This function change state of the single indications and send them.
  * @param  State: Should be TRUE or FALSE.
  * @retval None
  */
void Single_Indication_Write(uint_least32_t ObjectNumber, bool NewState) {
  if (TX_Single_Indication[ObjectNumber].State != NewState) {
    TX_Single_Indication[ObjectNumber].State = NewState;
    TX_Single_Indication[ObjectNumber].Timestamp = RTC_GetCounter();
    CAN_TxMsgHandle(TYPE_SINGLE_INDICATION, CAN_RTR_DATA, LENGTH_SINGLE_INDICATION,
        TX_Single_Indication[ObjectNumber].Identifier, &TX_Single_Indication[ObjectNumber].State);
  }
}

/**
  * @brief  This function configure the identifier of the 16bit measured values.
  * @param  None
  * @retval None
  */
void Measured_Value_16_Init(void) {
  /* Send */
#if TX_MEASURED_VALUE_16_SIZE >= 1
  TX_Measured_Value_16[0].Identifier = TX_MEASURED_VALUE_16_0_ID;
#endif
#if TX_MEASURED_VALUE_16_SIZE >= 2
  TX_Measured_Value_16[1].Identifier = TX_MEASURED_VALUE_16_1_ID;
#endif

  /* Receive */
#if RX_MEASURED_VALUE_16_SIZE >= 1
  RX_Measured_Value_16[0].Identifier = RX_MEASURED_VALUE_16_0_ID;
#endif
#if RX_MEASURED_VALUE_16_SIZE >= 2
  RX_Measured_Value_16[1].Identifier = RX_MEASURED_VALUE_16_1_ID;
#endif
}

/**
  * @brief  This function sends the 16bit measured values and should be run every second.
  * @param  None
  * @retval None
  */
void Measured_Value_16_Refresh(void) {
  uint_fast8_t i;
  uint32_t tmp_RTC_Counter = RTC_GetCounter();

  for (i = TX_MEASURED_VALUE_16_SIZE; i; i--) {
    if ( (tmp_RTC_Counter - TX_Measured_Value_16[i-1].Timestamp) % 60 == 0 ) {
      uint_least8_t tmp[2];
      tmp[1] = (uint8_t)(TX_Measured_Value_16[i-1].Value);
      tmp[0] = (uint8_t)(TX_Measured_Value_16[i-1].Value>>8);
      CAN_TxMsgHandle(TYPE_MEASURED_VALUE_16, CAN_RTR_DATA, LENGTH_MEASURED_VALUE_16,
          TX_Measured_Value_16[i-1].Identifier, tmp);
    }
  }
}

/**
  * @brief  This function change value of the 16bit measured value and send them.
  * @param  State: Should be TRUE or FALSE.
  * @retval None
  */
void Measured_Value_16_Write(uint_least32_t ObjectNumber, int16_t NewValue) {
  if (TX_Measured_Value_16[ObjectNumber].Value != NewValue) {
    TX_Measured_Value_16[ObjectNumber].Value = NewValue;
    TX_Measured_Value_16[ObjectNumber].Timestamp = RTC_GetCounter();

    uint_least8_t tmp[2];
    tmp[1] = (uint8_t)(TX_Measured_Value_16[ObjectNumber].Value);
    tmp[0] = (uint8_t)(TX_Measured_Value_16[ObjectNumber].Value>>8);
    CAN_TxMsgHandle(TYPE_MEASURED_VALUE_16, CAN_RTR_DATA, LENGTH_MEASURED_VALUE_16,
        TX_Measured_Value_16[ObjectNumber].Identifier, tmp);
  }
}
