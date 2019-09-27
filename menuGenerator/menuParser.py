#!/usr/bin/python
# -*- coding: utf-8 -*-

# 
# file : menuParser.h
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

import xml.etree.ElementTree as ET
import sys

EMPTY = ""
PATH_SEP = "/"
SPACE = " "
LF = "\n"

TAG_ROOT_TITLE = "rootTitle"
TAG_MENU_TREE  = "menuTree"
TAG_MENU       = "menu"
TAG_VARIABLE   = "variable"
TAG_FUNCTION   = "function"

ATTRIBUTE_CNAME     = "cname"
ATTRIBUTE_LABEL     = "label"

OBJECT_TYPES = [TAG_VARIABLE, TAG_FUNCTION, TAG_MENU]
MAX_LABEL_LEN = 17

class MENU_ITEM():
    INDEX_MENU = 0
    INDEX_VARIABLE = 0
    INDEX_FUNCTION = 0
    INDEX_OBJECT = 0

    def __init__(self, tag, cname, label, path):
        if not tag in OBJECT_TYPES:
            raise Exception("unexpected type %s"%str(tag))
        if len(label) > MAX_LABEL_LEN:
            sys.stdout.write('WARNING! label "%s" too long!\n'%label)
        self.__tag = tag
        self.__cname = cname
        self.__label = label
        self.__path = path
        self.__parent = None
        self.__next = None
        self.__previous = None
        self.__child = None
        
        if tag == TAG_MENU:
            self.__id = MENU_ITEM.INDEX_MENU
            MENU_ITEM.INDEX_MENU += 1
        elif tag == TAG_VARIABLE:
            self.__id = MENU_ITEM.INDEX_VARIABLE
            MENU_ITEM.INDEX_VARIABLE += 1
        elif tag == TAG_FUNCTION:
            self.__id = MENU_ITEM.INDEX_FUNCTION
            MENU_ITEM.INDEX_FUNCTION += 1
            
        self.__index = MENU_ITEM.INDEX_OBJECT
        MENU_ITEM.INDEX_OBJECT += 1

    def formatIndex(self, index):
        if index != None:
            text = "%-3u"%index
        else:
            text = "---"
        return text

    def tostring(self):
        parent = self.formatIndex(self.__parent)
        child = self.formatIndex(self.__child)
        next = self.formatIndex(self.__next)
        previous = self.formatIndex(self.__previous)
        txt = '%-08s %3u %3u parent=%s child=%s next=%s previous=%s cname="%s" label="%s" path="%s"'%(self.__tag, self.__index, self.__id, parent, child, next, previous, self.__cname, self.__label, self.__path)
        return txt

    def getId(self):
        return self.__id

    def setParent(self, index):
        self.__parent = index

    def getParent(self):
        return self.__parent

    def setChild(self, index):
        self.__child = index

    def getChild(self):
        return self.__child

    def setNext(self, index):
        self.__next = index

    def getNext(self):
        return self.__next

    def setPrevious(self, index):
        self.__previous = index

    def getPrevious(self):
        return self.__previous

    def getTag(self):
        return self.__tag

    def getCname(self):
        return self.__cname

    def getLabel(self):
        return self.__label

    def getPath(self):
        return self.__path

    def getIndex(self):
        return self.__index

class PARSER_MENU():
    def __init__(self, xml_fname):
        self.__doc = ET.parse(xml_fname)
        self.__xml_root = self.__doc.getroot()
        self.__objects = []
        
        root = self.__xml_root
        self.__currentPath = EMPTY
        self.__parseFile(root)
        if len(self.__objects) > 255:
            raise Exception("Too much entries (%u): 255 max!"%len(self.__objects))
        if self.__rootLabel == None:
            self.__rootLabel = " ---*** MENU ***---"
        self.populateFamily()
        
    def getRootLabel(self):
        return self.__rootLabel

    def __getParentPath(self, path):
        return PATH_SEP.join(path.split(PATH_SEP)[:-1])
        
    def pushPath(self, label):
        if self.__currentPath == EMPTY:
            self.__currentPath = label
        else:
            self.__currentPath = "%s%s%s"%(self.__currentPath, PATH_SEP, label)
        return self.__currentPath
        
    def popPath(self):
        if PATH_SEP in self.__currentPath:
            self.__currentPath = PATH_SEP.join(self.__currentPath.split(PATH_SEP)[:-1])
        elif self.__currentPath == EMPTY:
            raise Exception("PATH is already empty!")
        else:
            self.__currentPath = EMPTY
        return self.__currentPath

    def __getObjectsList(self, tag):
        objects = []
        for item in self.__objects:
            if item.getTag() == tag:
                objects.append(item)
        return objects
        
    def getObjects(self):
        return self.__objects
        
    def getMenus(self):
        return self.__getObjectsList(TAG_MENU)
    
    def getVariables(self):
        return self.__getObjectsList(TAG_VARIABLE)
    
    def getFunctions(self):
        return self.__getObjectsList(TAG_FUNCTION)
    
    def __parseFile(self, element):
        if element.tag != TAG_MENU_TREE:
            path = self.pushPath(element.get(ATTRIBUTE_CNAME))
            tag = element.tag
            if tag in (TAG_MENU, TAG_VARIABLE, TAG_FUNCTION):
                cname = element.get(ATTRIBUTE_CNAME)
                label = element.get(ATTRIBUTE_LABEL)
                temp_obj = MENU_ITEM(tag, cname, label, path)
                self.__objects.append(temp_obj)
            elif tag == TAG_ROOT_TITLE:
                self.__rootLabel = element.get(ATTRIBUTE_LABEL)
            else:
                raise Exception('Unexpected element "%s" found!\n'%tag)
            
        for child in element:
            self.__parseFile(child)
            
        if not element.tag in[TAG_MENU_TREE, TAG_ROOT_TITLE]:
            self.popPath()
        return

    def populateFamily(self):
        for index, item in enumerate(self.__objects):
            path = item.getPath()
            #let's find parent
            parent_path = self.__getParentPath(path)
            for x in range(index):
                parent = self.__objects[x]
                if parent.getPath() == parent_path:
                    # parent found
                    item.setParent(parent.getIndex())
                    break
            #let's find child
            for x in range(index+1, len(self.__objects)):
                child = self.__objects[x]
                if self.__getParentPath(child.getPath()) == path:
                    # child found
                    item.setChild(child.getIndex())
                    break
            # build brothers list
            brothers = []
            parent = self.__getParentPath(path)
            for brother in self.__objects:
                if self.__getParentPath(brother.getPath()) == parent:
                    brothers.append(brother.getIndex())
            #lets find next
            for brother in brothers:
                if brother > item.getIndex():
                    # next found
                    item.setNext(brother)
                    break
            #lets find previous
            brothers.reverse()
            for brother in brothers:
                if brother < item.getIndex():
                    # previous found
                    item.setPrevious(brother)
                    break

    def tostring(self):
        txt = EMPTY
        for item in self.__objects:
            txt += item.tostring() + LF
        return txt

if __name__ == "__main__":

    menu_fname = "../src/arduinoMenu/menuTree.xml"
    parsed_menu = PARSER_MENU(menu_fname)
    sys.stdout.write(parsed_menu.getRootLabel() + '\n')
    sys.stdout.write(parsed_menu.tostring())
