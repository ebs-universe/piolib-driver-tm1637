/* 
   Copyright (C) 2025 Chintalagiri Shashank
   Copyright (C) 2020 Daniel Efimenko
    
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

/**
 * @file tm1637/protocol.h
 * @brief TM1637 7 segment LCD driver
 *
 * This implementation is based on the MIT licensed implementation by 
 * Daniel Efimenko published at https://github.com/Danya0x07/tm1637-simple-library
 * 
 */

#ifndef TM1637_PROTOCOL_H
#define TM1637_PROTOCOL_H

#include "config.h"

#if TM1637_ENABLED

// Command Prefixes
#define TM1637_CMD_PREFIX_DATA     (1U << 6)
#define TM1637_CMD_PREFIX_DISPLAY  (2U << 6)
#define TM1637_CMD_PREFIX_ADDRESS  (3U << 6)

// Data Command Bits
#define TM1637_CMD_DATA_READ       (1U << 1)
#define TM1637_CMD_DATA_WRITE      0U

#define TM1637_CMD_DATA_FIXADDR    (1U << 2)
#define TM1637_CMD_DATA_INCADDR    0U

#define TM1637_CMD_DATA_TESTMODE   (1U << 3)
#define TM1637_CMD_DATA_NORMAL     0U

// Display Command Bits
#define TM1637_CMD_DISP_ON         (1U << 3)
#define TM1637_CMD_DISP_OFF        0U

// Brightness Settings (BRn = n/16)
typedef enum TM1637_BRIGHTNESS_t{
    TM1637_DISP_BR1 = 0U,
    TM1637_DISP_BR2 = 1U,
    TM1637_DISP_BR4 = 2U,
    TM1637_DISP_BR10 = 3U,
    TM1637_DISP_BR11 = 4U,
    TM1637_DISP_BR12 = 5U,
    TM1637_DISP_BR13 = 6U,
    TM1637_DISP_BR14 = 7U
} tm1637_brightness_t;

// Addresses
typedef enum TM1637_ADDRESS_t {
    TM1637_ADDR_C0H = 0U,
    TM1637_ADDR_C1H = 1U,
    TM1637_ADDR_C2H = 2U,
    TM1637_ADDR_C3H = 3U,
    TM1637_ADDR_C4H = 4U,
    TM1637_ADDR_C5H = 5U
} tm1637_address_t;

#endif  
#endif
