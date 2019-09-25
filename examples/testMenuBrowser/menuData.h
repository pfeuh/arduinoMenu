#ifndef menuData_h
#define menuData_h

/*
 * file : menuData.h
 * Copyright (c) pfeuh <ze.pfeuh@gmail.com>
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

/*********************************************/
/*                  WARNING!                 */
/* This file is generated by an application, */
/*   if you edit it, it will be overwritten  */
/*         by the next generation!           */
/*********************************************/

#include <Arduino.h>

#define MENU_GENERATOR_PY_VERSION "1.00"
#define MENU_GENERATOR_TIME_STAMP "2019/09/25 17:13:17"

#define MENU_BROWSER_NB_ENTRIES 40
#define MENU_BROWSER_NB_VARIABLES 15
#define MENU_BROWSER_NB_FUNCTIONS 21

// Table of parent              00040
// Table of child               00040
// Table of next                00040
// Table of previous            00040
// Table of labels              00516
// Table of pointers to labels  00080
// Table of function callbacks  00080
// Table of edit callbacks      00080
// Table of types               00040
// ----------------------------------
// TOTAL                        00956

// functions suppposed ready to execute
extern byte dumpAll();
extern byte loadAll();
extern byte saveAll();
extern byte dumpGlobal();
extern byte loadGlobal();
extern byte saveGlobal();
extern byte initSeq();
extern byte swapSeq();
extern byte copySeq();
extern byte loadFactory();
extern byte dumpSeq();
extern byte loadSeq();
extern byte saveSeq();
extern byte factorySettings();
extern byte testStepLeds();
extern byte testIndLeds();
extern byte testBeeper();
extern byte displayVersions();
extern byte test1();
extern byte test2();
extern byte test3();

// edit & display functions of variables suppposed ready to execute
extern void editChannelIn(byte direction);
extern void editChannelOut(byte direction);
extern void editProgramNumber(byte direction);
extern void editArpeggiator(byte direction);
extern void editClockIn(byte direction);
extern void editClockOut(byte direction);
extern void editKeyClick(byte direction);
extern void editAudioBeat(byte direction);
extern void editSysEx(byte direction);
extern void editTransposition(byte direction);
extern void editGroove(byte direction);
extern void editGateMode(byte direction);
extern void editLastStep(byte direction);
extern void editCcNum(byte direction);
extern void editAppVersion(byte direction);

const byte parentTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ MENU_BROWSER_NO_ENTRY,
   /* 001 */ 0,
   /* 002 */ 0,
   /* 003 */ 0,
   /* 004 */ 0,
   /* 005 */ 0,
   /* 006 */ 0,
   /* 007 */ 0,
   /* 008 */ 0,
   /* 009 */ 0,
   /* 010 */ 0,
   /* 011 */ 0,
   /* 012 */ 0,
   /* 013 */ 0,
   /* 014 */ 0,
   /* 015 */ 0,
   /* 016 */ 0,
   /* 017 */ MENU_BROWSER_NO_ENTRY,
   /* 018 */ 17,
   /* 019 */ 17,
   /* 020 */ 17,
   /* 021 */ 17,
   /* 022 */ 17,
   /* 023 */ 17,
   /* 024 */ 17,
   /* 025 */ 17,
   /* 026 */ 17,
   /* 027 */ 17,
   /* 028 */ 17,
   /* 029 */ MENU_BROWSER_NO_ENTRY,
   /* 030 */ 29,
   /* 031 */ 29,
   /* 032 */ 29,
   /* 033 */ 29,
   /* 034 */ 29,
   /* 035 */ 29,
   /* 036 */ 35,
   /* 037 */ 35,
   /* 038 */ 35,
   /* 039 */ 35,
};

