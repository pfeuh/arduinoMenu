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

#if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_SERIAL)

/******************/
/* Public methods */
/******************/

MENU_INPUT::MENU_INPUT()
{
}

byte MENU_INPUT::available()
{
    return Serial.available();
}

byte MENU_INPUT::read()
{
    return Serial.read();
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
    if(available())
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

#endif

