#pragma once

#include <string>


class CmdParser
{
public:

	CmdParser(int argc, char* argv[]);
	~CmdParser();

	const std::wstring& getBrowser();

private:
	std::string _brw_str;
	std::wstring _name;
};