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
from menuParser import EMPTY, LF, SPACE, PATH_SEP
import sys
import os

sys.stdout.write(sys.version + "\n")

NO_ENTRY = "MENU_BROWSER_NO_ENTRY"
NB_ENTRIES = "MENU_BROWSER_NB_ENTRIES"
NB_VARIABLES = "MENU_BROWSER_NB_VARIABLES"
NB_FUNCTIONS = "MENU_BROWSER_NB_FUNCTIONS"
ITEM_TYPE_TABLE = {"menu":"menuTypeMenu", "variable":"menuTypeVariable", "function":"menuTypeFunction"}
HEADER_PATTERN_NAME = "headerPattern.txt"
SPLASH_PATTERN_NAME = "splashPattern.txt"
SIZE_OF_FUNCTION_POINTER = 2
SIZE_OF_CHAR_POINTER = 2
FUNCTION_PTR = "MENU_BROWSER_FUNCTION_PTR"
EDIT_PTR = "MENU_BROWSER_EDIT_PTR"
ROOT_LABEL = "MENU_BROWSER_ROOT_LABEL"

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
    text = "// functions\n"
    for function in menu.getFunctions():
        text += "extern byte %s();\n"%(function.getCname())
    text += LF
    return text

def getFunctionsCode(menu, fname, black_list):
    text = EMPTY
    with open(fname) as fp:
        pattern = fp.read(-1)
    for function in menu.getFunctions():
        funcname = function.getCname()
        if not funcname in black_list:
            current_pattern = pattern[:]
            current_pattern = current_pattern.replace("#funcname#", funcname)
            text += current_pattern
        else:
            text += "// Function %s is black listed.\n\n"%funcname
    return text

def getDeclareVariablesCode(menu):
    text = "// edit/display variables' functions\n"
    for variable in menu.getVariables():
        text += "extern void edit%s(byte direction);\n"%(pascalize(variable.getCname()))
    text += LF
    return text

def getRootLabelCode(menu):
    title_len = len(menu.getRootLabel()) + 1
    text = 'const char %s[%s] PROGMEM = "%s";\n\n'%(ROOT_LABEL, title_len, menu.getRootLabel())
    addStatLine("%s root's title", title_len)
    return text
    
def getVariablesCode(menu, fname, black_list):
    text = EMPTY
    with open(fname) as fp:
        pattern = fp.read(-1)
    for variable in menu.getVariables():
        varname = variable.getCname()
        funcname = "edit" + pascalize(varname)
        if not funcname in black_list:
            current_pattern = pattern[:]
            current_pattern = current_pattern.replace("#funcname#", funcname)
            current_pattern = current_pattern.replace("#varname#", varname)
            text += current_pattern
        else:
            text += "// Function %s is black listed.\n\n"%funcname            
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
    return "%04u/%02u/%02u %02u:%02u:%02u"%(now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec)

def getSplash(fname, user_name, mail_address):
    with open(SPLASH_PATTERN_NAME) as fp:
        text = fp.read(-1)

    text = text.replace("#fname#", os.path.basename(fname))
    
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

def getHeader(fname, user_name, mail_address):
    with open(HEADER_PATTERN_NAME) as fp:
        text = fp.read(-1)
    
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

def getLevel(item):
    return len(item.getPath().split(PATH_SEP)) - 1

def getMaxLabelSize(menu):
    sizes = []
    for index, item in enumerate(menu.getObjects()):
        label = "%s%s\n"%(SPACE * (getLevel(item) * 4), item.getLabel())
        sizes.append(len(label))
    return max(sizes)

def getLabels(menu):
    size = getMaxLabelSize(menu)
    text = EMPTY
    text += "IDX LABEL                   PARENT   CHILD    NEXT    PREVIOUS\n"
    text += "--------------------------------------------------------------\n"
    for index, item in enumerate(menu.getObjects()):
        level = len(item.getPath().split(PATH_SEP)) - 1
        label = "%s%s"%(SPACE * (level * 4), item.getLabel())
        while len(label) < size:
            label += SPACE
        parent = item.formatIndex(item.getParent())
        child = item.formatIndex(item.getChild())
        next = item.formatIndex(item.getNext())
        previous = item.formatIndex(item.getPrevious())
        text += "%3u %s %-08s %-08s %-08s %-08s\n"%(index, label, parent, child, next, previous)
    return text

def getBlackListedFunctionsTable(blacklist_fname):
    with open(blacklist_fname) as fp:
        lines = [line.strip() for line in fp.readlines()]
    lines = [line for line in lines if line != EMPTY and not line.startswith("#")]
    return lines

