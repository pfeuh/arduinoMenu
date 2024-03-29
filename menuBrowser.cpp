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
//~ #include "menuData.h"

#define peek pgm_read_byte
// TODO: to earn 700 bytes, use peek in 2 times instead of wpeek
#define wpeek pgm_read_word

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
    return peek(parentTable + index);
}

byte MENU_BROWSER::getChild(byte index)
{
    return peek(childTable + index);
}

byte MENU_BROWSER::getNext(byte index)
{
    return peek(nextTable + index);
}

byte MENU_BROWSER::getPrevious(byte index)
{
    return peek(previousTable + index);
}

byte MENU_BROWSER::getEntryType(byte index)
{
    return peek(itemTypeTable + index)& MENU_BROWSER_TYPE_MASK;
}

bool MENU_BROWSER::getReadOnly(byte index)
{
    return (peek(itemTypeTable + index)& MENU_BROWSER_RO_MASK) == MENU_BROWSER_RO_MASK;
}

bool MENU_BROWSER::getLiving(byte index)
{
    return (peek(itemTypeTable + index)& MENU_BROWSER_LIVING_MASK) == MENU_BROWSER_LIVING_MASK;
}

const char* MENU_BROWSER::getLabelAddress(byte index)
{
    //~ return (const char*)(labelsTable + index);
    
    // TODO: verify indexes > 127 because of type "byte"
    return (const char*)wpeek(labelsTable + index * 2);

}

const char* MENU_BROWSER::getRootLabelAddress()
{
    return (const char*)rootLabel;
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
    
    if(state != browserStateEditing)
        livingIsRunning = false;
    
    switch(state)
    {
        case browserStateBrowsing:
            if(refreshCallback)
                refreshCallback();
            break;
        case browserStateEditing:
            if(getLiving(currentEntry))
            {
                livingIsRunning = true;
                livingMilestone = millis() + MENU_BROWSER_LIVING_PERIOD_MSEC;
            }
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
            //~ else
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
    for(byte x = 0; x < nbEntries; x++)
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
    return wpeek(editFunctionsTable + var_index * 2);
}

byte MENU_BROWSER::getFunctionIndex(byte index)
{
    byte func_index = MENU_BROWSER_NO_ENTRY;
    for(byte x = 0; x < nbEntries; x++)
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
    return wpeek(execFunctionsTable + func_index);
}

bool MENU_BROWSER::itIsShowTime()
{
    bool flag = false;
    if(livingIsRunning)
        if(millis() >= livingMilestone)
        {
            flag = true;
            livingMilestone += MENU_BROWSER_LIVING_PERIOD_MSEC;
        }
    return flag;
}

void MENU_BROWSER::begin(byte nb_entries, word* tables)
{
    nbEntries = nb_entries;
    rootLabel = tables[0];
    parentTable = tables[1];
    childTable = tables[2];
    nextTable = tables[3];
    previousTable = tables[4];
    labelsTable = tables[5];
    execFunctionsTable = (MENU_BROWSER_FUNCTION_PTR*)tables[6]; 
    editFunctionsTable = tables[7];
    itemTypeTable = tables[8];
}


