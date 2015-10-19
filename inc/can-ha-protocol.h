/*
 * can-ha-protocol.h
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


#ifndef CAN_HA_PROTOCOL_H_
#define CAN_HA_PROTOCOL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "can_communication.h"

/* Exported constants --------------------------------------------------------*/
/* Message Types */
#define MEASURED_VALUE_16       2030    //0b0111 1110 1110, 0x7EE
#define MV_16                   2030    //0b0111 1110 1110, 0x7EE
#define MEASURED_VALUE          2020    //0b0111 1110 0100, 0x7E4
#define MV_32                   2020    //0b0111 1110 0100, 0x7E4
#define SETPOINT_COMMAND_16     2010    //0b0111 1101 1010, 0x7DA
#define SPC_16                  2010    //0b0111 1101 1010, 0x7DA
#define SETPOINT_COMMAND_32     2000    //0b0111 1101 0000, 0x7D0
#define SPC_32                  2000    //0b0111 1101 0000, 0x7D0
#define SINGLE_INDICATION       1990    //0b0111 1100 0110, 0x7C6
#define SI                      1990    //0b0111 1100 0110, 0x7C6
#define DOUBLE_INDICATION       1980    //0b0111 1011 1100, 0x7BC
#define DI                      1980    //0b0111 1011 1100, 0x7BC
#define SINGLE_COMMAND          1970    //0b0111 1011 0010, 0x7B2
#define SC                      1970    //0b0111 1011 0010, 0x7B2
#define DOUBLE_COMMAND          1960    //0b0111 1010 1000, 0x7A8
#define DC                      1960    //0b0111 1010 1000, 0x7A8
#define CLOCK_SYNC              1950    //0b0111 1001 1110, 0x79E
#define HEARTBEAT               1940    //0b0111 1001 0100, 0x794

/* Exported variables --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#ifndef BOOL
  #define BOOL
  typedef enum { FALSE, TRUE }bool;
#endif

typedef struct {
  uint_least32_t  Identifier;
  uint32_t        Timestamp;
  uint_least8_t   State;
  uint_least8_t   padding[3];
}Single_IndicationTypeDef;

typedef struct {
  uint_least32_t  Identifier;
  uint32_t        Timestamp;
  uint_least8_t   State;
  uint_least8_t   padding[3];
}Double_IndicationTypeDef;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Config_CAN_HA_Protocol(void);
void Single_Indication_Init(void);
void Single_Indication_Refresh(void);
void Single_Indication_Write(uint_least32_t ObjectNumber, bool NewState);

#endif /* CAN_HA_PROTOCOL_H_ */
