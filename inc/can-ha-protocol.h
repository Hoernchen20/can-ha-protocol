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

/* Uncomment used */
#define NUMBER_TRANSMIT_SINGLE_INDICATON    4
#define NUMBER_TRANSMIT_DOUBLE_INDICATON    0
#define NUMBER_TRANSMIT_MEASURED_VALUE_16   2
#define NUMBER_TRANSMIT_MEASURED_VALUE_32    0

#define NUMBER_RECEIVE_SINGLE_INDICATION    0
#define NUMBER_RECEIVE_DOUBLE_INDICATION    0
#define NUMBER_RECEIVE_MEASURED_VALUE_16    1
#define NUMBER_RECEIVE_MEASURED_VALUE_32    0
#define NUMBER_RECEIVE_SETPOINT_16          1
#define NUMBER_RECEIVE_SETPOINT_32          0

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
    bool                    State;
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
    const SingleCommand_Function  Function;
}SingleCommand_TypeDef;

/**
  * @brief  Double Command
  *
  * Test
  */
typedef struct {
    const uint_least32_t    Identifier;
    const DoubleCommand_Function  Function;
}DoubleCommand_TypeDef;

/**
  * @brief  Set Point 16bit
  *
  * Fix point value
  * e.g. 21,53°C = 2153
  */
typedef struct {
    const uint_least32_t    Identifier;
    const SetPoint16_Function     Function;
}SetPoint16_TypeDef;

/**
  * @brief  Set Point 32bit
  *
  * Floating point value
  */
typedef struct {
    const uint_least32_t    Identifier;
    const SetPoint32_Function     Function;
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

/* Exported variables --------------------------------------------------------*/
extern volatile uint32_t UnixTimestamp;
//extern SingleIndication_TypeDef TX_Single_Indication[NUMBER_TRANSMIT_SINGLE_INDICATON];
extern DoubleIndication_TypeDef TX_Double_Indication[NUMBER_TRANSMIT_DOUBLE_INDICATON];
//extern MeasuredValue16_TypeDef TX_Measured_Value_16[NUMBER_TRANSMIT_MEASURED_VALUE_16];
extern MeasuredValue32_TypeDef TX_Measured_Value_32[NUMBER_TRANSMIT_MEASURED_VALUE_32];
extern SingleIndication_TypeDef RX_Single_Indication[NUMBER_RECEIVE_SINGLE_INDICATION];
extern MeasuredValue16_TypeDef RX_Measured_Value_16[NUMBER_RECEIVE_MEASURED_VALUE_16];
extern MeasuredValue32_TypeDef RX_Measured_Value_32[NUMBER_RECEIVE_MEASURED_VALUE_32];
extern SetPoint16_TypeDef RX_SetPoint16[NUMBER_RECEIVE_SETPOINT_16];
extern SetPoint32_TypeDef RX_SetPoint32[NUMBER_RECEIVE_SETPOINT_32];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Send data */
void CANHA_Heartbeat(void);

void CANHA_WriteSingleIndication(SingleIndication_TypeDef *Array, bool NewState);
void CANHA_RefreshSingleIndication(SingleIndication_TypeDef *Array, uint_fast8_t Size);

void CANHA_WriteDoubleIndication(DoubleIndication_TypeDef *Array, uint_least8_t NewState);
void CANHA_RefreshDoubleIndication(DoubleIndication_TypeDef *Array, uint_fast8_t Size);

void CANHA_WriteMeasuredValue16(MeasuredValue16_TypeDef *Array, int16_t NewValue);
void CANHA_RefreshMeasuredValue16(MeasuredValue16_TypeDef *Array, uint_fast8_t Size);

void CANHA_WriteMeasuredValue32(MeasuredValue32_TypeDef *Array, int32_t NewValue);
void CANHA_RefreshMeasuredValue32(MeasuredValue32_TypeDef *Array, uint_fast8_t Size);

bool CANHA_GetMsgFromTxBuf(CanHA_MsgTypeDef *GetMessage);
void CANHA_MsgSent(void);
void CANHA_PutMsgToTxBuf(uint_least16_t MessageType, uint_least32_t Identifier, uint_least8_t DataLength, uint_least8_t *Data);

/* Receive Data */
void CANHA_ReadRefresh(void);
bool CANHA_ReadSingleIndication(uint_fast8_t ObjectNumber);
bool CANHA_ReadMeasuredValue16(uint_fast8_t ObjectNumber, int_least16_t *Value);
bool CANHA_GetMsgFromRxBuf(CanHA_MsgTypeDef *GetMessage);
void CANHA_PutMsgToRxBuf(CanHA_MsgTypeDef *GetMessage); /* Insert in Can rx interrupt */

#endif /* CAN_HA_PROTOCOL_H_ */
