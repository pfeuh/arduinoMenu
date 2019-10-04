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

#if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
#include <matrixKeyboard.h>
//~ // my "Nano/Uno" config
//~ int rows[] = {3, 4, 5, 6};
//~ int columns[] = {7, 8, 9, 2};
// my "Mega" config
int  columns[] = {26, 24, 22, 36};
int  rows[]    = {34, 32, 30, 28};
const char codes[] PROGMEM =
{
    '1', '2', '3', MENU_INPUT_CHAR_CMD_UP,
    '4', '5', '6', MENU_INPUT_CHAR_CMD_RIGHT,
    '7', '8', '9', MENU_INPUT_CHAR_CMD_LEFT,
    '*', '0', '#', MENU_INPUT_CHAR_CMD_DOWN,
};
MATRIX_KEYBOARD keyboard = MATRIX_KEYBOARD(columns, sizeof(columns) / sizeof(int), rows, sizeof(rows) / sizeof(int), codes);
#endif

#if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
#include "menuEncoder.h"
MENU_ENCODER encoder = MENU_ENCODER(4, 3, 2);
#endif

#if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_JOYSTICK)
#include "menuJoystick.h"
MENU_JOYSTICK joystick = MENU_JOYSTICK(52, 50, 48, 46);
#endif

/******************/
/* Public methods */
/******************/

MENU_INPUT::MENU_INPUT()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    keyboard.begin();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
    encoder.begin();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_JOYSTICK)
    joystick.begin();
    #endif
}

byte MENU_INPUT::available()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    return keyboard.available();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_JOYSTICK)
    return joystick.available();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_SERIAL)
    return Serial.available();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
    return encoder.available();
    #endif
}

byte MENU_INPUT::read()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    return keyboard.read();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_JOYSTICK)
    return joystick.read();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_SERIAL)
    return Serial.read();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
    return encoder.read();
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
        #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
            // when encoder increases, menu has to go down 
            case MENU_INPUT_CHAR_CMD_DOWN: 
        #else
        case MENU_INPUT_CHAR_CMD_UP:
        #endif
            browser->gotoPrevious();
            break;
        #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
            // when encoder decreases, menu has to go up 
            case MENU_INPUT_CHAR_CMD_UP:
        #else
            case MENU_INPUT_CHAR_CMD_DOWN:
        #endif
            browser->gotoNext();
            break;
        case MENU_INPUT_CHAR_CMD_RIGHT: // right (entering a submenu)
            browser->gotoChild();
            break;
        case MENU_INPUT_CHAR_CMD_LEFT: // left (leaving a submenu)
            browser->gotoParent();
            break;
        default:
            break;
    }
}

void MENU_INPUT::editingSequencer(char car)
{
    byte index = browser->getCurrentEntry();
    
    if(browser->getReadOnly(index))
    {
        switch(car)
        {
            // if variable is readonly,
            // all values return to browsing
            case MENU_INPUT_CHAR_CMD_UP:
            case MENU_INPUT_CHAR_CMD_DOWN:
            case MENU_INPUT_CHAR_CMD_LEFT:
            case MENU_INPUT_CHAR_CMD_RIGHT:
                browser->setState(browserStateBrowsing);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(car)
        {
            case MENU_INPUT_CHAR_CMD_UP:
                browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_INCREASE);
                break;
            case MENU_INPUT_CHAR_CMD_DOWN:
                browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_DECREASE);
                break;
            case MENU_INPUT_CHAR_CMD_LEFT:
            case MENU_INPUT_CHAR_CMD_RIGHT:
                browser->setState(browserStateBrowsing);
                break;
            default:
                break;
        }
    }
}

void MENU_INPUT::preFunctionSequencer(char car)
{
    switch(car)
    {
        case MENU_INPUT_CHAR_CMD_LEFT:
        case MENU_INPUT_CHAR_CMD_UP:
        case MENU_INPUT_CHAR_CMD_DOWN:
            browser->setState(browserStateBrowsing);
            break;
        case MENU_INPUT_CHAR_CMD_RIGHT:
            browser->setState(browserStatePostfunction);
            break;
        default:
            break;
    }
}

void MENU_INPUT::postFunctionSequencer(char car)
{
    switch(car)
    {
        case MENU_INPUT_CHAR_CMD_UP:
        case MENU_INPUT_CHAR_CMD_DOWN:
        case MENU_INPUT_CHAR_CMD_LEFT:
        case MENU_INPUT_CHAR_CMD_RIGHT:
            browser->setState(browserStateBrowsing);
            break;
        default:
            break;
    }
}

void MENU_INPUT::sequencer()
{
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_JOYSTICK)
    joystick.sequencer();
    if(joystick.available())
    {
        byte car = joystick.read();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_KEYB16)
    keyboard.sequencer();
    if(keyboard.available())
    {
        byte car = keyboard.read();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_SERIAL)
    if(available())
    {
        byte car = read();
    #endif
    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_ENCODER)
    encoder.sequencer();
    if(encoder.available())
    {
        byte car = encoder.read();
    #endif

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
