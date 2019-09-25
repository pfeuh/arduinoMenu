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
NB_ENTRIES = "MENU_BROWSER_NB_ENTRIES"
NB_VARIABLES = "MENU_BROWSER_NB_VARIABLES"
NB_FUNCTIONS = "MENU_BROWSER_NB_FUNCTIONS"
ITEM_TYPE_TABLE = {"menu":"menuTypeMenu", "variable":"menuTypeVariable", "function":"menuTypeFunction"}
HEADER_PATTERN_NAME = "headerPattern.txt"
SIZE_OF_FUNCTION_POINTER = 2
SIZE_OF_CHAR_POINTER = 2
FUNCTION_PTR = "MENU_BROWSER_FUNCTION_PTR"
EDIT_PTR = "MENU_BROWSER_EDIT_PTR"

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
        text += "extern void edit%s(byte direction);\n"%(pascalize(variable.getCname()))
    text += LF
    return text

def getVariablesCode(menu, function_template):
    text = EMPTY
    with open(function_template) as fp:
        pattern = fp.read(-1)
    for variable in menu.getVariables():
        varname = variable.getCname()
        funcname = "edit" + pascalize(varname)
        current_pattern = pattern[:]
        current_pattern = current_pattern.replace("#funcname#", funcname)
        current_pattern = current_pattern.replace("#varname#", varname)
        text += current_pattern
    return text

def getFamilyTable(menu, hook, label):
    text = "const byte %sTable[%s] PROGMEM = \n{\n"%(label, NB_ENTRIES)
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
    text += "\nconst char *const labelsTable[%s] PROGMEM =\n{\n"%NB_ENTRIES
    addStatLine("labels", total)
    for index, label in enumerate(labels):
        text += "    /* %3u */ %s,\n"%(index, label)
    text += "};\n\n"
    addStatLine("pointers to labels", len(menu.getObjects()) * SIZE_OF_CHAR_POINTER)
    return text

def getCallbackTable(objects, table_prefix, function_prefix, ptr_type):
    text = "const PROGMEM %s %sFunctionsTable[%s] = \n{\n"%(ptr_type, table_prefix, len(objects))
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
    return getCallbackTable(menu.getFunctions(), "exec", EMPTY, FUNCTION_PTR)

def getEditFunctionsTableCode(menu):
    return getCallbackTable(menu.getVariables(), "edit", "edit", EDIT_PTR)

def getTimestamp():
    now = time.localtime()
    return '"%04u/%02u/%02u %02u:%02u:%02u"'%(now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec)

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
    text = text.replace("#timestamp#", "%s"%getTimestamp())
    
    return  text

def makeMenuDataFile(menu, fname, user_name, mail_address, edit_function_template, empty_functions_fname=None):
    text   = getDeclareFunctionsCode(menu)
    text += getDeclareVariablesCode(menu)
    text += getParentTableCode(menu)
    text += getChildTableCode(menu)
    text += getNextTableCode(menu)
    text += getPreviousTableCode(menu)
    text += getLabelTableCode(menu)
    text += getFunctionsTableCode(menu)
    text += getEditFunctionsTableCode(menu)
    text += getItemTypeTable(menu)
    text += "#endif\n\n"
    
    start_of_text = getHeader(fname, user_name, mail_address)
    start_of_text +=  "#define MENU_BROWSER_NB_ENTRIES %u\n"%len(menu.getObjects())
    start_of_text +=  "#define MENU_BROWSER_NB_VARIABLES %u\n"%len(menu.getVariables())
    start_of_text +=  "#define MENU_BROWSER_NB_FUNCTIONS %u\n\n"%len(menu.getFunctions())
    
    start_of_text +=  statText
    start_of_text +=  "// ----------------------------------\n"
    start_of_text +=  "// TOTAL                        %05u\n\n"%statValue

    if fname != None:
        with open(fname, "w") as fp:
            fp.write(start_of_text + text)
    else:
        sys.stdout.write(start_of_text + text)
        
    if empty_functions_fname != None:
        text  = getVariablesCode(menu, edit_function_template)
        text += getFunctionsCode(menu)
        with open(empty_functions_fname, "w") as fp:
            fp.write(text)

def getParams():
    conf_name = os.path.splitext(sys.argv[0])[0] + ".cfg"
    if len(sys.argv) > 1:
        conf_name = sys.argv[1]
    sys.stdout.write("configuration file %s\n"%conf_name)
    
    projectPath = None
    xmlFname = None
    create_empty_function = False
    userName = None
    userMail = None
    emptyFunctionsFname = None
    templateEditFname = "./defaultTEF.txt"
    
    with open(conf_name) as fp:
        lines = [line.strip() for line in fp.readlines()]
    for line in lines:
        if line .startswith("#"):
            continue
        elif line.startswith("-prj "):
            projectPath = line[len("-prj "):]
        elif line.startswith("-xml "):
            xmlFname = line[len("-xml "):]
        elif line == "-csf":
            create_empty_function = True
        elif line.startswith("-user "):
            userName = line[len("-user "):]
        elif line.startswith("-mail "):
            userMail = line[len("-mail "):]
        elif line.startswith("-tef "):
            templateEditFname = line[len("-tef "):].replace("\\", "/")
            
    if projectPath == None:
        raise Exception("The project path is missing!\n")
    elif xmlFname == None:
        xmlFname = os.path.join(projectPath, "menuTree.xml")
    projectPath = projectPath.replace("\\", "/")
    xmlFname = xmlFname.replace("\\", "/")
    if not os.path.exists(xmlFname):
        raise Exception("%s not found!\n"%str(xmlFname))
    if templateEditFname != None:
        if not os.path.isfile(templateEditFname):
            raise Exception("%s not found!\n"%str(templateEditFname))
    
    menuDataFname = os.path.join(projectPath, "menuData.h").replace("\\", "/")
    if create_empty_function:
        emptyFunctionsFname = os.path.join(projectPath, "sharedFunctions.h").replace("\\", "/")
    
    sys.stdout.write("projectPath         %s\n"%str(projectPath))
    sys.stdout.write("xmlFname            %s\n"%str(xmlFname))
    sys.stdout.write("menuDataFname       %s\n"%str(menuDataFname))
    if create_empty_function:
        sys.stdout.write("emptyFunctionsFname %s\n"%str(emptyFunctionsFname))
    sys.stdout.write("templateEditFname   %s\n"%str(templateEditFname))
    sys.stdout.write("userName            %s\n"%str(userName))
    sys.stdout.write("userMail            %s\n"%str(userMail))
    
    return projectPath, xmlFname, emptyFunctionsFname, userName, userMail, menuDataFname, templateEditFname

if __name__ == "__main__":
    
    import time
    start = time.time()
    
    projectPath, xmlFname, emptyFunctionsFname, userName, userMail, menuDataFname, templateEditFname = getParams()

    statText = EMPTY
    statValue = 0
    menu = menuParser.PARSER_MENU(xmlFname)
    
    makeMenuDataFile(menu, menuDataFname, userName, userMail, templateEditFname, emptyFunctionsFname)
    
    stop = time.time()
    sys.stdout.write("Job done in %.3f second(s)!\n"%(stop-start))
