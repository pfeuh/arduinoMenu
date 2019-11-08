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

/******************/
/* Public methods */
/******************/

MENU_INPUT::MENU_INPUT()
{
}

void MENU_INPUT::begin(MENU_ENCODER* _encoder)
{
    encoder = _encoder;
    inputType = encoderType;
    encoder->begin();
}

void MENU_INPUT::begin(MENU_KEYBOARD* _keyboard)
{
    keyboard = _keyboard;
    inputType = keyboardType;
    keyboard->begin();
}

byte MENU_INPUT::available()
{
    switch(inputType)
    {
        case encoderType:
            return encoder->available();
        case keyboardType:
            return keyboard->available();
        default:
            return 0;
    }
}

byte MENU_INPUT::read()
{
    switch(inputType)
    {
        case encoderType:
            return encoder->read();
        case keyboardType:
            return keyboard->read();
        default:
            return 0;
    }
}

void MENU_INPUT::addBrowser(MENU_BROWSER* _browser)
{
    browser = _browser;
}

void MENU_INPUT::browsingSequencer(char car)
{
    switch(car)
    {
        // when encoder increases, menu has to go down 
        case MENU_INPUT_CHAR_CMD_DOWN: 
            browser->gotoPrevious();
            break;
        case MENU_INPUT_CHAR_CMD_UP:
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
    
    if(browser->getReadOnly(index) || browser->getLiving(index))
    {
        switch(car)
        {
            // if variable is readonly or living value,
            // all keys return to browsing
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
     encoder->sequencer();
    if(encoder->available())
    {
        byte car = encoder->read();
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
