//From String to int
#include <string>
#pragma once

class tools{
    public:
        static int StringToInt(std::string str);
        static std::string IntToString(int num);
        static std::string IntToHexString(int num);
        static int HexStringToInt(std::string str);
};