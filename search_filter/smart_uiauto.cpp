#include "smart_uiauto.h"

#include <iostream>

#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>


SmartUiAuto::SmartUiAuto()
{
	CoInitialize(NULL);
}

SmartUiAuto::~SmartUiAuto()
{
	CoUninitialize();
}


std::pair<CComQIPtr<IUIAutomation>, CComPtr<IUIAutomationElement>> SmartUiAuto::getSearchBar(const std::wstring& browser_)
{
    while (true)
    {
        _hwnd = FindWindowEx(0, _hwnd, browser_.c_str(), NULL);
        if (!_hwnd)
        {
            std::cout << "Couldnt find the opened " << browser_.c_str() << " window" << std::endl;
            break;
        }
        if (!IsWindowVisible(_hwnd))
            continue;

        CComQIPtr<IUIAutomation> uia;
        if (FAILED(uia.CoCreateInstance(CLSID_CUIAutomation)) || !uia)
            break;

        CComPtr<IUIAutomationElement> root;
        if (FAILED(uia->ElementFromHandle(_hwnd, &root)) || !root)
            break;

        CComPtr<IUIAutomationCondition> condition;


        uia->CreatePropertyCondition(UIA_ControlTypePropertyId,
            CComVariant(0xC354), &condition);


        CComPtr<IUIAutomationElement> edit;
        if (FAILED(root->FindFirst(TreeScope_Descendants, condition, &edit))
            || !edit)
            continue; //maybe we don't have the right tab, continue...

        CComVariant url;
        edit->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &url);
        MessageBoxEx(0, url.bstrVal, L"Adress and search box value", 0, 0);
        
        return { uia, edit };
    }
}