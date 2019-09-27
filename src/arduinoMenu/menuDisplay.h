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
#define MENU_DISPLAY_LAST_COL (MENU_DISPLAY_NB_COLS - 1)

#define MENU_DISPLAY_CHAR_CR '\r'
#define MENU_DISPLAY_CHAR_LF '\n'
#define MENU_DISPLAY_CHAR_ARROW_UP 1
#define MENU_DISPLAY_CHAR_ARROW_DOWN 2

class MENU_DISPLAY
{
    public:
        MENU_DISPLAY();
        void begin();
        void addBrowser(MENU_BROWSER* _browser);
        void refreshBrowserScreen();
        void showEditVariableScreen();
        void showPreFunctionScreen();
        void showPostFunctionScreen(byte err_num);
        void printVariable(char* str_var);
        void write(char car);
        void gotoXY(byte _x, byte _y);
        void clearScreen();
        void setBlinking(bool flag);

        void print(const char* str_ptr);
        void println(const char* str_ptr);
        void print(const __FlashStringHelper* str_ptr);
        void println(const __FlashStringHelper* str_ptr);
    
    
        void print(char value, byte base);
        void println(char value, byte base);
        void print(byte value, byte base);
        void println(byte value, byte base);
    
        void print(int value, byte base);
        void println(int value, byte base);
        void print(long value, byte base);
        void println(long value, byte base);
        void print(unsigned long value, byte base);
        void println(unsigned long value, byte base);
    
    private:
        MENU_BROWSER* browser;
        byte x;
        byte y;

        void printTitle(byte index);
        void printObjectLabel(byte index);
        void printEntriesList(byte index);
        char getHexNibble(byte nibble);
        void printInteger(signed long int value, bool is_signed, byte base);

};

// static functions as workaround for class' methods as callbacks
extern MENU_DISPLAY* MENU_DISPLAY_SINGLETON;
extern void MENU_DISPLAY_refreshBrowserScreen();
extern void MENU_DISPLAY_showEditVariableScreen();
extern void MENU_DISPLAY_showPreFunctionScreen();
extern void MENU_DISPLAY_showPostFunctionScreen(byte err_num);

#endif

