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
Single_IndicationTypeDef Single_Indication[SINGLE_INDICATION_SIZE];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configure the different object types.
  * @param  None
  * @retval None
  */
void Config_CAN_HA_Protocol(void) {
  Single_Indication_Init();
}

/**
  * @brief  This function configure the identifier of the single indications.
  * @param  None
  * @retval None
  */
void Single_Indication_Init(void) {
#if SINGLE_INDICATION_SIZE >= 1
  Single_Indication[0].Identifier = SINGLE_INDICATION_0_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 2
  Single_Indication[1].Identifier = SINGLE_INDICATION_1_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 3
  Single_Indication[2].Identifier = SINGLE_INDICATION_2_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 4
  Single_Indication[3].Identifier = SINGLE_INDICATION_3_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 5
  Single_Indication[4].Identifier = SINGLE_INDICATION_4_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 6
  Single_Indication[5].Identifier = SINGLE_INDICATION_5_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 7
  Single_Indication[6].Identifier = SINGLE_INDICATION_6_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 8
  Single_Indication[7].Identifier = SINGLE_INDICATION_7_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 9
  Single_Indication[8].Identifier = SINGLE_INDICATION_8_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 10
  Single_Indication[9].Identifier = SINGLE_INDICATION_9_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 11
  Single_Indication[10].Identifier = SINGLE_INDICATION_10_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 12
  Single_Indication[11].Identifier = SINGLE_INDICATION_11_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 13
  Single_Indication[12].Identifier = SINGLE_INDICATION_12_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 14
  Single_Indication[13].Identifier = SINGLE_INDICATION_13_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 15
  Single_Indication[14].Identifier = SINGLE_INDICATION_14_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 16
  Single_Indication[15].Identifier = SINGLE_INDICATION_15_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 17
  Single_Indication[16].Identifier = SINGLE_INDICATION_16_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 18
  Single_Indication[17].Identifier = SINGLE_INDICATION_17_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 19
  Single_Indication[18].Identifier = SINGLE_INDICATION_18_ID;
#endif
#if SINGLE_INDICATION_SIZE >= 20
  Single_Indication[19].Identifier = SINGLE_INDICATION_19_ID;
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

  for (i = SINGLE_INDICATION_SIZE; i; i--) {
    if ( (tmp_RTC_Counter - Single_Indication[i-1].Timestamp) % 60 ) {
      CAN_TxMsgHandle(EM, CAN_RTR_DATA, Laenge_EM, Single_Indication[i-1].Identifier, &Single_Indication[i-1].State);
    }
  }
}

/**
  * @brief  This function change state of the single indications and send them.
  * @param  State: Should be TRUE or FALSE.
  * @retval None
  */
void Single_Indication_Write(uint_least32_t ObjectNumber, bool NewState) {
  if (Single_Indication[ObjectNumber].State != NewState) {
    Single_Indication[ObjectNumber].State = NewState;
    Single_Indication[ObjectNumber].Timestamp = RTC_GetCounter();
    CAN_TxMsgHandle(EM, CAN_RTR_DATA, Laenge_EM, Single_Indication[ObjectNumber].Identifier, &Single_Indication[ObjectNumber].State);
  }
}
