void dumpAll()
{
}

void loadAll()
{
}

void saveAll()
{
}

void dumpGlobal()
{
}

void loadGlobal()
{
}

void saveGlobal()
{
}

void initSeq()
{
}

void swapSeq()
{
}

void copySeq()
{
}

void loadFactory()
{
}

void dumpSeq()
{
}

void loadSeq()
{
}

void saveSeq()
{
}

void factorySettings()
{
}

void testStepLeds()
{
}

void testIndLeds()
{
}

void testBeeper()
{
}

void displayVersions()
{
}

void editChannelIn(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        channelIn +=1;
    else
        channelIn -=1;
    Serial.print(channelIn);
    Serial.write('\n');
}

void editChannelOut(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        channelOut +=1;
    else
        channelOut -=1;
    Serial.print(channelOut);
    Serial.write('\n');
}

void editProgramNumber(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        programNumber +=1;
    else
        programNumber -=1;
    Serial.print(programNumber);
    Serial.write('\n');
}

void editArpeggiator(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        arpeggiator +=1;
    else
        arpeggiator -=1;
    Serial.print(arpeggiator);
    Serial.write('\n');
}

void editClockIn(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        clockIn +=1;
    else
        clockIn -=1;
    Serial.print(clockIn);
    Serial.write('\n');
}

void editClockOut(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        clockOut +=1;
    else
        clockOut -=1;
    Serial.print(clockOut);
    Serial.write('\n');
}

void editKeyClick(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        keyClick +=1;
    else
        keyClick -=1;
    Serial.print(keyClick);
    Serial.write('\n');
}

void editAudioBeat(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        audioBeat +=1;
    else
        audioBeat -=1;
    Serial.print(audioBeat);
    Serial.write('\n');
}

void editSysEx(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        sysEx +=1;
    else
        sysEx -=1;
    Serial.print(sysEx);
    Serial.write('\n');
}

void editTransposition(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        transposition +=1;
    else
        transposition -=1;
    Serial.print(transposition);
    Serial.write('\n');
}

void editGroove(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        groove +=1;
    else
        groove -=1;
    Serial.print(groove);
    Serial.write('\n');
}

void editGateMode(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        gateMode +=1;
    else
        gateMode -=1;
    Serial.print(gateMode);
    Serial.write('\n');
}

void editLastStep(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        lastStep +=1;
    else
        lastStep -=1;
    Serial.print(lastStep);
    Serial.write('\n');
}

void editCcNum(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        ccNum +=1;
    else
        ccNum -=1;
    Serial.print(ccNum);
    Serial.write('\n');
}

void editTest1(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        test1 +=1;
    else
        test1 -=1;
    Serial.print(test1);
    Serial.write('\n');
}

void editTest2(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        test2 +=1;
    else
        test2 -=1;
    Serial.print(test2);
    Serial.write('\n');
}

void editTest3(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        test3 +=1;
    else
        test3 -=1;
    Serial.print(test3);
    Serial.write('\n');
}

void editAppVersion(bool direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        appVersion +=1;
    else
        appVersion -=1;
    Serial.print(appVersion);
    Serial.write('\n');
}

