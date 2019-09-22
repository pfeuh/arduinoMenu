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

// this program test only the menu browser using Serial interface

#include <menuBrowser.h>

// browser object creation
MENU_BROWSER browser = MENU_BROWSER();

void sequencer()
{
    if(Serial.available())
        switch(Serial.read())
        {
            case 'u': // up
                browser.gotoParent();
                break;
            case 'l': // left
                browser.gotoPrevious();
                break;
            case 'r': // right
                browser.gotoNext();
                break;
            case 'd': // down
                browser.gotoChild();
                break;
            case 'v': // valid
                browser.valid();
                break;
            case 'e': // escape
                browser.abort();
                break;
            default:
                break;
        }
}

void refreshMenu()
{
    Serial.println(browser.getLabel(browser.getCurrentEntry()));
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    Serial.println(F("Test of menuBrowser v" MENU_BROWSER_VERSION));
    Serial.println(F("Compilation : " __DATE__ " " __TIME__));

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
