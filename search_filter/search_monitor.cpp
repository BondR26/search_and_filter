#include "search_monitor.h"

#include <iostream>

#include <AtlBase.h>
#include <AtlCom.h>

#include <UIAutomation.h>
#include <UIAutomationCore.h>
#include <UIAutomationClient.h>

#include "start_search_hook.h"


SearchMonitor::SearchMonitor()
    : _refCount(1)
{
    
}

SearchMonitor::~SearchMonitor()
{

}

ULONG STDMETHODCALLTYPE SearchMonitor::AddRef()
{
    return InterlockedIncrement(&_refCount);
}

ULONG STDMETHODCALLTYPE SearchMonitor::Release()
{
    LONG count = InterlockedDecrement(&_refCount);
    if (count == 0) delete this;
    return count;
}

HRESULT STDMETHODCALLTYPE SearchMonitor::QueryInterface(REFIID riid, void** ppvObject) {
    if (riid == IID_IUnknown || riid == IID_IUIAutomationEventHandler) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

const std::wstring gSearchSymb = L"q=";
const std::wstring gTestSymb = L"test:";

std::mutex gMutex;

HRESULT STDMETHODCALLTYPE SearchMonitor::HandleAutomationEvent(IUIAutomationElement* sender, EVENTID eventId)
{
    // Ensure mutual exclusion while handling the event
    std::lock_guard<std::mutex> lock(gMutex);

    // Enter critical section
    if (eventId == UIA_Text_TextChangedEventId )
    {
         std::cout << "Event in chagning text\n";
         EnterCriticalSection(&gCrtSection);
         if (gStartSearchPressed)
         {
             std::cout << "The enter was pressed\n";
            
             BSTR name;
             CComVariant url;
             sender->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &url);

             std::wcout << L"Address bar text searched: " << url.bstrVal << std::endl;

             std::wstring search_str = url.bstrVal;

             // find the beginning of looked for thing
             size_t pos = search_str.find(gSearchSymb);
             if (pos != std::string::npos)
             {
                 // Insert "test:" after "q="
                 pos += gSearchSymb.length();
                 //insert test: after q= so that it would be q=test:
                 search_str.insert(pos, gTestSymb);
                 std::wcout << "Modified string: " << search_str.c_str() << std::endl;
                 // Set focus to the address bar
                 
                 // Get the IValuePattern interface
                 CComPtr<IValueProvider> pValuePattern;
                 HRESULT hr = sender->GetCurrentPattern(UIA_ValuePatternId, (IUnknown**)&pValuePattern);
                 if (SUCCEEDED(hr) && pValuePattern)
                 {
                     // Set the new value
                     pValuePattern->SetValue(search_str.c_str());

                     sender->SetFocus();
                     // simulate enter
                     INPUT inputEnter = {};
                     inputEnter.type = INPUT_KEYBOARD;
                     inputEnter.ki.wVk = VK_RETURN;
                     SendInput(1, &inputEnter, sizeof(INPUT));
                     //
                     gStartSearchPressed = false; // Reset flag

                     LeaveCriticalSection(&gCrtSection);
                     return E_ABORT;
                 }
                 else 
                 {
                     std::wcout << L"Failed to get IValuePattern" << std::endl;
                 }

             }
             else
             {
                 std::cout << "Couldnt find the symbol\n";
             }
         }
         else
         {
             std::cout << "Enter was not pressed\n";
         }
         LeaveCriticalSection(&gCrtSection);
    }
    std::cout << "Return OK\n";
    return S_OK;
}

void SearchMonitor::setup(const std::pair<CComQIPtr<IUIAutomation>, CComPtr<IUIAutomationElement>>& search_bar_)
{
    search_bar_.first->AddAutomationEventHandler(UIA_Text_TextChangedEventId, search_bar_.second, TreeScope_Element, NULL, this);
}