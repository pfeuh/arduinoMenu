#ifndef arduinoMenuBrowser_h
#define arduinoMenuBrowser_h

/*
 * file : arduinoMenuBrowser.h
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
#include <Print.h>
#include "menuBrowser.h"
#include "menuInput.h"

#define ARDUINO_MENU_VERSION "1.00"

#define ARDUINO_MENU_NB_ROWS 4
#define ARDUINO_MENU_NB_DISPLAYED_ITEMS (ARDUINO_MENU_NB_ROWS - 1)
#define ARDUINO_MENU_NB_COLS 20
#define ARDUINO_MENU_LAST_COL (ARDUINO_MENU_NB_COLS - 1)

#define ARDUINO_MENU_CHAR_ARROW_BIG_BLACK 0
#define ARDUINO_MENU_CHAR_ARROW_UP 1
#define ARDUINO_MENU_CHAR_ARROW_DOWN 2
#define ARDUINO_MENU_STR_ARROW_UP "\001"
#define ARDUINO_MENU_STR_ARROW_DOWN "\002"
#define ARDUINO_MENU_STR_ARROW_RIGHT "\003"
#define ARDUINO_MENU_STR_ARROW_LEFT "\004"
#define ARDUINO_MENU_CHAR_CR '\r'
#define ARDUINO_MENU_CHAR_LF '\n'
#define ARDUINO_MENU_CHAR_SPACE ' '
#define ARDUINO_MENU_CHAR_GREATER_THAN '>'
#define ARDUINO_MENU_CHAR_LESS_THAN '<'

class ARDUINO_MENU : public Print
{
    public:
        ARDUINO_MENU();
        void begin();
        void sequencer();
        size_t write(uint8_t car);
        void gotoXY(byte x, byte y);
        void clearScreen();
        void setState(menuBrowserState _state);
        void setBlinking(bool flag);
    
        void printTitle(byte index);
        void printSelectedLabel(byte index);
        void printEntriesList(byte index);

        void refreshBrowserScreen();
        void showEditVariableScreen();
        void showPreFunctionScreen();
        void showPostFunctionScreen(byte err_num);
    
        void print_P(const char* str_ptr);

    private:
        MENU_BROWSER* browser;
        MENU_INPUT*   input;
    
        byte x;
        byte y;


    };

// static functions as workaround for class' methods as callbacks
extern      ARDUINO_MENU* ARDUINO_MENU_SINGLETON;
extern void ARDUINO_MENU_refreshBrowserScreen();
extern void ARDUINO_MENU_showEditVariableScreen();
extern void ARDUINO_MENU_showPreFunctionScreen();
extern void ARDUINO_MENU_showPostFunctionScreen(byte err_num);

#endif

