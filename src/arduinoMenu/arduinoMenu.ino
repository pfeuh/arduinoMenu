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

#include "arduinoMenu.h"

// menu containing browser, display and input objects
ARDUINO_MENU menu = ARDUINO_MENU();

const char modeOmniMessage[] PROGMEM = "Mode omni";

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
const char appVersion[] PROGMEM = "YAMM " ARDUINO_MENU_VERSION;
const char compilationTimestamp[] PROGMEM = __DATE__ " " __TIME__;

#include "sharedFunctions.h"

void editChannelIn(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
    {
        if(channelIn < 16)
            channelIn +=1;
    }
    else 
        if(direction == MENU_BROWSER_DATA_DECREASE)
        {
            if(channelIn)
                channelIn -=1;
        }
    if(channelIn)
        menu.print(channelIn);
    else
        menu.print_P(modeOmniMessage);
}

void editAppVersion(byte direction)
{
    if(direction){};// avoiding compiler warning "unused parameter"
    menu.print_P(appVersion);
}

void editCompilationTimestamp(byte direction)
{
    if(direction){};// avoiding compiler warning "unused parameter"
    menu.print_P(compilationTimestamp);
}

byte test1()
{
    return 1;
}

byte test2()
{
    return 2;
}

byte test3()
{
    return 3;
}


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    //~ Serial.begin(9600);
    //~ Serial.println(F("Test of menuBrowser v" MENU_BROWSER_VERSION));
    //~ Serial.println(F("Compilation : " __DATE__ " " __TIME__));

    menu.begin();
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
