#ifndef menuInput_h
#define menuInput_h

/*
 * file : menuInput.h
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

#define MENU_INPUT_VERSION "1.00"
#include "menuBrowser.h"

#define MENU_INPUT_CHAR_CMD_UP    'u'
#define MENU_INPUT_CHAR_CMD_DOWN  'd'
#define MENU_INPUT_CHAR_CMD_RIGHT 'r'
#define MENU_INPUT_CHAR_CMD_LEFT  'l'

class MENU_INPUT
{
    public:
        MENU_INPUT();
        byte available();
        byte read();
        void addBrowser(MENU_BROWSER* _browser);
        void browsingSequencer(char car);
        void editingSequencer(char car);
        void preFunctionSequencer(char car);
        void postFunctionSequencer(char car);
        void sequencer();

    private:
        MENU_BROWSER* browser;

};

#endif

