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
#include "menuData.h"

#define peek pgm_read_byte
#define wpeek pgm_read_word

// menu containing browser, display and input objects
ARDUINO_MENU menu = ARDUINO_MENU();

const char modeOmniMessage[] PROGMEM = "Mode omni";

// menu driven variables
byte channelIn = 1;
byte channelOut = 16;
float tempo = 120.0;
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
unsigned long livingValue1 = 0;
signed long livingValue2 = 0;

#include "sharedFunctions.h"

void editChannelIn(byte direction)
{
    const char message[] = "Mode is Omni";
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
        menu.printVariable(channelIn);
    else
        //~ menu.printVariable_P(modeOmniMessage);
        menu.printVariable(message);
}

void editChannelOut(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
            channelOut +=1;
    else 
        if(direction == MENU_BROWSER_DATA_DECREASE)
                channelOut -=1;
    menu.printVariable(channelOut);
}

void editTempo(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tempo +=0.5;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tempo -=0.5;
    menu.printVariable(tempo, 5);
}

void editAppVersion(byte direction)
{
    if(direction){};// avoiding compiler warning "unused parameter"
    //~ menu.printVariable_P(appVersion);
    menu.printVariable(F("YAAMM 1.01"));
}

void editCompilationTimestamp(byte direction)
{
    if(direction){};// avoiding compiler warning "unused parameter"
    menu.printVariable_P(compilationTimestamp);
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

void print_P(word strptr)
{
    while(peek(strptr))
        Serial.write(peek(strptr++));
}

void setup()
{    
    pinMode(LED_BUILTIN, OUTPUT);

    #if(MENU_INPUT_DEVICE == MENU_INPUT_DEVICE_SERIAL || MENU_OUTPUT_DEVICE == MENU_OUTPUT_DEVICE_SERIAL)
        Serial.begin(9600);
    #endif

    #if(MENU_OUTPUT_DEVICE == MENU_INPUT_DEVICE_SERIAL)
        Serial.println(F("Sample menu demo v" MENU_BROWSER_VERSION));
        Serial.println(F("Compilation : " __DATE__ " " __TIME__));
        Serial.println(F("input:"));
        Serial.print(MENU_INPUT_DEVICE);
        Serial.print(F(" output:"));
        Serial.println(MENU_OUTPUT_DEVICE);
    #endif
    
    menu.begin(MENU_BROWSER_NB_ENTRIES, MENU_DATA_tables);
    Serial.begin(9600);
}

void loop()
{
    menu.sequencer();

    livingValue1++;
    livingValue2--;
    
    // let's blink status led
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}
