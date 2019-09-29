/*
 * file : menuDisplay.cpp
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "menuDisplay.h"

#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, MENU_DISPLAY_NB_COLS, MENU_DISPLAY_NB_ROWS);  // set the LCD address to 0x27 for a 20 chars and 4 lines display

// lookup table for digits from base 2 to 16
const char PROGMEM MENU_DISPLAY_hexLut[] = "0123456789ABCDEF";

const char MENU_DISPLAY_horizontalLine[] PROGMEM = "--------------------\n";
const char MENU_DISPLAY_emptyLine[]      PROGMEM = "                    \n";

// lookup table of lines' first character location in lcd ram
#define MENU_DISPLAY_SECOND_LINE_OFFSET 0x40
const byte LCD_20X4_IIC_lineOffset[] PROGMEM =
{
    0x00,
    MENU_DISPLAY_SECOND_LINE_OFFSET,
    MENU_DISPLAY_NB_COLS,
    MENU_DISPLAY_SECOND_LINE_OFFSET + MENU_DISPLAY_NB_COLS
};

// custom characters
#define MENU_DISPLAY_CHAR_HEIGHT 8

/****************************************/
/* using static functions as workaround */
/*   for class' methods as callbacks    */
/****************************************/

MENU_DISPLAY* MENU_DISPLAY_SINGLETON;

void MENU_DISPLAY_refreshBrowserScreen()
{
    MENU_DISPLAY_SINGLETON->refreshBrowserScreen();
}

void MENU_DISPLAY_showEditVariableScreen()
{
    MENU_DISPLAY_SINGLETON->showEditVariableScreen();
}

void MENU_DISPLAY_showPreFunctionScreen()
{
    MENU_DISPLAY_SINGLETON->showPreFunctionScreen();
}

void MENU_DISPLAY_showPostFunctionScreen(byte err_num)
{
    MENU_DISPLAY_SINGLETON->showPostFunctionScreen(err_num);
}

/*******************/
/* Private methods */
/*******************/

void MENU_DISPLAY::printTitle(byte index)
{
    clearScreen();
    
    index = browser->getParent(index);
    if(index != MENU_BROWSER_NO_ENTRY)
        print(browser->getLabel(index));
    else
        print(browser->getRootLabel());        
    write('\n');
}

void MENU_DISPLAY::printObjectLabel(byte index)
{
    write('<');
    print(browser->getLabel(index));
    write('>');
    write('\n');
    //~ lcd.write('<');
    //~ lcd.print(browser->getLabel(index));
    //~ lcd.write('>');
    //~ lcd.write('\n');
}

void MENU_DISPLAY::printEntriesList(byte index)
{
    byte current_entry_index = index;
    
    for(byte x = 0; x < (MENU_DISPLAY_NB_DISPLAYED_ITEMS - 1); x++)
    {
        if(browser->getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
            current_entry_index = browser->getPrevious(current_entry_index);
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
                if(browser->getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    write(MENU_DISPLAY_CHAR_ARROW_UP);
                else
                    write(' ');
            }
            else if(row_num == MENU_DISPLAY_NB_DISPLAYED_ITEMS)
            {
                if(browser->getNext(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    write(MENU_DISPLAY_CHAR_ARROW_DOWN);
                else
                    write(' ');
            }
            else
                write(' ');

            // let's display entry
            if(current_entry_index == index)
            {
                // this is the selected entry
                write('<');
                print(browser->getLabel(current_entry_index));
                write('>');
            }
            else
            {
                // this entry is not selected
                write(' ');
                print(browser->getLabel(current_entry_index));
                write(' ');
            }
            
            write('\n');
            current_entry_index = browser->getNext(current_entry_index);
        }
        else
        {
            // no more entry to display, let's display an empty line
            lcd.write('\n');
        }
    
    }
}

char MENU_DISPLAY::getHexNibble(byte nibble)
{
    return pgm_read_byte(MENU_DISPLAY_hexLut + nibble);
}

void MENU_DISPLAY::printInteger(signed long int value, bool is_signed, byte base)
{
    bool is_negative = false;
    // LIFO is the key...
    // in base 2, 32 bits need 32 digits... plus one for minus symbol
    byte stack[33];
    byte sp = 0;

    if(is_signed && (base == 10))// only base 10 uses '-'
        if(value & 0x80000000)
        {
            value = 0 - value;
            is_negative = true;
        }

    // let's push digits on the stack
    while(value)
    {
        stack[sp++] = value % base;
        value = value / base;
    }
    
    // with previous algorithm value zero has no digit... Let's give it one.
    if(!sp)
        stack[sp++] = 0;
    
    // a negative number must be preceded by a minus
    if(is_negative)
        write('-');    
    
    // let's pop digits from the stack
    while(sp)
    {
        write(getHexNibble(stack[--sp]));
    }
}

/******************/
/* Public methods */
/******************/

MENU_DISPLAY::MENU_DISPLAY()
{
    MENU_DISPLAY_SINGLETON = this;
}

void MENU_DISPLAY::begin()
{
    // TODO: pass these 16 bytes in ROM
    byte glypheArrowUp[] = {B00000,B00100,B01110,B11111,B00100,B00100,B00000,B00000};
    byte glypheArrowDown[] = {B00000,B00000,B00100,B00100,B11111,B01110,B00100,B00000};
    
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.display();
    lcd.createChar(MENU_DISPLAY_CHAR_ARROW_UP, glypheArrowUp);
    lcd.createChar(MENU_DISPLAY_CHAR_ARROW_DOWN, glypheArrowDown);
}

void MENU_DISPLAY::addBrowser(MENU_BROWSER* _browser)
{
    browser = _browser;
}

void MENU_DISPLAY::refreshBrowserScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printEntriesList(index);
}

void MENU_DISPLAY::showEditVariableScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    print(MENU_DISPLAY_horizontalLine);
    printObjectLabel(index);
    browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_JUST_DISPLAY);
}

