/*
 * file : menuDisplay.cpp
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "menuDisplay.h"

/****************************************/
/* using static functions as workaround */
/*   for class' methods as callbacks    */
/****************************************/

MENU_DISPLAY* MENU_DISPLAY_SINGLETON;

void MENU_DISPLAY_refreshBrowserScreen()
{
    MENU_DISPLAY_SINGLETON->refreshBrowserScreen();
}

void MENU_DISPLAY_showEditVariableScreen()
{
    MENU_DISPLAY_SINGLETON->showEditVariableScreen();
}

void MENU_DISPLAY_showPreFunctionScreen()
{
    MENU_DISPLAY_SINGLETON->showPreFunctionScreen();
}

void MENU_DISPLAY_showPostFunctionScreen(byte err_num)
{
    MENU_DISPLAY_SINGLETON->showPostFunctionScreen(err_num);
}

/*******************/
/* Private methods */
/*******************/

void MENU_DISPLAY::printTitle(byte index)
{
    index = browser->getParent(index);
    if(index != MENU_BROWSER_NO_ENTRY)
    {
        Serial.print(browser->getLabel(index));
        Serial.write('\n');
    }
    else
        Serial.print(F(" ---*** MENU ***--- \n"));        
}

void MENU_DISPLAY::printObjectLabel(byte index)
{
    Serial.write('<');
    Serial.print(browser->getLabel(index));
    Serial.write('>');
    Serial.write('\n');
}

void MENU_DISPLAY::printEntriesList(byte index)
{
    byte current_entry_index = index;
    
    for(byte x = 0; x < (MENU_DISPLAY_NB_DISPLAYED_ITEMS - 1); x++)
    {
        if(browser->getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
            current_entry_index = browser->getPrevious(current_entry_index);
        else
            break;
    }
    
    for(byte row_num = 1; row_num <= MENU_DISPLAY_NB_DISPLAYED_ITEMS; row_num++)
    {
        if(current_entry_index != MENU_BROWSER_NO_ENTRY)
        {
            // let's manage first column dedicated to arrows
            if(row_num == 1)
            {
                if(browser->getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    Serial.write('u');
                else
                    Serial.write(' ');
            }
            else if(row_num == MENU_DISPLAY_NB_DISPLAYED_ITEMS)
            {
                if(browser->getNext(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    Serial.write('d');
                else
                    Serial.write(' ');
            }
            else
                Serial.write(' ');

            // let's display entry
            if(current_entry_index == index)
            {
                // this is the selected entry
                Serial.write('<');
                Serial.print(browser->getLabel(current_entry_index));
                Serial.flush();
                Serial.write('>');
            }
            else
            {
                // this entry is not selected
                Serial.write(' ');
                Serial.print(browser->getLabel(current_entry_index));
                Serial.flush();
                Serial.write(' ');
            }
            
            Serial.write('\n');
            current_entry_index = browser->getNext(current_entry_index);
        }
        else
        {
            // no more entry to display, let's display an empty line
            Serial.write('\n');
        }
    
    }
}

/******************/
/* Public methods */
/******************/

MENU_DISPLAY::MENU_DISPLAY()
{
    MENU_DISPLAY_SINGLETON = this;
}

void MENU_DISPLAY::addBrowser(MENU_BROWSER* _browser)
{
    browser = _browser;
}

void MENU_DISPLAY::refreshBrowserScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printEntriesList(index);
}

void MENU_DISPLAY::showEditVariableScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printObjectLabel(index);
    Serial.print(F("edit variable "));
    Serial.print(browser->getVariableIndex(index));
    Serial.write('\n');
    browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_JUST_DISPLAY);
    //~ Serial.write('\n');
}

void MENU_DISPLAY::showPreFunctionScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printObjectLabel(index);
    Serial.print(F("Execute or abort"));
    Serial.write('\n');
    Serial.write('\n');
}

void MENU_DISPLAY::showPostFunctionScreen(byte err_num)
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printObjectLabel(index);
    Serial.write('\n');
    if(err_num)
    {
        Serial.print(F("Error #"));
        Serial.print(err_num);
    }
    else
        Serial.print(F("Function executed"));
    Serial.write('\n');
}

void MENU_DISPLAY::printVariable(char* str_var)
{
    Serial.print(str_var);
    Serial.write('\n');
}

void MENU_DISPLAY::print(char* text)
{
    Serial.print(text);
}

void MENU_DISPLAY::write(char car)
{
    Serial.write(car);
}

void MENU_DISPLAY::gotoXY(byte x, byte y)
{
    Serial.print(F("goto("));
    Serial.print(x);
    Serial.write(':');
    Serial.print(y);
    Serial.write(')');
}

void MENU_DISPLAY::clearScreen()
{
    Serial.print(F("clearScreen\n\n\n\n\n\n\n\n\n\n"));
}