const byte childTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ 1,
   /* 001 */ MENU_BROWSER_NO_ENTRY,
   /* 002 */ MENU_BROWSER_NO_ENTRY,
   /* 003 */ MENU_BROWSER_NO_ENTRY,
   /* 004 */ MENU_BROWSER_NO_ENTRY,
   /* 005 */ MENU_BROWSER_NO_ENTRY,
   /* 006 */ MENU_BROWSER_NO_ENTRY,
   /* 007 */ MENU_BROWSER_NO_ENTRY,
   /* 008 */ MENU_BROWSER_NO_ENTRY,
   /* 009 */ MENU_BROWSER_NO_ENTRY,
   /* 010 */ MENU_BROWSER_NO_ENTRY,
   /* 011 */ MENU_BROWSER_NO_ENTRY,
   /* 012 */ MENU_BROWSER_NO_ENTRY,
   /* 013 */ MENU_BROWSER_NO_ENTRY,
   /* 014 */ MENU_BROWSER_NO_ENTRY,
   /* 015 */ MENU_BROWSER_NO_ENTRY,
   /* 016 */ MENU_BROWSER_NO_ENTRY,
   /* 017 */ 18,
   /* 018 */ MENU_BROWSER_NO_ENTRY,
   /* 019 */ MENU_BROWSER_NO_ENTRY,
   /* 020 */ MENU_BROWSER_NO_ENTRY,
   /* 021 */ MENU_BROWSER_NO_ENTRY,
   /* 022 */ MENU_BROWSER_NO_ENTRY,
   /* 023 */ MENU_BROWSER_NO_ENTRY,
   /* 024 */ MENU_BROWSER_NO_ENTRY,
   /* 025 */ MENU_BROWSER_NO_ENTRY,
   /* 026 */ MENU_BROWSER_NO_ENTRY,
   /* 027 */ MENU_BROWSER_NO_ENTRY,
   /* 028 */ MENU_BROWSER_NO_ENTRY,
   /* 029 */ 30,
   /* 030 */ MENU_BROWSER_NO_ENTRY,
   /* 031 */ MENU_BROWSER_NO_ENTRY,
   /* 032 */ MENU_BROWSER_NO_ENTRY,
   /* 033 */ MENU_BROWSER_NO_ENTRY,
   /* 034 */ MENU_BROWSER_NO_ENTRY,
   /* 035 */ 36,
   /* 036 */ MENU_BROWSER_NO_ENTRY,
   /* 037 */ MENU_BROWSER_NO_ENTRY,
   /* 038 */ MENU_BROWSER_NO_ENTRY,
   /* 039 */ MENU_BROWSER_NO_ENTRY,
};

const byte nextTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ 17,
   /* 001 */ 2,
   /* 002 */ 3,
   /* 003 */ 4,
   /* 004 */ 5,
   /* 005 */ 6,
   /* 006 */ 7,
   /* 007 */ 8,
   /* 008 */ 9,
   /* 009 */ 10,
   /* 010 */ 11,
   /* 011 */ 12,
   /* 012 */ 13,
   /* 013 */ 14,
   /* 014 */ 15,
   /* 015 */ 16,
   /* 016 */ MENU_BROWSER_NO_ENTRY,
   /* 017 */ 29,
   /* 018 */ 19,
   /* 019 */ 20,
   /* 020 */ 21,
   /* 021 */ 22,
   /* 022 */ 23,
   /* 023 */ 24,
   /* 024 */ 25,
   /* 025 */ 26,
   /* 026 */ 27,
   /* 027 */ 28,
   /* 028 */ MENU_BROWSER_NO_ENTRY,
   /* 029 */ MENU_BROWSER_NO_ENTRY,
   /* 030 */ 31,
   /* 031 */ 32,
   /* 032 */ 33,
   /* 033 */ 34,
   /* 034 */ 35,
   /* 035 */ MENU_BROWSER_NO_ENTRY,
   /* 036 */ 37,
   /* 037 */ 38,
   /* 038 */ 39,
   /* 039 */ MENU_BROWSER_NO_ENTRY,
};

const byte previousTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ MENU_BROWSER_NO_ENTRY,
   /* 001 */ MENU_BROWSER_NO_ENTRY,
   /* 002 */ 1,
   /* 003 */ 2,
   /* 004 */ 3,
   /* 005 */ 4,
   /* 006 */ 5,
   /* 007 */ 6,
   /* 008 */ 7,
   /* 009 */ 8,
   /* 010 */ 9,
   /* 011 */ 10,
   /* 012 */ 11,
   /* 013 */ 12,
   /* 014 */ 13,
   /* 015 */ 14,
   /* 016 */ 15,
   /* 017 */ 0,
   /* 018 */ MENU_BROWSER_NO_ENTRY,
   /* 019 */ 18,
   /* 020 */ 19,
   /* 021 */ 20,
   /* 022 */ 21,
   /* 023 */ 22,
   /* 024 */ 23,
   /* 025 */ 24,
   /* 026 */ 25,
   /* 027 */ 26,
   /* 028 */ 27,
   /* 029 */ 17,
   /* 030 */ MENU_BROWSER_NO_ENTRY,
   /* 031 */ 30,
   /* 032 */ 31,
   /* 033 */ 32,
   /* 034 */ 33,
   /* 035 */ 34,
   /* 036 */ MENU_BROWSER_NO_ENTRY,
   /* 037 */ 36,
   /* 038 */ 37,
   /* 039 */ 38,
};

