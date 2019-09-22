#!/usr/bin/python
# -*- coding: utf-8 -*-

# 
# file : printLabels.h
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

# This script is used to generate a part of the documentation

import menuParser
from menuParser import EMPTY, LF, PATH_SEP, SPACE
import sys
import os
sys.stdout.write(sys.version + "\n")

def getLevel(item):
    return len(item.getPath().split(PATH_SEP)) - 1

def getMaxLabelSize(menu):
    sizes = []
    for index, item in enumerate(menu.getObjects()):
        label = "%s%s\n"%(SPACE * (getLevel(item) * 4), item.getLabel())
        sizes.append(len(label))
    return max(sizes)

def printLabels(menu):
    size = getMaxLabelSize(menu)
    sys.stdout.write("IDX LABEL                   PARENT   CHILD    NEXT    PREVIOUS\n")
    sys.stdout.write("--------------------------------------------------------------\n")
    for index, item in enumerate(menu.getObjects()):
        level = len(item.getPath().split(PATH_SEP)) - 1
        label = "%s%s"%(SPACE * (level * 4), item.getLabel())
        while len(label) < size:
            label += SPACE
        parent = item.formatIndex(item.getParent())
        child = item.formatIndex(item.getChild())
        next = item.formatIndex(item.getNext())
        previous = item.formatIndex(item.getPrevious())
        sys.stdout.write("%3u %s %-08s %-08s %-08s %-08s\n"%(index, label, parent, child, next, previous))

if __name__ == "__main__":
    
    inFname = None
    for index, value in enumerate(sys.argv):
        if value == "-i":
            inFname = sys.argv[index+1]

    menu = menuParser.PARSER_MENU(inFname)
    printLabels(menu)
