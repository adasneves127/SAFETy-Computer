#include "Registers.h"
#include "Memory.h"
#include <iostream>
#include <string>
#include "Monitor.h"
#include "ALU.h"
#include "Control.h"
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>


Registers* _reg;
Memory* _mem;
Monitor* _mon;
ALU* _alu;
controlUnit* _control;


uint8_t options = 0;
char* fileNames[3];

void loadRam(char* fileName);
void loadRom(char* fileName);
void loadHeaders(char* fileName);

int main(int argc, char** argv){

    for(int i = 1; i < argc; i++){
        if(std::string(argv[i]) == "-h"){
            std::cout << "SAFETy Simulator\n";
            std::cout << "Usage: SAFETy [[-e] | <ROM File> <RAM File> <Headers File>] [-d] [-m]\n";
            std::cout << "Options:\n";
            std::cout << "\t-e\t\tInitialize the Simulator with empty memory contents\n";
            std::cout << "\t-d\t\tEnable Debug Mode\n";
            std::cout << "\t-m\t\tEnable Monitor Mode\n";
            std::cout << "\t-r\t\tEnable Raw Text Mode\n";
            return 0;
        }
        if(std::string(argv[i]) == "-m"){
            options |= 0b00000001;
        }
        if(std::string(argv[i]) == "-d"){
            options |= 0b00000010;
        }
        if(std::string(argv[i]) == "-e"){
            options |= 0b00000100;
        }
        if(std::string(argv[i]) == "-r"){
            options |= 0b00001000;
        }
    }
    if(argc < 4 && !(options & 0b00000100)){
        std::cout <<"\u001b[31mErr:\u001b[0m Invalid Arguments\n";
        return EXIT_FAILURE;
    }

    if((options & 0b00000100)){
        fileNames[0] = (char*)"/dev/zero";
        fileNames[1] = (char*)"/dev/zero";
        fileNames[2] = (char*)"/dev/zero";
    } else {
        fileNames[0] = argv[1];
        fileNames[1] = argv[2];
        fileNames[2] = argv[3];
    }

    _reg = new Registers();

    _mem = new Memory();
    if(options & 0b00001000){
        _mem->enableRaw();
    }

    _alu = new ALU();

    _control = new controlUnit(_reg, _alu, _mem);

    std::thread RamThread(loadRam, fileNames[1]);
    std::thread RomThread(loadRom, fileNames[0]);
    std::thread HeadThread(loadHeaders, fileNames[2]);

    RamThread.join();
    RomThread.join();
    HeadThread.join();

    if(options & 0b00000001){
        _mon = new Monitor();
        _mon->isDebug = (options & 0b00000010) >> 1;
        _mon->init(_mem, _reg, _alu, _control);
        uint8_t res = _mon->run();
        if(res == 255){
            printf("Thank you for using SAFETy!\n");
        }
    }

    //Save the files!
    uint8_t fileBuffer[0x8000];
    


    delete _reg;
    delete _mem;
    delete _alu;
    delete _control;
    delete _mon;
}

void loadRam(char* fileName){
    _mem->loadRAM(fileName);
}
void loadRom(char* fileName){
    _mem->loadROM(fileName);
}
void loadHeaders(char* fileName){
    _mem->loadHeaders(fileName);
}

