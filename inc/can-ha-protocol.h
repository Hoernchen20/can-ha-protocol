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
#include "can_communication.h"
#include "rtc.h"

/* Exported constants --------------------------------------------------------*/
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

/* Exported variables --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#ifndef BOOL
  #define BOOL
  typedef enum { FALSE, TRUE }bool;
#endif

/* Functionpointer */
typedef void (*SingleCommand_Function) (uint_least8_t);
typedef void (*DoubleCommand_Function) (uint_least8_t);
typedef void (*SetPoint16_Function) (int16_t);
typedef void (*SetPoint32_Function) (int32_t);

/* Structs for message types */

/**
  * @brief  Test Überschrift
  *
  * Hallo JAkjewnfkldsnvn wehf lanf gwipogj aljg
  */
typedef struct {
  uint_least32_t  Identifier;
  uint32_t        Timestamp;
  uint_least8_t   State;
  uint_least8_t   padding[3];
}SingleIndication_TypeDef;

typedef struct {
  uint_least32_t  Identifier;
  uint32_t        Timestamp;
  uint_least8_t   State;
  uint_least8_t   padding[3];
}DoubleIndication_TypeDef;

typedef struct {
  uint_least32_t  Identifier;
  uint32_t        Timestamp;
  int_least16_t   Value;
  uint_least8_t   padding[2];
}MeasuredValue16_TypeDef;

typedef struct {
  uint_least32_t  Identifier;
  uint32_t        Timestamp;
  int_least32_t   Value;
}MeasuredValue32_TypeDef;

typedef struct {
  uint_least32_t          Identifier;
  SingleCommand_Function  Function;
  int_least8_t            State;
  uint_least8_t           padding[3];
}SingleCommand_TypeDef;

typedef struct {
  uint_least32_t          Identifier;
  DoubleCommand_Function  Function;
  int_least8_t            State;
  uint_least8_t           padding[3];
}DoubleCommand_TypeDef;

typedef struct {
  uint_least32_t        Identifier;
  SetPoint16_Function   Function;
  int_least16_t         Value;
  uint_least8_t         padding[2];
}SetPoint16_TypeDef;

typedef struct {
  uint_least32_t        Identifier;
  SetPoint32_Function   Function;
  int_least32_t         Value;
}SetPoint32_TypeDef;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Config_CAN_HA_Protocol(void);
void Single_Indication_Init(void);
void Single_Indication_Refresh(void);
void Single_Indication_Write(uint_least32_t ObjectNumber, bool NewState);
void Measured_Value_16_Init(void);
void Measured_Value_16_Refresh(void);
void Measured_Value_16_Write(uint_least32_t ObjectNumber, int16_t NewValue);
void Measured_Value_32_Init(void);
void Measured_Value_32_Refresh(void);
void Measured_Value_32_Write(uint_least32_t ObjectNumber, int32_t NewValue);

#endif /* CAN_HA_PROTOCOL_H_ */
