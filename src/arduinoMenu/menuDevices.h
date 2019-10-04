#ifndef menuDevices_h
#define menuDevices_h

/*
 * file : menuDevices.h
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>

/*****************/
/* Input devices */
/*****************/

#define MENU_INPUT_DEVICE_NONE     0
#define MENU_INPUT_DEVICE_SERIAL   1
#define MENU_INPUT_DEVICE_JOYSTICK 2
#define MENU_INPUT_DEVICE_KEYB16   3
#define MENU_INPUT_DEVICE_ENCODER  4

// let's select an input device from list above
#define  MENU_INPUT_DEVICE MENU_INPUT_DEVICE_ENCODER

/******************/
/* Output devices */
/******************/

#define MENU_OUTPUT_DEVICE_NONE    0
#define MENU_OUTPUT_DEVICE_SERIAL  1
#define MENU_OUTPUT_DEVICE_LCD_4x2 2

// let's select an output device from list above
#define  MENU_OUTPUT_DEVICE MENU_OUTPUT_DEVICE_LCD_4x2

#endif

