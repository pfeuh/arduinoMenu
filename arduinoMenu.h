#ifndef arduinoMenuBrowser_h
#define arduinoMenuBrowser_h

/*
 * file : arduinoMenuBrowser.h
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
#include <matrixKeyboard.h>
#include <lcd20x4iic.h>
#include <incrementalEncoder.h>
#include "menuBrowser.h"

#define ARDUINO_MENU_VERSION "1.00"

typedef void (*ARDUINO_MENU_type_callback)(byte bt_num);

class ARDUINO_MENU
{
    public:
        ARDUINO_MENU();
        //~ void begin(Arduino* _keyboard, LCD_20X4_IIC* _lcd, INCREMENTAL_ENCODER* _encoder, MENU_BROWSER* menu);
        void begin();
        void sequencer();
        void refresh();

    private:
        //~ MATRIX_KEYBOARD* keyboard;
        //~ LCD_20X4_IIC* lcd;
        //~ INCREMENTAL_ENCODER* encoder;
        //~ MENU* menu;

    };

#endif

