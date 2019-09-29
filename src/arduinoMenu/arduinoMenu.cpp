/*
 * file : arduinoMenu.cpp
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

#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C display(0x27, ARDUINO_MENU_NB_COLS, ARDUINO_MENU_NB_ROWS);  // set the LCD address to 0x27 for a 20 chars and 4 lines display

#define peek pgm_read_byte

// lookup table for digits from base 2 to 16
const char ARDUINO_MENU_hexLut[] PROGMEM = "0123456789ABCDEF";

// some menu's messages
const char ARDUINO_MENU_horizontalLine[] PROGMEM = "--------------------\n";
const char ARDUINO_MENU_execMessage[] PROGMEM = " Abort:" ARDUINO_MENU_STR_ARROW_LEFT "  Execute:" ARDUINO_MENU_STR_ARROW_RIGHT"\n";
const char ARDUINO_MENU_errorMessage1[] PROGMEM = "Error #";
const char ARDUINO_MENU_errorMessage2[] PROGMEM =  " Quit:" ARDUINO_MENU_STR_ARROW_RIGHT ARDUINO_MENU_STR_ARROW_LEFT ARDUINO_MENU_STR_ARROW_UP ARDUINO_MENU_STR_ARROW_DOWN;
const char ARDUINO_MENU_successMessage[] PROGMEM = "Successful Quit:" ARDUINO_MENU_STR_ARROW_RIGHT ARDUINO_MENU_STR_ARROW_LEFT ARDUINO_MENU_STR_ARROW_UP ARDUINO_MENU_STR_ARROW_DOWN;

// lookup table of lines' first character location in lcd ram
#define ARDUINO_MENU_SECOND_LINE_OFFSET 0x40
const byte LCD_20X4_IIC_lineOffset[] PROGMEM =
{
    0x00,
    ARDUINO_MENU_SECOND_LINE_OFFSET,
    ARDUINO_MENU_NB_COLS,
    ARDUINO_MENU_SECOND_LINE_OFFSET + ARDUINO_MENU_NB_COLS
};

// custom characters
#define ARDUINO_MENU_CHAR_HEIGHT 8
const char lcdCustomFont[] PROGMEM =
    {
        B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111, // fulll black
        B00000,B00100,B01110,B11111,B00100,B00100,B00000,B00000, // arrow up 
        B00000,B00000,B00100,B00100,B11111,B01110,B00100,B00000, // arrow down
        B00000,B00100,B00110,B11111,B00110,B00100,B00000,B00000, // arrow right
        B00000,B00100,B01100,B11111,B01100,B00100,B00000,B00000, // arrow left
    };    

// browser object creation
MENU_BROWSER menuBrowser = MENU_BROWSER();

// input object creation
MENU_INPUT menuInput = MENU_INPUT();

/****************************************/
/* using static functions as workaround */
/*   for class' methods as callbacks    */
/****************************************/

ARDUINO_MENU* ARDUINO_MENU_SINGLETON;

void ARDUINO_MENU_refreshBrowserScreen()
{
    ARDUINO_MENU_SINGLETON->refreshBrowserScreen();
}

void ARDUINO_MENU_showEditVariableScreen()
{
    ARDUINO_MENU_SINGLETON->showEditVariableScreen();
}

void ARDUINO_MENU_showPreFunctionScreen()
{
    ARDUINO_MENU_SINGLETON->showPreFunctionScreen();
}

void ARDUINO_MENU_showPostFunctionScreen(byte err_num)
{
    ARDUINO_MENU_SINGLETON->showPostFunctionScreen(err_num);
}

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

ARDUINO_MENU::ARDUINO_MENU(): Print()
{
}

void ARDUINO_MENU::begin()
{
    
    ARDUINO_MENU_SINGLETON = this;
    browser = &menuBrowser;
    input   = &menuInput;
    
    display.init();
    display.backlight();
    display.clear();
    display.display();
    // custom characters loading
    const char* char_ptr = lcdCustomFont;
    display.command(LCD_SETCGRAMADDR);
    for(x=0; x<(sizeof(lcdCustomFont)); x++)
        display.write(peek(char_ptr++));
   
    input->addBrowser(browser);

    browser->setRefreshCallback(ARDUINO_MENU_refreshBrowserScreen);
    browser->setEditCallback(ARDUINO_MENU_showEditVariableScreen);
    browser->setPreFunctionCallback(ARDUINO_MENU_showPreFunctionScreen);
    browser->setPostFunctionCallback(ARDUINO_MENU_showPostFunctionScreen);

    browser->setState(browserStateBrowsing);
    // ready to call sequencer periodically
}

void ARDUINO_MENU::sequencer()
{
    input->sequencer();
}

