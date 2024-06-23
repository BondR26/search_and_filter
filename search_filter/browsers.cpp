#include "browsers.h"

#include <map>

const std::wstring MOZILLA = L"MozillaWindowClass";
const std::wstring CHROME  = L"Chrome_WidgetWin_1";
const std::wstring EDGE    = L"Chrome_WidgetWin_1";

std::map<eBrowsers, std::wstring> gNames =
{
	{eBrowsers::eMOZILLA, MOZILLA },
	{eBrowsers::eCHROME , CHROME },
	{eBrowsers::eEDGE   , EDGE	}
};

eBrowsers operator++(eBrowsers& value, int)
{
	value = (value == eBrowsers::eEDGE) ? eBrowsers::eMOZILLA : static_cast<eBrowsers>(static_cast<int>(value) + 1);
	return value;
}

const std::wstring& getHwndNameOfBrowser(eBrowsers brwsr_)
{
	return gNames[brwsr_];
}