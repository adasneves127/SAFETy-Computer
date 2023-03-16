#include <stdint.h>
#include <iostream>
#include <string>

int parseArgs(int argc, char** argv);
int handleArgs(int argVals);

int main(int argc, char** argv)
{
    int argVals = parseArgs(argc, argv);
    int status = handleArgs(argVals);
    if(status == -1){
        // The arguments requested dictated that the program should exit
        return 0;
    }
}

int parseArgs(int argc, char** argv){
    int ret = 0;
    for(int i = 0; i < argc; i++){
        if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help"){
            ret |= 0x01;
        }
        if(std::string(argv[i]) == "-v" || std::string(argv[i]) == "--version"){
            ret |= 0x02;
        }
        if(std::string(argv[i]) == "-d" || std::string(argv[i]) == "--debug"){
            ret |= 0x04;
        }
        if(std::string(argv[i]) == "-e"){
            ret |= 0x08;
        }
        if(std::string(argv[i]) == "-m"){
            ret |= 0x20;
        }
        if(std::string(argv[i]) == "-r"){
            ret |= 0x40;
        }
    }
    return ret;
}

int handleArgs(int argVals){
    if(argVals & 0x01){
        std::cout << "SAFETy Simulator V1.2\n";
        std::cout << "Usage: SAFETy [[-e] | <ROM File> <RAM File>] [-d] [-m]\n";
        std::cout << "Options:\n";
        std::cout << "\t-h, --help\t\tDisplay this help message\n";
        std::cout << "\t-v, --version\t\tDisplay the version of the simulator\n";
        std::cout << "\t-d, --debug\t\tEnable debug mode\n";
        std::cout << "\t-e\t\t\tInitialize memory with all 0x00 (empty)\n";
        std::cout << "\t-m\t\t\tEnable monitor mode\n";
        std::cout << "\t-r\t\t\tEnable 'Raw Output' mode\n";
        return -1;
    }
    if(argVals & 0x02){
        std::cout << "SAFETy Simulator V1.2\n";
        return -1;
    }
    if(argVals & 0x04){
        std::cout << "Debug mode enabled\n";
    }
    if(argVals & 0x08){
        std::cout << "Memory initialized with all 0x00\n";
    }
    if(argVals & 0x20){
        std::cout << "Monitor mode enabled\n";
    }
    if(argVals & 0x40){
        std::cout << "Raw output mode enabled\n";
    }
    return 0;
}