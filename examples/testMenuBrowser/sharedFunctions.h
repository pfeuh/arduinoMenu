void editChannelIn(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        channelIn +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        channelIn -=1;
    menu.printVariable(integer2str(channelIn, false));
}

void editChannelOut(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        channelOut +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        channelOut -=1;
    menu.printVariable(integer2str(channelOut, false));
}

void editProgramNumber(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        programNumber +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        programNumber -=1;
    menu.printVariable(integer2str(programNumber, false));
}

void editArpeggiator(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        arpeggiator +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        arpeggiator -=1;
    menu.printVariable(integer2str(arpeggiator, false));
}

void editClockIn(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        clockIn +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        clockIn -=1;
    menu.printVariable(integer2str(clockIn, false));
}

void editClockOut(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        clockOut +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        clockOut -=1;
    menu.printVariable(integer2str(clockOut, false));
}

void editKeyClick(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        keyClick +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        keyClick -=1;
    menu.printVariable(integer2str(keyClick, false));
}

void editAudioBeat(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        audioBeat +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        audioBeat -=1;
    menu.printVariable(integer2str(audioBeat, false));
}

void editSysEx(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        sysEx +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        sysEx -=1;
    menu.printVariable(integer2str(sysEx, false));
}

void editTransposition(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        transposition +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        transposition -=1;
    menu.printVariable(integer2str(transposition, false));
}

void editGroove(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        groove +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        groove -=1;
    menu.printVariable(integer2str(groove, false));
}

void editGateMode(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        gateMode +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        gateMode -=1;
    menu.printVariable(integer2str(gateMode, false));
}

void editLastStep(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        lastStep +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        lastStep -=1;
    menu.printVariable(integer2str(lastStep, false));
}

void editCcNum(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        ccNum +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        ccNum -=1;
    menu.printVariable(integer2str(ccNum, false));
}

void editAppVersion(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        appVersion +=1;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        appVersion -=1;
    menu.printVariable(integer2str(appVersion, false));
}

byte dumpAll()
{
    return 0;
}

byte loadAll()
{
    return 0;
}

byte saveAll()
{
    return 0;
}

byte dumpGlobal()
{
    return 0;
}

byte loadGlobal()
{
    return 0;
}

byte saveGlobal()
{
    return 0;
}

byte initSeq()
{
    return 0;
}

byte swapSeq()
{
    return 0;
}

byte copySeq()
{
    return 0;
}

byte loadFactory()
{
    return 0;
}

byte dumpSeq()
{
    return 0;
}

byte loadSeq()
{
    return 0;
}

byte saveSeq()
{
    return 0;
}

byte factorySettings()
{
    return 0;
}

byte testStepLeds()
{
    return 0;
}

byte testIndLeds()
{
    return 0;
}

byte testBeeper()
{
    return 0;
}

byte displayVersions()
{
    return 0;
}

byte test1()
{
    return 1;
}

byte test2()
{
    return 2;
}

byte test3()
{
    return 3;
}

