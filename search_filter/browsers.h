#pragma once
#include <string>


enum class eBrowsers : int
{
	eMOZILLA,
	eCHROME,
	eEDGE,
	eUnknown // To handle wrap-around or end condition
};

eBrowsers operator++(eBrowsers& value, int);

const std::wstring& getHwndNameOfBrowser(eBrowsers brwsr_);