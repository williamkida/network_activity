
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include "windows.h"
#undef DO_EXPORTS
#include "network_activity.h"
#pragma comment(lib, "network_activity")
int _tmain(int argc, _TCHAR* argv[])
{
    GetIpv4EnabledNics();
    WriteActiveNicsList();
    //WriteActiveNicsListLog();
    DWORD d3 = GetOutOctets();
    GetOutOctetsLog();
    DWORD d4 = GetInOctets();
    GetIpv4EnabledNicsLog();
    //d3 = GetOutOctets();
    d4 = GetInOctetsLog();
    CreateShortcutInStartUp();
    wchar_t* a=GetStartupDirectory();
}