/*   0 */ const char labelGlobalEdit[] PROGMEM = "GLOBAL EDIT";
/*   1 */ const char labelChannelIn[] PROGMEM = "Input channel";
/*   2 */ const char labelChannelOut[] PROGMEM = "Output channel";
/*   3 */ const char labelProgramNumber[] PROGMEM = "Program number";
/*   4 */ const char labelArpeggiator[] PROGMEM = "Arpeggiator";
/*   5 */ const char labelClockIn[] PROGMEM = "Midi clock in";
/*   6 */ const char labelClockOut[] PROGMEM = "Midi clock out";
/*   7 */ const char labelKeyClick[] PROGMEM = "Keyboard echo";
/*   8 */ const char labelAudioBeat[] PROGMEM = "Metronome";
/*   9 */ const char labelSysEx[] PROGMEM = "systemExclusive";
/*  10 */ const char labelTransposition[] PROGMEM = "Transposition";
/*  11 */ const char labelDumpAll[] PROGMEM = "Dump all";
/*  12 */ const char labelLoadAll[] PROGMEM = "Load all";
/*  13 */ const char labelSaveAll[] PROGMEM = "Save all";
/*  14 */ const char labelDumpGlobal[] PROGMEM = "Dump global par.";
/*  15 */ const char labelLoadGlobal[] PROGMEM = "Load global par.";
/*  16 */ const char labelSaveGlobal[] PROGMEM = "Save global par.";
/*  17 */ const char labelSequenceEdit[] PROGMEM = "SEQUENCE EDIT";
/*  18 */ const char labelGroove[] PROGMEM = "Groove";
/*  19 */ const char labelGateMode[] PROGMEM = "Gate duration";
/*  20 */ const char labelLastStep[] PROGMEM = "Last step";
/*  21 */ const char labelCcNum[] PROGMEM = "Ctrl chg number";
/*  22 */ const char labelInitSeq[] PROGMEM = "initialize seq.";
/*  23 */ const char labelSwapSeq[] PROGMEM = "Swap sequences";
/*  24 */ const char labelCopySeq[] PROGMEM = "copy sequence";
/*  25 */ const char labelLoadFactory[] PROGMEM = "Load factory seq";
/*  26 */ const char labelDumpSeq[] PROGMEM = "Dump sequence";
/*  27 */ const char labelLoadSeq[] PROGMEM = "Load sequence";
/*  28 */ const char labelSaveSeq[] PROGMEM = "Save sequence";
/*  29 */ const char labelMaintenance[] PROGMEM = "MAINTENANCE";
/*  30 */ const char labelFactorySettings[] PROGMEM = "Factory settings";
/*  31 */ const char labelTestStepLeds[] PROGMEM = "Test step leds";
/*  32 */ const char labelTestIndLeds[] PROGMEM = "Test ind. leds";
/*  33 */ const char labelTestBeeper[] PROGMEM = "Test Beeper";
/*  34 */ const char labelDisplayVersions[] PROGMEM = "Versions";
/*  35 */ const char labelSubmenu[] PROGMEM = "TESTS";
/*  36 */ const char labelTest1[] PROGMEM = "test 1";
/*  37 */ const char labelTest2[] PROGMEM = "test 2";
/*  38 */ const char labelTest3[] PROGMEM = "test 3";
/*  39 */ const char labelAppVersion[] PROGMEM = "Version number";

const char *const labelsTable[MENU_BROWSER_NB_ENTRIES] PROGMEM =
{
    /*   0 */ labelGlobalEdit,
    /*   1 */ labelChannelIn,
    /*   2 */ labelChannelOut,
    /*   3 */ labelProgramNumber,
    /*   4 */ labelArpeggiator,
    /*   5 */ labelClockIn,
    /*   6 */ labelClockOut,
    /*   7 */ labelKeyClick,
    /*   8 */ labelAudioBeat,
    /*   9 */ labelSysEx,
    /*  10 */ labelTransposition,
    /*  11 */ labelDumpAll,
    /*  12 */ labelLoadAll,
    /*  13 */ labelSaveAll,
    /*  14 */ labelDumpGlobal,
    /*  15 */ labelLoadGlobal,
    /*  16 */ labelSaveGlobal,
    /*  17 */ labelSequenceEdit,
    /*  18 */ labelGroove,
    /*  19 */ labelGateMode,
    /*  20 */ labelLastStep,
    /*  21 */ labelCcNum,
    /*  22 */ labelInitSeq,
    /*  23 */ labelSwapSeq,
    /*  24 */ labelCopySeq,
    /*  25 */ labelLoadFactory,
    /*  26 */ labelDumpSeq,
    /*  27 */ labelLoadSeq,
    /*  28 */ labelSaveSeq,
    /*  29 */ labelMaintenance,
    /*  30 */ labelFactorySettings,
    /*  31 */ labelTestStepLeds,
    /*  32 */ labelTestIndLeds,
    /*  33 */ labelTestBeeper,
    /*  34 */ labelDisplayVersions,
    /*  35 */ labelSubmenu,
    /*  36 */ labelTest1,
    /*  37 */ labelTest2,
    /*  38 */ labelTest3,
    /*  39 */ labelAppVersion,
};

