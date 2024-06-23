#include "cmdparser.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include <Windows.h>

#include "browsers.h"


CmdParser::CmdParser(int argc, char* argv[])
{
	if (argc > 1)
	{
		_brw_str = argv[1];
	}
	else
	{
		std::cout << "No browser provided; Selecting default browser\n";
	}
}

CmdParser::~CmdParser()
{

}

std::vector<std::function<bool(const std::wstring&)>> gfuncs =
{
	[](const std::wstring& input) -> bool
	{
		return (input.find(L"mozilla") != std::wstring::npos);
	},

	[](const std::wstring& input) -> bool
	{
		// Check for "chrome" and "mozilla"
		return (input.find(L"chrome") != std::wstring::npos);
	},

	[](const std::wstring& input) -> bool
	{
		return (input.find(L"edge") != std::wstring::npos);
	}
};


std::wstring GetRegistryValue(HKEY hKeyRoot, const std::wstring& subKey, const std::wstring& valueName)
{
	HKEY hKey;
	if (RegOpenKeyEx(hKeyRoot, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		return L"";
	}

	DWORD dataSize = 0;
	if (RegQueryValueEx(hKey, valueName.c_str(), nullptr, nullptr, nullptr, &dataSize) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return L"";
	}

	std::wstring value(dataSize / sizeof(wchar_t), L'\0');
	if (RegQueryValueEx(hKey, valueName.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&value[0]), &dataSize) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return L"";
	}

	RegCloseKey(hKey);

	// Remove any trailing null characters
	size_t pos = value.find_last_not_of(L'\0');
	if (pos != std::wstring::npos) {
		value.resize(pos + 1);
	}

	return value;
}

std::wstring GetDefaultBrowser() {
	std::wstring progId = GetRegistryValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice", L"ProgId");
	if (progId.empty()) {
		return L"Unable to determine the default browser.";
	}

	if (progId == L"ChromeHTML") {
		return L"Google Chrome";
	}
	else if (progId == L"FirefoxURL") {
		return L"Mozilla Firefox";
	}
	else if (progId == L"IE.HTTP") {
		return L"Internet Explorer";
	}
	else if (progId == L"MSEdgeHTM") {
		return L"Microsoft Edge";
	}
	else {
		return L"Unknown browser: " + progId;
	}
}

std::wstring getHwndName(const std::wstring& wstr_)
{
	std::wstring to_lower_wstr;
	to_lower_wstr.resize(wstr_.size());

	std::transform(wstr_.begin(), wstr_.end(), to_lower_wstr.begin(), ::towlower);

	std::wstring name;
	eBrowsers count = eBrowsers::eMOZILLA;
	// iterate through function and increase the counter
	// the time when the check return true is the enum for the hwnd name of browser
	for (auto checker : gfuncs)
	{
		if (checker(to_lower_wstr))
		{
			name = getHwndNameOfBrowser(count);
			break;
		}
		count++;
	}

	return name;
}

const std::wstring& CmdParser::getBrowser()
{
	std::wstring wstr = std::wstring(_brw_str.begin(), _brw_str.end());
	// firstly we analuze whther the input from the shell contains any mention about 
	// current browser
	_name = getHwndName(wstr);

	// however if the cmd input is doesnt contain any sane browser name
	// we are going to look for in the registry fo the system defualt browser
	if (_name.empty())
	{
		std::cout << "Selected default browser \n";
		std::wstring def_brwsr = GetDefaultBrowser();
		_name = getHwndName(def_brwsr);
	}

	return _name;
}