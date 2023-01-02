#pragma once
#include <stdint.h>

class Register{
    public:
        Register();
        void initialize(char name);
        void set(uint8_t val);
        uint8_t get();
        void printDebug();
        char getName();
    private:
        uint8_t regVal;
        char regChr;
};

class Registers{
    public:
        Registers();
        void set(int reg, uint8_t value);
        uint8_t get(int reg);
        void printDebug();
        Register* getRegister(int reg);
        void reset();
    private:
        Register* regs[4];
};