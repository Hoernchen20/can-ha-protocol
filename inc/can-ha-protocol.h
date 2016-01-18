/**
 * @file    can-ha-protocol.h
 * @author  Felix Horn
 * @version 0.1
 * @date    25-Oct-2015
 * @brief   This file contains all the defines and functions prototypes for the
 *          CAN HomeAutomation Protocol.
 *
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


#ifndef CAN_HA_PROTOCOL_H_
#define CAN_HA_PROTOCOL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "can-ha-protocol-conf.h"

/* Exported constants --------------------------------------------------------*/
#define CAN_BUFFER_SIZE 8

/* Exported variables --------------------------------------------------------*/
extern volatile uint32_t UnixTimestamp;

/* Exported types ------------------------------------------------------------*/

/* Functionpointer */
typedef void (*SingleCommand_Function) (uint_least8_t);
typedef void (*DoubleCommand_Function) (uint_least8_t);
typedef void (*SetPoint16_Function) (int16_t);
typedef void (*SetPoint32_Function) (int32_t);

/* Structs for message types */
/**
  * @brief  Single Indication
  *
  * Test
  */
typedef struct {
    const uint_least32_t    Identifier;
    uint32_t                Timestamp;
    uint8_t                 State;
    uint_least8_t           padding[3];
}SingleIndication_TypeDef;

/**
  * @brief  Double Indication
  *
  * Test
  */
typedef struct {
    const uint_least32_t    Identifier;
    uint32_t                Timestamp;
    uint_least8_t           State;
    uint_least8_t           padding[3];
}DoubleIndication_TypeDef;

/**
  * @brief  Measured Value 16bit
  *
  * Fix point value
  * e.g. 21,53°C = 2153
  */
typedef struct {
    const uint_least32_t    Identifier;
    uint32_t                Timestamp;
    int_least16_t           Value;
    uint_least8_t           padding[2];
}MeasuredValue16_TypeDef;

/**
  * @brief  Measured Value 32bit
  *
  * Floating Point Value
  */
typedef struct {
    const uint_least32_t    Identifier;
    uint32_t                Timestamp;
    int_least32_t           Value;
}MeasuredValue32_TypeDef;

/**
  * @brief  Single Command
  *
  * Test
  */
typedef struct {
    const uint_least32_t    Identifier;
    SingleCommand_Function  Function;
    int_least8_t            State;
    uint_least8_t           padding[3];
}SingleCommand_TypeDef;

/**
  * @brief  Double Command
  *
  * Test
  */
typedef struct {
    const uint_least32_t    Identifier;
    DoubleCommand_Function  Function;
    int_least8_t            State;
    uint_least8_t           padding[3];
}DoubleCommand_TypeDef;

/**
  * @brief  Set Point 16bit
  *
  * Fix point value
  * e.g. 21,53°C = 2153
  */
typedef struct {
    const uint_least32_t    Identifier;
    SetPoint16_Function     Function;
    int_least16_t           Value;
    uint_least8_t           padding[2];
}SetPoint16_TypeDef;

/**
  * @brief  Set Point 32bit
  *
  * Floating point value
  */
typedef struct {
    const uint_least32_t    Identifier;
    SetPoint32_Function     Function;
    int_least32_t           Value;
}SetPoint32_TypeDef;

/**
  * @brief  CAN home automation message struct
  *
  * Test
  */
typedef struct {
  uint_least16_t            MessageType;
  uint_least32_t            Identifier;
  uint_least8_t             DataLength;
  uint_least8_t             Data[8];
}CanHA_MsgTypeDef;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Send data */
void CANHA_WriteRefresh(void);
void CANHA_WriteSingleIndication(uint_least32_t ObjectNumber, bool NewState);
void CANHA_WriteMeasuredValue16(uint_least32_t ObjectNumber, int16_t NewValue);
void CANHA_WriteMeasuredValue32(uint_least32_t ObjectNumber, int32_t NewValue);
bool CANHA_GetMsgFromTxBuf(CanHA_MsgTypeDef *GetMessage);
void CANHA_MsgSent(void);
void CANHA_PutMsgToTxBuf(uint_least16_t MessageType, uint_least32_t Identifier, uint_least8_t DataLength, uint_least8_t *Data);

/* Receive Data */
void CANHA_ReadRefresh(void);
bool CANHA_ReadSingleIndication(uint_fast8_t ObjectNumber);
bool CANHA_ReadMeasuredValue16(uint_fast8_t ObjectNumber, int_least16_t *Value);
bool CANHA_GetMsgFromRxBuf(CanHA_MsgTypeDef *GetMessage);
void CANHA_PutMsgToRxBuf(CanHA_MsgTypeDef *GetMessage); /* Insert in Can rx interrupt */

/* Unix Timestamp */
void CANHA_IncUnixTimestamp(void);
void CANHA_SetUnixTimestamp(uint_least32_t NewTimestamp);
uint_least32_t CANHA_GetUnixTimestamp(void);
#endif /* CAN_HA_PROTOCOL_H_ */
