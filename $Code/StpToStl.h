
#pragma once

#include <windows.h>
#include <string>


extern bool is_verbose;

extern int StpToStl(const std::wstring in_file, const std::wstring out_file, double deflection = 0, bool ascii = false);

extern bool get_number(char *str, double& value);
extern bool check_file_name(const std::string current_path, std::string &file_name);
extern std::string AnsiToUtf8String(const std::string value, DWORD locale = CP_ACP);
extern std::wstring AnsiToWideString(const std::string value, UINT code_page = CP_ACP);
extern std::string WideToUtf8String(const std::wstring value);




