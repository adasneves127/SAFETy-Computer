#include <stdint.h>
#include "Registers.h"
#pragma once

class Memory{
    public:
        Memory();
        void jump(uint16_t address);
        void load(uint16_t address, Register* RD);
        void store(uint16_t address, Register* RS);
        void put(uint16_t address, uint8_t data);
        void push(Register* RS);
        void pop(Register* RD);
        void call(uint16_t address);
        uint8_t nextIns();

        void loadRAM(char* fileName);
        void loadROM(char* fileName);
        void loadHeaders(char* fileName);

        uint8_t read(uint16_t address);

        uint16_t getPC();

    private:
        uint8_t RAM[0x8000];
        uint8_t ROM[0x8000];
        uint8_t HEADS[0x8000];
        uint8_t FUNCTABLE[0x8000];
        uint8_t stackPointer, stackPage;
        uint8_t memPageL, memPageH;
        uint8_t chrToPrint;
        void changePage();
        char* romFile;
        bool is_call = true;
        uint16_t PC;
};