const PROGMEM MENU_BROWSER_FUNCTION_PTR execFunctionsTable[21] = 
{
    /*  11   0 */ &dumpAll,
    /*  12   1 */ &loadAll,
    /*  13   2 */ &saveAll,
    /*  14   3 */ &dumpGlobal,
    /*  15   4 */ &loadGlobal,
    /*  16   5 */ &saveGlobal,
    /*  22   6 */ &initSeq,
    /*  23   7 */ &swapSeq,
    /*  24   8 */ &copySeq,
    /*  25   9 */ &loadFactory,
    /*  26  10 */ &dumpSeq,
    /*  27  11 */ &loadSeq,
    /*  28  12 */ &saveSeq,
    /*  30  13 */ &factorySettings,
    /*  31  14 */ &testStepLeds,
    /*  32  15 */ &testIndLeds,
    /*  33  16 */ &testBeeper,
    /*  34  17 */ &displayVersions,
    /*  36  18 */ &test1,
    /*  37  19 */ &test2,
    /*  38  20 */ &test3,
};

const PROGMEM MENU_BROWSER_EDIT_PTR editFunctionsTable[15] = 
{
    /*   1   0 */ &editChannelIn,
    /*   2   1 */ &editChannelOut,
    /*   3   2 */ &editProgramNumber,
    /*   4   3 */ &editArpeggiator,
    /*   5   4 */ &editClockIn,
    /*   6   5 */ &editClockOut,
    /*   7   6 */ &editKeyClick,
    /*   8   7 */ &editAudioBeat,
    /*   9   8 */ &editSysEx,
    /*  10   9 */ &editTransposition,
    /*  18  10 */ &editGroove,
    /*  19  11 */ &editGateMode,
    /*  20  12 */ &editLastStep,
    /*  21  13 */ &editCcNum,
    /*  39  14 */ &editAppVersion,
};

const enum menuOptionType itemTypeTable[40] PROGMEM = 
{
   /* 000 */ menuTypeMenu,
   /* 001 */ menuTypeVariable,
   /* 002 */ menuTypeVariable,
   /* 003 */ menuTypeVariable,
   /* 004 */ menuTypeVariable,
   /* 005 */ menuTypeVariable,
   /* 006 */ menuTypeVariable,
   /* 007 */ menuTypeVariable,
   /* 008 */ menuTypeVariable,
   /* 009 */ menuTypeVariable,
   /* 010 */ menuTypeVariable,
   /* 011 */ menuTypeFunction,
   /* 012 */ menuTypeFunction,
   /* 013 */ menuTypeFunction,
   /* 014 */ menuTypeFunction,
   /* 015 */ menuTypeFunction,
   /* 016 */ menuTypeFunction,
   /* 017 */ menuTypeMenu,
   /* 018 */ menuTypeVariable,
   /* 019 */ menuTypeVariable,
   /* 020 */ menuTypeVariable,
   /* 021 */ menuTypeVariable,
   /* 022 */ menuTypeFunction,
   /* 023 */ menuTypeFunction,
   /* 024 */ menuTypeFunction,
   /* 025 */ menuTypeFunction,
   /* 026 */ menuTypeFunction,
   /* 027 */ menuTypeFunction,
   /* 028 */ menuTypeFunction,
   /* 029 */ menuTypeMenu,
   /* 030 */ menuTypeFunction,
   /* 031 */ menuTypeFunction,
   /* 032 */ menuTypeFunction,
   /* 033 */ menuTypeFunction,
   /* 034 */ menuTypeFunction,
   /* 035 */ menuTypeMenu,
   /* 036 */ menuTypeFunction,
   /* 037 */ menuTypeFunction,
   /* 038 */ menuTypeFunction,
   /* 039 */ menuTypeVariable,
};

#endif

