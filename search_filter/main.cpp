#include <iostream>

#include <UIAutomation.h>
#include <stdlib.h>

#include <windows.h>

#include "smart_uiauto.h"
#include "search_monitor.h"
#include "cmdparser.h"
#include "start_search_hook.h"


int main(int argc, char* argv[])
{
    SmartUiAuto mgr;

    CmdParser brw_info(argc, argv);
   
    auto search = mgr.getSearchBar(brw_info.getBrowser());
    
    CComPtr<SearchMonitor> pHandler = new SearchMonitor();
    pHandler->setup(search);

    // Message loop to keep the application running and handling events
    InitializeCriticalSection(&gCrtSection);
    StartSearchHook hk;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    DeleteCriticalSection(&gCrtSection);
    return 0;
}