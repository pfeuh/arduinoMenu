/*
 * file : arduinoMenu.cpp
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


#include "arduinoMenu.h"
#include "menuBrowser.h"

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

ARDUINO_MENU::ARDUINO_MENU()
{
}

//~ void ARDUINO_MENU::begin(MATRIX_KEYBOARD* _keyboard, LCD_20X4_IIC* _lcd, INCREMENTAL_ENCODER* _encoder, MENU* _menu)
void ARDUINO_MENU::begin()
{
    //~ keyboard = _keyboard;
    //~ lcd = _lcd;
    //~ encoder = _encoder;
    //~ menu = _menu;
    
    //~ lcd->begin();
    //~ lcd->setBacklight(1);
    //~ lcd->clear();
    
    //~ keyboard->begin();
}

void ARDUINO_MENU::sequencer()
{
    //~ keyboard->sequencer();
    //~ encoder->sequencer();
}

void ARDUINO_MENU::refresh()
{
    //~ lcd->clear();
    //~ lcd->home();
    //~ lcd->print(menu->getLabel(menu->getCurrentEntry()));
}
