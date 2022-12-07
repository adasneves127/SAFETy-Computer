#include <string>
#include "tools.h"
#include <stdio.h>


//From String to int
int tools::StringToInt(std::string str){
    int num = 0;
    for(int i = 0; i < str.length(); i++){
        num += (int)str[i];
    }
    return num;
}

//From int to String
std::string tools::IntToString(int num){
    std::string str = "";
    for(int i = 0; i < num; i++){
        str += (char)num;
    }
    return str;
}

//From int to hex string
std::string tools::IntToHexString(int num){
    std::string str = "";
    for(int i = 0; i < num; i++){
        str += (char)num;
    }
    return str;
}

//From hex string to int
int tools::HexStringToInt(std::string str){
    printf("Hex: %s\n", str.c_str());
    int num = 0;
    for(int i = 0; i < str.length(); i++){
        int tmp = tolower(str[i]);
        printf("tmp: %c\n", tmp);
        printf("tmp - '0': %d\n", tmp - '0');
        printf("num: %d\n", num);
        num *= 16;
        if(tmp - '0' > 9){
            tmp -= 'a';
            tmp += 10;
        }
        num += tmp;
    }
    return num;
}