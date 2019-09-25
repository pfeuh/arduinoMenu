/*
 * file : testMenuBrowser.ino
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

// This program shows only the menu browser using Serial interface.
// OK to switch to a joystick, a set of buttons or something else.

#include "arduinoMenu.h"

// browser object creation
MENU_BROWSER browser = MENU_BROWSER();

// browser object creation
MENU_DISPLAY display = MENU_DISPLAY();

// browser object creation
MENU_INPUT input = MENU_INPUT();

// menu containing browser, display and input objects
ARDUINO_MENU menu = ARDUINO_MENU();

// menu driven variables
byte channelIn = 1;
byte channelOut = 16;
byte programNumber = 0;
byte arpeggiator = 0;
byte clockIn = 0;
byte clockOut = 0;
byte keyClick = 0;
byte audioBeat = 0;
byte sysEx = 1;
int  transposition = 0;
byte groove = 2;
byte gateMode = 1;
byte lastStep = 15;
byte ccNum = 20;
byte test1 = 10;
byte test2 = 20;
byte test3 = 30;
byte appVersion = 101;

char printBuffer[MENU_DISPLAY_NB_COLS];
char stack[MENU_DISPLAY_NB_COLS];
byte sp;

char* integer2str(signed long int value, bool is_signed)
{
    Serial.print(value);
    Serial.print(F(" -> "));

    bool is_negative = false;
    sp = 0;

    if(is_signed)
        if(value & 0x80000000)
        {
            value = 0 - value;
            is_negative = true;
        }

    // let's push digits in the stack
    while(value)
    {
        stack[sp++] = value % 10;
        value = value / 10;
    }
    
    // with previous algorithm value zero has no digit... Let's give it one.
    if(!sp)
        stack[sp++] = 0;
    
    // let's prepare the returned string
    char* target = printBuffer;
    
    // a negative number must be preceded by a minus
    if(is_negative)
        *target++ = '-';
    
    // let's pop digits from the stack
    while(sp)
        *target++ = stack[--sp] + '0';
    
    // don't forget the string terminator
    *target = '\0';
    
    return printBuffer;
}

// this include binds the menu and the
// functions used by arduinoMenu
#include "sharedFunctions.h"

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    Serial.println(F("Test of menuBrowser v" MENU_BROWSER_VERSION));
    Serial.println(F("Compilation : " __DATE__ " " __TIME__));

    menu.begin(&browser, &display, &input);
}

void loop()
{
    menu.sequencer();

    // let's blink status led
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}
