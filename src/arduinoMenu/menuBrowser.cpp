/*
 * file : menuBrowser.cpp
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

#include "menuBrowser.h"
#include "menuData.h"

/*******************/
/* Private methods */
/*******************/

void MENU_BROWSER::gotoEntry(byte entry)
{
    if(entry != MENU_BROWSER_NO_ENTRY)
    {
        // update only on valid entry
        currentEntry = entry;
        if(refreshCallback)
            refreshCallback();
    }
}

/******************/
/* Public methods */
/******************/

MENU_BROWSER::MENU_BROWSER()
{
}

byte MENU_BROWSER::getCurrentEntry()
{
    return currentEntry;
}

byte MENU_BROWSER::getParent(byte index)
{
    return pgm_read_byte(parentTable + index);
}

byte MENU_BROWSER::getChild(byte index)
{
    return pgm_read_byte(childTable + index);
}

byte MENU_BROWSER::getNext(byte index)
{
    return pgm_read_byte(nextTable + index);
}

byte MENU_BROWSER::getPrevious(byte index)
{
    return pgm_read_byte(previousTable + index);
}

byte MENU_BROWSER::getEntryType(byte index)
{
    return pgm_read_byte(itemTypeTable + index);
}

const char* MENU_BROWSER::getLabel(byte index)
{
    // got from https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
    strcpy_P(buffer, (char *)pgm_read_word(&(labelsTable[index])));
    return buffer;
}

const char* MENU_BROWSER::getRootLabel()
{
    strcpy_P(buffer, MENU_BROWSER_ROOT_LABEL);
    return buffer;
}

void MENU_BROWSER::gotoChild()
{
    switch(getEntryType(currentEntry))
    {
        case menuTypeMenu:
            gotoEntry(getChild(currentEntry));
            break;
        case menuTypeVariable:
            setState(browserStateEditing);
            break;
        case menuTypeFunction:
            setState(browserStatePrefunction);
            break;
    }
}

void MENU_BROWSER::gotoParent()
{
    gotoEntry(getParent(currentEntry));
}

void MENU_BROWSER::gotoNext()
{
    gotoEntry(getNext(currentEntry));
}

void MENU_BROWSER::gotoPrevious()
{
    gotoEntry(getPrevious(currentEntry));
}

void MENU_BROWSER::valid()
{
    // NOT IMPLEMENTED YET!
}

void MENU_BROWSER::abort()
{
    // NOT IMPLEMENTED YET!
}

void MENU_BROWSER::setRefreshCallback(void (*callback)())
{
    refreshCallback = callback;
}

void MENU_BROWSER::setEditCallback(void (*callback)())
{
    editCallback = callback;
}

void MENU_BROWSER::setPreFunctionCallback(void (*callback)())
{
    preFunctionCallback = callback;
}

void MENU_BROWSER::setPostFunctionCallback(void (*callback)(byte err_num))
{
    postFunctionCallback = callback;
}

menuBrowserState MENU_BROWSER::getState()
{
    return state;
}

void MENU_BROWSER::setState(menuBrowserState _state)
{
    byte err_num;
    state = _state;
    switch(state)
    {
        case browserStateBrowsing:
            if(refreshCallback)
                refreshCallback();
            break;
        case browserStateEditing:
            if(editCallback)
                editCallback();
            break;
        case browserStatePrefunction:
            if(preFunctionCallback)
                preFunctionCallback();
            break;
        case browserStatePostfunction:
            err_num = getFunction(getCurrentEntry())();
                if(postFunctionCallback)
                    postFunctionCallback(err_num);
            else
            break;
        case browserStateUser:
            break;
        default:
            break;

    }
}

byte MENU_BROWSER::getVariableIndex(byte index)
{
    byte var_index = MENU_BROWSER_NO_ENTRY;
    for(byte x = 0; x < MENU_BROWSER_NB_ENTRIES; x++)
    {
        if(getEntryType(x) == menuTypeVariable)
            var_index++;
        if(x == index)
            break;
    }
    return var_index;
}

MENU_BROWSER_EDIT_PTR MENU_BROWSER::getVariableEditFunction(byte index)
{
    byte var_index = getVariableIndex(index);
    return pgm_read_word(editFunctionsTable + var_index);
}

byte MENU_BROWSER::getFunctionIndex(byte index)
{
    byte func_index = MENU_BROWSER_NO_ENTRY;
    for(byte x = 0; x < MENU_BROWSER_NB_ENTRIES; x++)
    {
        if(getEntryType(x) == menuTypeFunction)
            func_index++;
        if(x == index)
            break;
    }
    return func_index;
}

MENU_BROWSER_FUNCTION_PTR MENU_BROWSER::getFunction(byte index)
{
    byte func_index = getFunctionIndex(index);
    return pgm_read_word(execFunctionsTable + func_index);
}

