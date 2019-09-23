#ifndef menuBrowser_h
#define menuBrowser_h

/*
 * file : menuBrowser.h
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

#include <Arduino.h>

#define MENU_BROWSER_VERSION "1.00"

#define MENU_BROWSER_NO_ENTRY 0xff
enum menuOptionType {menuTypeMenu=1, menuTypeVariable=2, menuTypeFunction=3};
typedef void (*MENU_BROWSER_FUNCTION_PTR)(void);
#define MENU_BROWSER_MAX_LABEL_LEN 17

class MENU_BROWSER
{
    public:
        MENU_BROWSER();
        void begin();
        byte getCurrentEntry();
        byte getParent(byte index);
        byte getChild(byte index);
        byte getNext(byte index);
        byte getPrevious(byte index);
        byte getEntryType(byte index);
        const char* getLabel(byte index);
        void gotoChild();
        void gotoParent();
        void gotoNext();
        void gotoPrevious();
        void valid();
        void abort();
        void setRefreshCallback(void (*callback)());

    private:
        byte currentEntry;
        void gotoEntry(byte entry);
        void (*refreshCallback)();
        char buffer[MENU_BROWSER_MAX_LABEL_LEN+1];

};

#endif

