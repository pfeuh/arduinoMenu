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
#include "menuBrowser.h"
#include "menuInput.h"
#include "menuDisplay.h"

#define ARDUINO_MENU_VERSION "1.00"

class ARDUINO_MENU
{
    public:
        ARDUINO_MENU();
        //~ void begin(MENU_BROWSER* _browser, MENU_DISPLAY* _display, MENU_INPUT* _input);
        void begin();
        void sequencer();
        void printVariable(char* str_var);
        void write(char car);
        void print(char* text);
        void gotoXY(byte x, byte y);
        void clearScreen();
        void setState(menuBrowserState _state);

    private:
        MENU_BROWSER* browser;
        MENU_DISPLAY* display;
        MENU_INPUT*   input;

    };

#endif

