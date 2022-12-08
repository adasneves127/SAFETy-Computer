#pragma once

class Register{
    public:
        Register();
        void initialize(char name);
        void set(unsigned char val);
        unsigned char get();
        void printDebug();
    private:
        unsigned char regVal;
        char regChr;
};

class Registers{
    public:
        Registers();
        void set(int reg, unsigned char value);
        unsigned char get(int reg);
        void printDebug();
        Register* getRegister(int reg);
    private:
        Register* regs[4];
};