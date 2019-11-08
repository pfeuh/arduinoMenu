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

#include <arduinoMenu.h>
#include "menuData.h"

#define peek pgm_read_byte
#define wpeek pgm_read_word

#define ENCODER 1
#define KEYBOARD 2
#define INPUT_DEVICE ENCODER
//~ #define INPUT_DEVICE KEYBOARD

#if(INPUT_DEVICE == ENCODER)
MENU_ENCODER encoder = MENU_ENCODER(4, 3, 2);
#endif

#if(MENU_INPUT_DEVICE == KEYBOARD)
int  columns[] = {26, 24, 22, 36};
int  rows[]    = {34, 32, 30, 28};
const char codes[] PROGMEM =
{
    '1', '2', '3', MENU_INPUT_CHAR_CMD_UP,
    '4', '5', '6', MENU_INPUT_CHAR_CMD_RIGHT,
    '7', '8', '9', MENU_INPUT_CHAR_CMD_LEFT,
    '*', '0', '#', MENU_INPUT_CHAR_CMD_DOWN,
};
MENU_KEYBOARD keyboard = MENU_KEYBOARD(columns, sizeof(columns) / sizeof(int), rows, sizeof(rows) / sizeof(int), codes);
#endif

// lcd for menu and user
// set the LCD address to 0x27 for a 20 chars and 4 lines display
LiquidCrystal_I2C display(0x27, 20, 4);

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
        menu.printVariable_P(modeOmniMessage);
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

    Serial.begin(9600);
    Serial.println(F("Sample menu demo v" MENU_BROWSER_VERSION));
    Serial.println(F("Compilation : " __DATE__ " " __TIME__));
    Serial.print(F("INPUT_DEVICE : "));
    Serial.println(INPUT_DEVICE);
    
    #if(INPUT_DEVICE == ENCODER)
    menu.begin(MENU_BROWSER_NB_ENTRIES, MENU_DATA_tables, &display, &encoder);
    #endif
    
    #if(INPUT_DEVICE == KEYBOARD)
    menu.begin(MENU_BROWSER_NB_ENTRIES, MENU_DATA_tables, &display, &keyboard);
    #endif
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
