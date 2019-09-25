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

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

ARDUINO_MENU::ARDUINO_MENU()
{
}

void ARDUINO_MENU::begin(MENU_BROWSER* _browser, MENU_DISPLAY* _display, MENU_INPUT* _input)
{
    browser = _browser;
    display = _display;
    input   = _input;
    
    input->addBrowser(browser);
    display->addBrowser(browser);

    browser->setRefreshCallback(MENU_DISPLAY_refreshBrowserScreen);
    browser->setEditCallback(MENU_DISPLAY_showEditVariableScreen);
    browser->setPreFunctionCallback(MENU_DISPLAY_showPreFunctionScreen);
    browser->setPostFunctionCallback(MENU_DISPLAY_showPostFunctionScreen);

    browser->setState(browserStateBrowsing);
}

void ARDUINO_MENU::sequencer()
{
    input->sequencer();
}

void ARDUINO_MENU::printVariable(char* str_var)
{
    display->printVariable(str_var);
}
