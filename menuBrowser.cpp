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

void MENU_BROWSER::begin()
{
    currentEntry = 0;
    refreshCallback = NULL;
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

byte MENU_BROWSER::getBrotherPosition(byte index)
{
    byte position = 0;
    
    index = getPrevious(index);
    while(index != MENU_BROWSER_NO_ENTRY)
    {
        position++;
        index = getPrevious(index);
    }
    return position;
}

byte MENU_BROWSER::getNbBrothers(byte index)
{
    byte nb_brothers = getBrotherPosition(index) + 1;
    
    while(getNext(index) != MENU_BROWSER_NO_ENTRY)
    {
        nb_brothers++;
        index = getNext(index);
    }
    return nb_brothers;
}

byte MENU_BROWSER::getFirstBrother(byte index)
{
    while(getPrevious(index) != MENU_BROWSER_NO_ENTRY)
    {
        index = getNext(index);
    }
    return index;
}

byte MENU_BROWSER::getBrotherByPosition(byte index, byte position)
{
    byte entry = getFirstBrother(index);
    
    while(position--)
        entry = getNext(entry);
    
    return entry;
}

byte MENU_BROWSER::getNbEntries()
{
    return MENU_BROWSER_NB_ENTRIES;
}

const char* MENU_BROWSER::getLabel(byte index)
{
    // got from https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
    strcpy_P(buffer, (char *)pgm_read_word(&(labelsTable[index])));
    return buffer;
}

void MENU_BROWSER::gotoChild()
{
    gotoEntry(getChild(currentEntry));
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
