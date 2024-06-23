#pragma once

#include <utility>

#include <Windows.h>
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>

//void SetKeyboardHook();
//void UnhookKeyboardHook();

class SearchMonitor : public IUIAutomationEventHandler
{
public:

	SearchMonitor();

	~SearchMonitor();

    void setup(const std::pair<CComQIPtr<IUIAutomation>, CComPtr<IUIAutomationElement>>& search_bar_);

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);

    HRESULT STDMETHODCALLTYPE HandleAutomationEvent(IUIAutomationElement* sender, EVENTID eventId);
private:
    LONG _refCount;
};