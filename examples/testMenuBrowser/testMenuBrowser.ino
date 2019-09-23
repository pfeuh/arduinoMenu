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
#define stackPop()       stack[stackPtr--]=value
#define stackEnd()       stackPtr!=255
#define stackSize()      stackPtr
#define stackGetCell(index) stack[value]

void sequencer()
{
    if(Serial.available())
        switch(Serial.read())
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

void printTitle(byte index)
{
    Serial.println(F("--------------------"));
    index = browser.getParent(index);
    if(index != MENU_BROWSER_NO_ENTRY)
        Serial.println(browser.getLabel(index));
    else
        Serial.println(F(" ---*** MENU ***--- "));        
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
                if(browser.getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
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
                // this is not
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

void refreshMenu()
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

    //~ for(byte index = 0; index < browser.getNbEntries(); index++)
    //~ {
        //~ Serial.print(browser.getLabel(index));
        //~ Serial.write(' ');
        //~ Serial.print(browser.getBrotherPosition(index));
        //~ Serial.write(' ');
        //~ Serial.println(browser.getNbBrothers(index));
    //~ }
    
    browser.begin();
    browser.setRefreshCallback(&refreshMenu);
    refreshMenu();
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
