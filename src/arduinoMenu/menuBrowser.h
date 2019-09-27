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
enum menuOptionType {
    menuTypeMenu=1,
    menuTypeVariable=2,
    menuTypeFunction=3};
enum menuBrowserState {
    browserStateBrowsing=1,
    browserStateEditing=2,
    browserStatePrefunction=3,
    browserStatePostfunction=4,
    browserStateUser=5};
typedef byte (*MENU_BROWSER_FUNCTION_PTR)(void);
typedef void (*MENU_BROWSER_EDIT_PTR)(byte direction);
//~ #define MENU_BROWSER_MAX_LABEL_LEN 17
#define MENU_BROWSER_DATA_INCREASE 0
#define MENU_BROWSER_DATA_DECREASE 1
#define MENU_BROWSER_DATA_JUST_DISPLAY 2

// lcd width = 20, let's add string terminator '\0'
#define MENU_BROWSER_BUFFER_SIZE 21
    
class MENU_BROWSER
{
    public:
        MENU_BROWSER();
        byte getCurrentEntry();
        byte getParent(byte index);
        byte getChild(byte index);
        byte getNext(byte index);
        byte getPrevious(byte index);
        byte getEntryType(byte index);
        const char* getLabel(byte index);
        const char* getRootLabel();
        void gotoChild();
        void gotoParent();
        void gotoNext();
        void gotoPrevious();
        void valid();
        void abort();
        void setRefreshCallback(void (*callback)());
        void setEditCallback(void (*callback)());
        void setPreFunctionCallback(void (*callback)());
        void setPostFunctionCallback(void (*callback)(byte err_num));
        menuBrowserState getState();
        void setState(menuBrowserState _state);
        byte getVariableIndex(byte index);
        MENU_BROWSER_EDIT_PTR getVariableEditFunction(byte index);
        MENU_BROWSER_FUNCTION_PTR getFunction(byte index);
        byte getFunctionIndex(byte index);

    private:
        menuBrowserState state = browserStateBrowsing;
        byte currentEntry = 0;
        void gotoEntry(byte entry);
        void (*refreshCallback)() = NULL;
        void (*editCallback)() = NULL;
        void (*preFunctionCallback)() = NULL;
        void (*postFunctionCallback)(byte err_num) = NULL;
        char buffer[MENU_BROWSER_BUFFER_SIZE];

};

#endif

