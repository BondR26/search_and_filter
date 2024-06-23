#pragma once

#include <string>
#include <utility>

#include <Windows.h>
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>


class SmartUiAuto
{
public:
	SmartUiAuto();
	~SmartUiAuto();

	std::pair<CComQIPtr<IUIAutomation>, CComPtr<IUIAutomationElement>> getSearchBar(const std::wstring& browser_);

private:
	HWND _hwnd = NULL;
};