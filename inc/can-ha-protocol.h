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
#include "can-ha-protocol-conf.h"

/* Exported constants --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#ifndef BOOL
#define BOOL
typedef enum { FALSE, TRUE }bool;
#endif

struct {
  uint_least32_t  Identifier;
  uint_least8_t   State;
  uint_least8_t   TimestampSecond;
  uint_least8_t   padding[2];
}Single_Indication[SINGLE_INDICATION_SIZE];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Config_CAN_HA_Protocol(void);
void Single_Indication_Init(void);
void Single_Indication_Refresh(void);
void Single_Indication_Write(uint_least32_t ObjectNumber, bool NewState);

#endif /* CAN_HA_PROTOCOL_H_ */