size_t ARDUINO_MENU::write(uint8_t car)
{
    // here is the most important method, each print(something) method uses it
    switch(car)
    {
        case ARDUINO_MENU_CHAR_CR:
            gotoXY(0, y);
            break;
        case ARDUINO_MENU_CHAR_LF:
            gotoXY(0,(y + 1) % ARDUINO_MENU_NB_ROWS);
            break;
        default:
            if(x <= ARDUINO_MENU_LAST_COL)
                display.write(car);
    }
    return 1; // success
}

void ARDUINO_MENU::gotoXY(byte _x, byte _y)
{
    display.command(LCD_SETDDRAMADDR | (_x + pgm_read_byte(LCD_20X4_IIC_lineOffset + _y)));
    x = _x;
    y = _y;
}

void ARDUINO_MENU::clearScreen()
{
    display.clear();
    display.home();
    x = 0;
    y = 0;
}

void ARDUINO_MENU::setBlinking(bool flag)
{
    flag ^= 1;
    print(F("setBlinking\nnot implemented!\n"));
}

void ARDUINO_MENU::setState(menuBrowserState _state)
{
    browser->setState(_state);
}

void ARDUINO_MENU::refreshBrowserScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    printEntriesList(index);
}

void ARDUINO_MENU::showEditVariableScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    print_P(ARDUINO_MENU_horizontalLine);
    printSelectedLabel(index);
    browser->getVariableEditFunction(index)(MENU_BROWSER_DATA_JUST_DISPLAY);
}

void ARDUINO_MENU::showPreFunctionScreen()
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    print_P(ARDUINO_MENU_horizontalLine);
    printSelectedLabel(index);
    print_P(ARDUINO_MENU_execMessage);
}

void ARDUINO_MENU::showPostFunctionScreen(byte err_num)
{
    byte index = browser->getCurrentEntry();
    printTitle(index);
    print_P(ARDUINO_MENU_horizontalLine);
    printSelectedLabel(index);
    if(err_num)
    {
        print_P(ARDUINO_MENU_errorMessage1);
        print(err_num);
        print_P(ARDUINO_MENU_errorMessage2);
    }
    else
        print_P(ARDUINO_MENU_successMessage);
}

void ARDUINO_MENU::printTitle(byte index)
{
    clearScreen();
    
    index = browser->getParent(index);
    if(index != MENU_BROWSER_NO_ENTRY)
        print_P(browser->getLabelAddress(index));
    else
        print_P(browser->getRootLabelAddress());        
    write(ARDUINO_MENU_CHAR_LF);
}

void ARDUINO_MENU::printSelectedLabel(byte index)
{
    write(ARDUINO_MENU_CHAR_LESS_THAN);
    print_P(browser->getLabelAddress(index));
    write(ARDUINO_MENU_CHAR_GREATER_THAN);
    write(ARDUINO_MENU_CHAR_LF);
}

void ARDUINO_MENU::printEntriesList(byte index)
{
    byte current_entry_index = index;
    
    for(byte x = 0; x < (ARDUINO_MENU_NB_DISPLAYED_ITEMS - 1); x++)
    {
        if(browser->getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
            current_entry_index = browser->getPrevious(current_entry_index);
        else
            break;
    }
    
    for(byte row_num = 1; row_num <= ARDUINO_MENU_NB_DISPLAYED_ITEMS; row_num++)
    {
        if(current_entry_index != MENU_BROWSER_NO_ENTRY)
        {
            // let's manage first column dedicated to arrows
            if(row_num == 1)
            {
                if(browser->getPrevious(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    write(ARDUINO_MENU_CHAR_ARROW_UP);
                else
                    write(ARDUINO_MENU_CHAR_SPACE);
            }
            else if(row_num == ARDUINO_MENU_NB_DISPLAYED_ITEMS)
            {
                if(browser->getNext(current_entry_index) != MENU_BROWSER_NO_ENTRY)
                    write(ARDUINO_MENU_CHAR_ARROW_DOWN);
                else
                    write(ARDUINO_MENU_CHAR_SPACE);
            }
            else
                write(ARDUINO_MENU_CHAR_SPACE);

            // let's display entry
            if(current_entry_index == index)
                // this is the selected entry
                printSelectedLabel(current_entry_index);
            else
            {
                // this entry is not selected
                write(ARDUINO_MENU_CHAR_SPACE);
                print_P(browser->getLabelAddress(current_entry_index));
                write(ARDUINO_MENU_CHAR_SPACE);
                write(ARDUINO_MENU_CHAR_LF);
            }
            
            current_entry_index = browser->getNext(current_entry_index);
        }
        else
        {
            // no more entry to display, let's display an empty line
            write(ARDUINO_MENU_CHAR_LF);
        }
    
    }
}

void ARDUINO_MENU::print_P(const char* str_ptr)
{
    while(pgm_read_byte(str_ptr))
        write(pgm_read_byte(str_ptr++));
}
