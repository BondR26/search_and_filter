#include "start_search_hook.h"

bool gStartSearchPressed = false;

CRITICAL_SECTION gCrtSection;

HHOOK hKeyboardHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
            if (p->vkCode == VK_RETURN)
            {
                EnterCriticalSection(&gCrtSection);
                gStartSearchPressed = true;         
                LeaveCriticalSection(&gCrtSection);
            }
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

StartSearchHook::StartSearchHook()
{
    // Initialize critical section
    
    init();
}

StartSearchHook::~StartSearchHook()
{
    release();
   
}

void StartSearchHook::release()
{
	UnhookWindowsHookEx(hKeyboardHook);
}

void StartSearchHook::init()
{
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
}