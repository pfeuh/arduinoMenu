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

#include <menuBrowser.h>

#define MENU_DISPLAY_NB_ROWS 4
#define MENU_DISPLAY_NB_COLS 20
#define MENU_DISPLAY_NB_DISPLAYED_ITEMS (MENU_DISPLAY_NB_ROWS - 1)

// browser object creation
MENU_BROWSER browser = MENU_BROWSER();

// let's use a tiny stack
byte stack[MENU_DISPLAY_NB_ROWS - 1];
byte stackPtr;
#define stackInit()      stackPtr=0
#define stackPush(value) stack[stackPtr++]=value
#define stackPop()       stack[--stackPtr]=value
#define stackEnd()       (stackPtr!=255)
#define stackSize()      (stackPtr)
#define stackGetCell(index) (stack[value])

long int userValue = 0;

// menu driven variables
byte ChannelIn = 1;
byte ChannelOut = 1;
byte ProgramNumber = 0;
byte Arpeggiator = 0;
byte ClockIn = 0;
byte ClockOut = 0;
byte KeyClick = 0;
byte AudioBeat = 0;
byte SysEx = 1;
int  Transposition = 0;
byte Groove = 2;
byte GateMode = 1;
byte LastStep = 15;
byte CcNum = 20;
byte Test1 = 10;
byte Test2 = 20;
byte Test3 = 30;
byte AppVersion = 101;

void clearDisplay()
{
    Serial.print("\033[2J\n");
}

void browsingSequencer(char car)
{
    switch(car)
    {
        case 'u': // up
            browser.gotoPrevious();
            break;
        case 'd': // down
            browser.gotoNext();
            break;
        case 'r': // right (entering a submenu)
            browser.gotoChild();
            break;
        case 'l': // left (leaving a submenu)
            browser.gotoParent();
            break;
        default:
            break;
    }
}

void editingSequencer(char car)
{
    switch(car)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            userValue = userValue * 10 + car - '0';
            Serial.write(car);
            break;
        case '/':
            browser.setState(browserStateBrowsing);
            break;
        default:
            break;
    }
}

void sequencer()
{
    if(Serial.available())
    {
        byte car = Serial.read();

        switch(browser.getState())
        {
            case browserStateBrowsing:
                browsingSequencer(car);
                break;
            case browserStateEditing:
                editingSequencer(car);
                break;
            case browserStatePrefunction:
                //~ browsingSequencer(car);
                break;
            case browserStatepostfunction:
                //~ browsingSequencer(car);
                break;
            case browserStateUser:
                //~ browsingSequencer(car);
                break;
        }
    }
}

void printTitle(byte index)
{
    clearDisplay();
    index = browser.getParent(index);
    if(index != MENU_BROWSER_NO_ENTRY)
    {
        Serial.print(browser.getLabel(index));
        Serial.write('\n');
    }
    else
        Serial.print(F(" ---*** MENU ***--- \n"));        
}

void printEntriesList(byte index)
{
    byte current_entry_index = index;
    
    for(byte x = 0; x < (MENU_DISPLAY_NB_DISPLAYED_ITEMS - 1); x++)
    {
        if(browser.getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
            current_entry_index = browser.getPrevious(current_entry_index);
        else
            break;
    }
    
    for(byte row_num = 1; row_num <= MENU_DISPLAY_NB_DISPLAYED_ITEMS; row_num++)
    {
        if(current_entry_index != MENU_BROWSER_NO_ENTRY)
        {
            // let's manage first column dedicated to arrows
            if(row_num == 1)
            {
                if(browser.getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    Serial.write('u');
                else
                    Serial.write(' ');
            }
            else if(row_num == MENU_DISPLAY_NB_DISPLAYED_ITEMS)
            {
                if(browser.getNext(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    Serial.write('d');
                else
                    Serial.write(' ');
            }
            else
                Serial.write(' ');

            // let's display entry
            if(current_entry_index == index)
            {
                // this is the selected entry
                Serial.write('<');
                Serial.print(browser.getLabel(current_entry_index));
                Serial.flush();
                Serial.write('>');
            }
            else
            {
                // this entry is not selected
                Serial.write(' ');
                Serial.print(browser.getLabel(current_entry_index));
                Serial.flush();
                Serial.write(' ');
            }
            
        }
        else
        {
            // no more entry to display
            Serial.print(current_entry_index);
            Serial.write('\n');
        }
    
        Serial.write('\n');
        current_entry_index = browser.getNext(current_entry_index);
    }
}

void refreshEditor()
{
    byte index = browser.getCurrentEntry();
    printTitle(index);
    Serial.write('<');
    Serial.print(browser.getLabel(index));
    Serial.write('>');
    Serial.write('\n');
    Serial.print(F("variable "));
    Serial.print(browser.getVariableIndex(index));
    Serial.write('\n');
}

void refreshLauncher()
{
    byte index = browser.getCurrentEntry();
    printTitle(index);
    Serial.write('<');
    Serial.print(browser.getLabel(index));
    Serial.write('>');
    Serial.write('\n');
    Serial.print(F("variable "));
    Serial.print(browser.getVariableIndex(index));
    Serial.write('\n');
}

void refreshBrowser()
{
    byte index = browser.getCurrentEntry();
    printTitle(index);
    printEntriesList(index);
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    Serial.println(F("Test of menuBrowser v" MENU_BROWSER_VERSION));
    Serial.println(F("Compilation : " __DATE__ " " __TIME__));

    browser.begin();
    browser.setRefreshCallback(&refreshBrowser);
    browser.setEditCallback(&refreshEditor);
    browser.setEditCallback(&refreshLauncher);
    browser.setState(browserStateBrowsing);
    
    //~ refreshBrowser();
}

void loop()
{
    sequencer();

        // let's blink status led
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}
