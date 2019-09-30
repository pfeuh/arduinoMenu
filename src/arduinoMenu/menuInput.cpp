/*
 * file : menuInput.cpp
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

#include "menuInput.h"
#include "menuInputDevice.h"

#if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
#include <matrixKeyboard.h>
//~ // my "Nano" config
//~ int rows[] = {3, 4, 5, 6};
//~ int columns[] = {7, 8, 9, 2};
//~ // my "Mega" config
int  columns[] = {26, 24, 22, 36};
int  rows[]    = {34, 32, 30, 28};
// you definitely want a match between your charset and column number / row number
// <<< this table MUST be in RAM! >>>
char codes[] = {'1', '2', '3', 'u', '4', '5', '6', 'r', '7', '8', '9', 'l', '*', '0', '#', 'd'};
MATRIX_KEYBOARD keyboard = MATRIX_KEYBOARD(columns, sizeof(columns) / sizeof(int), rows, sizeof(rows) / sizeof(int), codes);
#endif

/******************/
/* Public methods */
/******************/

MENU_INPUT::MENU_INPUT()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    keyboard.begin();
    #endif
}

byte MENU_INPUT::available()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    return keyboard.available();
    #else
    return Serial.available();
    #endif
}

byte MENU_INPUT::read()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    return keyboard.read();
    #else
    return Serial.read();
    #endif
}

void MENU_INPUT::addBrowser(MENU_BROWSER* _browser)
{
    browser = _browser;
}

void MENU_INPUT::browsingSequencer(char car)
{
    switch(car)
    {
        case 'u': // up
            browser->gotoPrevious();
            break;
        case 'd': // down
            browser->gotoNext();
            break;
        case 'r': // right (entering a submenu)
            browser->gotoChild();
            break;
        case 'l': // left (leaving a submenu)
            browser->gotoParent();
            break;
        default:
            break;
    }
}

void MENU_INPUT::editingSequencer(char car)
{
    byte index = browser->getCurrentEntry();
    
    switch(car)
    {
        case 'u':
            browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_INCREASE);
            break;
        case 'd':
            browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_DECREASE);
            break;
        case 'l':
        case 'r':
            browser->setState(browserStateBrowsing);
            break;
        default:
            break;
    }
}

void MENU_INPUT::preFunctionSequencer(char car)
{
    switch(car)
    {
        case 'l':
            browser->setState(browserStateBrowsing);
            break;
        case 'r':
            browser->setState(browserStatePostfunction);
            break;
        default:
            break;
    }
}

void MENU_INPUT::postFunctionSequencer(char car)
{
    // byte index = browser->getCurrentEntry();
    
    switch(car)
    {
        case 'r':
            browser->setState(browserStateBrowsing);
            break;
        default:
            break;
    }
}

void MENU_INPUT::sequencer()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    keyboard.sequencer();
    if(keyboard.available())
    #else
    if(available())
    #endif
    {
        byte car = read();

        switch(browser->getState())
        {
            case browserStateBrowsing:
                browsingSequencer(car);
                break;
            case browserStateEditing:
                editingSequencer(car);
                break;
            case browserStatePrefunction:
                preFunctionSequencer(car);
                break;
            case browserStatePostfunction:
                postFunctionSequencer(car);
                break;
            case browserStateUser:
                //~ browsingSequencer(car);
                break;
        }
    }
}