void MENU_DISPLAY::showPreFunctionScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    print(MENU_DISPLAY_horizontalLine);
    printObjectLabel(index);
    print(F("Execute or abort"));
}

void MENU_DISPLAY::showPostFunctionScreen(byte err_num)
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    print(MENU_DISPLAY_horizontalLine);
    printObjectLabel(index);
    if(err_num)
    {
        print(F("Error #"));
        print((int)err_num, DEC);
    }
    else
        lcd.print(F("Successful"));
}

void MENU_DISPLAY::printVariable(char* str_var)
{
    gotoXY(0, 3);
    print(MENU_DISPLAY_emptyLine);
    gotoXY(0, 3);
    print(str_var);
}

void MENU_DISPLAY::write(char car)
{
    switch(car)
    {
        case MENU_DISPLAY_CHAR_CR:
            Serial.println(F("CR"));
            gotoXY(0, y);
            break;
        case MENU_DISPLAY_CHAR_LF:
            Serial.println(F("LF"));
            gotoXY(0,(y + 1) % MENU_DISPLAY_NB_ROWS);
            break;
        default:
            if(x <= MENU_DISPLAY_LAST_COL)
            {
                Serial.write(car);
                lcd.write(car);
                x++;
            }
            else
            {
                Serial.write('(');
                Serial.write(car);
                Serial.write(')');
            }
    }
}

void MENU_DISPLAY::gotoXY(byte _x, byte _y)
{
    lcd.command(LCD_SETDDRAMADDR | (_x + pgm_read_byte(LCD_20X4_IIC_lineOffset + _y)));
    x = _x;
    y = _y;
}

void MENU_DISPLAY::clearScreen()
{
    lcd.clear();
    lcd.home();
    x = 0;
    y = 0;
}

void MENU_DISPLAY::setBlinking(bool flag)
{
    Serial.print(F("setBlinking("));
    Serial.print(flag);
    Serial.print(F(")\n"));
}

void MENU_DISPLAY::print(const char* str_ptr)
{
    while(*str_ptr)
        write(*str_ptr++);
}

void MENU_DISPLAY::println(const char* str_ptr)
{
    print(str_ptr);
    write('\n');
}

void MENU_DISPLAY::print(const __FlashStringHelper* str_ptr)
{
    word ptr = (word)str_ptr;
    char c = pgm_read_byte(ptr++);
    
    while(c)
    {
        write(c);
        c = pgm_read_byte(ptr++);
    }
}

void MENU_DISPLAY::println(const __FlashStringHelper* str_ptr)
{
    print(str_ptr);
    write('\n');
}

void MENU_DISPLAY::print(int value, byte base=10)
{
    printInteger(value, true, base);
}

void MENU_DISPLAY::println(int value, byte base=10)
{
    print(value, base);
    write('\n');
}

void MENU_DISPLAY::print(long value, byte base=10)
{
    printInteger(value, true, base);
}

void MENU_DISPLAY::println(long value, byte base=10)
{
    print(value, base);
    write('\n');
}

void MENU_DISPLAY::print(unsigned long value, byte base=10)
{
    printInteger(value, false, base);
}

void MENU_DISPLAY::println(unsigned long value, byte base=10)
{
    print(value, base);
    write('\n');
}

