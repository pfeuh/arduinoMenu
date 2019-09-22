#!/usr/bin/python
# -*- coding: utf-8 -*-

# 
# file : menuGenerator.h
# Copyright (c) pfeuh <ze.pfeuh@gmail>
# All rights reserved.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

import menuParser
from menuParser import EMPTY, LF
import sys
import os
sys.stdout.write(sys.version + "\n")

NO_ENTRY = "MENU_BROWSER_NO_ENTRY"
ITEM_TYPE_TABLE = {"menu":"menuTypeMenu", "variable":"menuTypeVariable", "function":"menuTypeFunction"}
HEADER_PATTERN_NAME = "headerPattern.txt"
SIZE_OF_FUNCTION_POINTER = 2
SIZE_OF_CHAR_POINTER = 2

def pascalize(text):
    return text[0].upper() + text[1:]

def formatIndex(index):
    if index != None:
        text = "%u"%index
    else:
        text = NO_ENTRY
    return text

def addStatLine(text, value):
    global statText, statValue
    statText += "// Table of %-20s%05u\n"%(text, value)
    statValue += value

def getItemTypeTable(menu):
    text = "const enum menuOptionType itemTypeTable[%u] PROGMEM = \n{\n"%(len(menu.getObjects()))
    for item in menu.getObjects():
        itype = ITEM_TYPE_TABLE[item.getTag()]
        text += "   /* %03u */ %s,\n"%(item.getIndex(), itype)
    text += "};\n\n"
    addStatLine("types", len(menu.getObjects()))
    return text

def getDeclareFunctionsCode(menu):
    text = "// functions suppposed ready to execute\n"
    for function in menu.getFunctions():
        text += "extern void %s();\n"%(function.getCname())
    text += LF
    return text

def getFunctionsCode(menu):
    text = EMPTY
    for function in menu.getFunctions():
        text += "void %s()\n{\n}\n\n"%(function.getCname())
    return text

def getDeclareVariablesCode(menu):
    text = "// edit & display functions of variables suppposed ready to execute\n"
    for variable in menu.getVariables():
        text += "extern void edit%s();\n"%(pascalize(variable.getCname()))
        text += "extern void display%s();\n"%(pascalize(variable.getCname()))
    text += LF
    return text

def getVariablesCode(menu):
    text = EMPTY
    for variable in menu.getVariables():
        text += "extern void edit%s()\n{\n}\n\n"%(pascalize(variable.getCname()))
        text += "extern void display%s()\n{\n}\n\n"%(pascalize(variable.getCname()))
    return text

def getFamilyTable(menu, hook, label):
    text = "const byte %sTable[%u] PROGMEM = \n{\n"%(label, len(menu.getObjects()))
    for item in menu.getObjects():
        text += "   /* %03u */ %s,\n"%(item.getIndex(), formatIndex(hook(item)))
    text += "};\n\n"
    addStatLine("%s"%label, len(menu.getObjects()))
    return text

def getParentTableCode(menu):
    return getFamilyTable(menu, menuParser.MENU_ITEM.getParent, "parent")

def getChildTableCode(menu):
    return getFamilyTable(menu, menuParser.MENU_ITEM.getChild, "child")

def getNextTableCode(menu):
    return getFamilyTable(menu, menuParser.MENU_ITEM.getNext, "next")

def getPreviousTableCode(menu):
    return getFamilyTable(menu, menuParser.MENU_ITEM.getPrevious, "previous")

def getLabelTableCode(menu):
    text = EMPTY
    labels = []
    total = 0
    for item in menu.getObjects():
        label = "label%s"%(pascalize(item.getCname()))
        text += '/* %3u */ const char %s[] PROGMEM = "%s";\n'%(item.getIndex(), label, item.getLabel())
        total += len(item.getLabel()) + 1
        labels.append(label)
    text += "\nconst char *const labelsTable[] PROGMEM =\n{\n"
    addStatLine("labels", total)
    for index, label in enumerate(labels):
        text += "    /* %3u */ %s,\n"%(index, label)
    text += "};\n\n"
    addStatLine("pointers to labels", len(menu.getObjects()) * SIZE_OF_CHAR_POINTER)
    return text

