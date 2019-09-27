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

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 lines display

const char PROGMEM MENU_DISPLAY_HEX_LUT[] = "0123456789ABCDEF";

//~ #define LCD_20X4_IIC_NB_CUSTOM_CHARS 8
#define LCD_20X4_IIC_SECOND_LINE_OFFSET 0x40
//~ #define LCD_20X4_IIC_CHARS_PER_ROW 0x14

const byte LCD_20X4_IIC_lineOffset[] PROGMEM =
{
    0x00,
    LCD_20X4_IIC_SECOND_LINE_OFFSET,
    MENU_DISPLAY_NB_COLS,
    LCD_20X4_IIC_SECOND_LINE_OFFSET + MENU_DISPLAY_NB_COLS
};

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
                    write('u');
                else
                    write(' ');
            }
            else if(row_num == MENU_DISPLAY_NB_DISPLAYED_ITEMS)
            {
                if(browser->getNext(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    write('d');
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
                lcd.print(browser->getLabel(current_entry_index));
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
    return pgm_read_byte(MENU_DISPLAY_HEX_LUT + nibble);
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
        //~ lcd.write(pgm_read_byte(STD_DISPLAY_HEX_LUT + stack[--sp])); 
        write(getHexNibble(stack[--sp]));
        //~ lcd.print(stack[--sp]);
        //~ write(' ');
    }
    //~ write('\n');    
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
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.display();
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
    printObjectLabel(index);
    lcd.print(F("edit variable "));
    lcd.print(browser->getVariableIndex(index));
    lcd.write('\n');
    browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_JUST_DISPLAY);
    //~ lcd.write('\n');
}

void MENU_DISPLAY::showPreFunctionScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printObjectLabel(index);
    lcd.print(F("Execute or abort"));
    lcd.write('\n');
    lcd.write('\n');
}

void MENU_DISPLAY::showPostFunctionScreen(byte err_num)
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printObjectLabel(index);
    lcd.write('\n');
    if(err_num)
    {
        lcd.print(F("Error #"));
        lcd.print(err_num);
    }
    else
        lcd.print(F("Function executed"));
    lcd.write('\n');
}

void MENU_DISPLAY::printVariable(char* str_var)
{
    lcd.print(str_var);
    lcd.write('\n');
}

void MENU_DISPLAY::write(char car)
{
    switch(car)
    {
        case MENU_DISPLAY_CHAR_CR:
            gotoXY(0, y);
            Serial.println(F("CR"));
            break;
        case MENU_DISPLAY_CHAR_LF:
            Serial.println(F("LF"));
            gotoXY(0,(y + 1) % MENU_DISPLAY_NB_ROWS);
            break;
        default:
            if(x <= MENU_DISPLAY_LAST_COL)
            {
                lcd.write(car);
                x++;
                Serial.write(car);
            }
    }
}

void MENU_DISPLAY::gotoXY(byte x, byte y)
{
    lcd.command(LCD_SETDDRAMADDR | (x + pgm_read_byte(LCD_20X4_IIC_lineOffset + y)));
}

void MENU_DISPLAY::clearScreen()
{
    lcd.print(F("clearScreen\n\n\n\n\n\n\n\n\n\n"));
}

void MENU_DISPLAY::setBlinking(bool flag)
{
    lcd.print(F("setBlinking("));
    lcd.print(flag);
    lcd.write(')');
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