def makeMenuDataFile(menu, fname, user_name, mail_address, edit_function_template,\
        function_template, debug_fname, blacklist_fname, empty_functions_fname=None):
    global statText, statValue
    statText = EMPTY
    statValue = 0
    # file menuData.h
    text  =  "#define MENU_BROWSER_NB_ENTRIES %u\n"%len(menu.getObjects())
    text +=  "#define MENU_BROWSER_NB_VARIABLES %u\n"%len(menu.getVariables())
    text +=  "#define MENU_BROWSER_NB_FUNCTIONS %u\n\n"%len(menu.getFunctions())
    text += getRootLabelCode(menu)
    text += getDeclareFunctionsCode(menu)
    text += getDeclareVariablesCode(menu)
    text += getParentTableCode(menu)
    text += getChildTableCode(menu)
    text += getNextTableCode(menu)
    text += getPreviousTableCode(menu)
    text += getLabelTableCode(menu)
    text += getFunctionsTableCode(menu)
    text += getEditFunctionsTableCode(menu)
    text += getItemTypeTable(menu)
    
    body = getHeader(fname, user_name, mail_address)
    body = body.replace("#body#", text)
    body = body.replace("#splash#", getSplash(fname, user_name, mail_address))
    statText +=  "// ----------------------------------\n"
    statText +=  "// TOTAL                        %05u\n\n"%statValue
    body = body.replace("#stats#", statText)
    with open(fname, "w") as fp:
        fp.write(body)

    # file emptyFunctions.h
    if blacklist_fname == None:
        black_list = []
    else:
        black_list = getBlackListedFunctionsTable(blacklist_fname)

    if empty_functions_fname != None:
        text = getSplash(empty_functions_fname, user_name, mail_address)
        text += getVariablesCode(menu, edit_function_template, black_list)
        text += getFunctionsCode(menu, function_template, black_list)
        with open(empty_functions_fname, "w") as fp:
            fp.write(text)

    # file menuDebug.txt
    if debug_fname:
        text  = getSplash(debug_fname, user_name, mail_address)
        text += getLabels(menu)
        with open(debug_fname, "w") as fp:
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
    blackListedFunctionsFname = None
    templateEditFname = "./defaultTEF.txt"
    templateFname = "./defaultTF.txt"
    debug_flag = False
    debugFname = None
    
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
        elif line.startswith("-tf "):
            templateFname = line[len("-tf "):].replace("\\", "/")
        elif line == "-d":
            debug_flag = True
        elif line.startswith("-b "):
            blackListedFunctionsFname = line[len("-b "):].replace("\\", "/")
            
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
    if templateFname != None:
        if not os.path.isfile(templateFname):
            raise Exception("%s not found!\n"%str(templateFname))
    
    menuDataFname = os.path.join(projectPath, "menuData.h").replace("\\", "/")
    if create_empty_function:
        emptyFunctionsFname = os.path.join(projectPath, "sharedFunctions.h").replace("\\", "/")
    if debug_flag:
        debugFname = os.path.join(projectPath, "menuDebug.txt").replace("\\", "/")
    
    sys.stdout.write("projectPath         %s\n"%str(projectPath))
    sys.stdout.write("xmlFname            %s\n"%str(xmlFname))
    sys.stdout.write("menuDataFname       %s\n"%str(menuDataFname))
    if create_empty_function:
        sys.stdout.write("emptyFunctionsFname %s\n"%str(emptyFunctionsFname))
    if debug_flag:
        sys.stdout.write("debugFname          %s\n"%str(debugFname))
    sys.stdout.write("templateEditFname   %s\n"%str(templateEditFname))
    sys.stdout.write("templateFname       %s\n"%str(templateFname))
    if blackListedFunctionsFname != None:
        sys.stdout.write("BlackListedFname    %s\n"%str(blackListedFunctionsFname))
    sys.stdout.write("userName            %s\n"%str(userName))
    sys.stdout.write("userMail            %s\n"%str(userMail))
    
    return projectPath, xmlFname, emptyFunctionsFname, userName, userMail, menuDataFname, templateEditFname,\
        templateFname, debugFname, blackListedFunctionsFname

if __name__ == "__main__":
    
    import time
    start = time.time()
    
    projectPath, xmlFname, emptyFunctionsFname, userName, userMail, menuDataFname, templateEditFname,\
        templateFname, debugFname, blackListedFunctionsFname = getParams()

    statText = EMPTY
    statValue = 0
    menu = menuParser.PARSER_MENU(xmlFname)
    
    makeMenuDataFile(menu, menuDataFname, userName, userMail, templateEditFname, templateFname,
        debugFname, blackListedFunctionsFname, emptyFunctionsFname)
    
    stop = time.time()
    sys.stdout.write("Job done in %.3f second(s)!\n"%(stop-start))