def getCallbackTable(objects, table_prefix, function_prefix):
    text = "const PROGMEM MENU_BROWSER_FUNCTION_PTR %sFunctionsTable[%s] = \n{\n"%(table_prefix, len(objects))
    for index, item in enumerate(objects):
        if function_prefix == EMPTY:
            fname = item.getCname()
        else:
            fname = function_prefix + pascalize(item.getCname())
        text += '    /* %3u %3u */ &%s,\n'%(item.getIndex(), item.getId(), fname)
    text += "};\n\n"
    if table_prefix == "exec":
        table_prefix = "function"
    addStatLine("%s callbacks"%table_prefix, len(menu.getObjects()) * SIZE_OF_FUNCTION_POINTER)
    return text

def getFunctionsTableCode(menu):
    return getCallbackTable(menu.getFunctions(), "exec", EMPTY)

def getEditFunctionsTableCode(menu):
    return getCallbackTable(menu.getVariables(), "edit", "edit")

def getDisplayFunctionsTableCode(menu):
    return getCallbackTable(menu.getVariables(), "display", "display")

def getHeader(fname, user_name, mail_address):
    with open(HEADER_PATTERN_NAME) as fp:
        text = fp.read(-1)
    if fname == None:
        word = "stdout"
    else:
        word = os.path.splitext(os.path.basename(fname))[0]
    text = text.replace("#fname#", word)
    
    word = user_name
    if word == None:
        word = "unknown"
    text = text.replace("#user#", word)
    word = mail_address
    if word == None:
        word = ""
    text = text.replace("#mail#", "<%s>"%word)
    return  text

def makeMenuDataFile(menu, fname, user_name, mail_address, include_empty_functions=False):
    text   = getDeclareFunctionsCode(menu)
    if include_empty_functions:
        text += getFunctionsCode(menu)
    text += getDeclareVariablesCode(menu)
    if include_empty_functions:
        text += getVariablesCode(menu)
    text += getParentTableCode(menu)
    text += getChildTableCode(menu)
    text += getNextTableCode(menu)
    text += getPreviousTableCode(menu)
    text += getLabelTableCode(menu)
    text += getFunctionsTableCode(menu)
    text += getEditFunctionsTableCode(menu)
    text += getDisplayFunctionsTableCode(menu)
    text += getItemTypeTable(menu)
    text += "#endif\n\n"
    
    start_of_text = getHeader(fname, user_name, mail_address)
    start_of_text +=  statText
    start_of_text +=  "// ----------------------------------\n"
    start_of_text +=  "// TOTAL                        %05u\n\n"%statValue

    if fname != None:
        with open(fname, "w") as fp:
            fp.write(start_of_text + text)
    else:
        sys.stdout.write(start_of_text + text)

if __name__ == "__main__":
    
    import time
    start = time.time()
    
    inFname = None
    outFname = None
    userName = None
    mailAddress = None
    createEmptyFunction = 0
    for index, value in enumerate(sys.argv):
        if value == "-i":
            inFname = sys.argv[index+1]
        elif value == "-o":
            outFname = sys.argv[index+1]
        elif value == "-u":
            userName = sys.argv[index+1]
        elif value == "-m":
            mailAddress = sys.argv[index+1]
        elif value == "-c":
            createEmptyFunction = eval(sys.argv[index+1])

    sys.stdout.write("Current path        = %s\n"%os.path.dirname(os.path.abspath(__file__)))
    sys.stdout.write("inFname             = %s\n"%str(inFname))
    sys.stdout.write("outFname            = %s\n"%str(outFname))
    sys.stdout.write("userName            = %s\n"%str(userName))
    sys.stdout.write("mailAddress         = %s\n"%str(mailAddress))
    sys.stdout.write("createEmptyFunction = %s\n"%str(createEmptyFunction))

    statText = EMPTY
    statValue = 0

    menu = menuParser.PARSER_MENU(inFname)
    makeMenuDataFile(menu, outFname, userName, mailAddress, include_empty_functions=False)
    
    stop = time.time()
    sys.stdout.write("Job done in %.3f second(s)!\n"%(stop-start))
    
