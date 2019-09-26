#ifndef menuDisplay_h
#define menuDisplay_h

/*
 * file : menuDisplay.h
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
#include "menuBrowser.h"

#define MENU_DISPLAY_VERSION "1.00"

#define MENU_DISPLAY_NB_ROWS 4
#define MENU_DISPLAY_NB_DISPLAYED_ITEMS (MENU_DISPLAY_NB_ROWS - 1)
#define MENU_DISPLAY_NB_COLS 20

class MENU_DISPLAY
{
    public:
        MENU_DISPLAY();
        void addBrowser(MENU_BROWSER* _browser);
        void refreshBrowserScreen();
        void showEditVariableScreen();
        void showPreFunctionScreen();
        void showPostFunctionScreen(byte err_num);
        void printVariable(char* str_var);
        void print(char* text);

    private:
        MENU_BROWSER* browser;

        void printTitle(byte index);
        void printObjectLabel(byte index);
        void printEntriesList(byte index);

};

// static functions as workaround for class' methods as callbacks
extern MENU_DISPLAY* MENU_DISPLAY_SINGLETON;
extern void MENU_DISPLAY_refreshBrowserScreen();
extern void MENU_DISPLAY_showEditVariableScreen();
extern void MENU_DISPLAY_showPreFunctionScreen();
extern void MENU_DISPLAY_showPostFunctionScreen(byte err_num);

#endif

