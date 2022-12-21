#include "Memory.h"
#include "Registers.h"
#include "ALU.h"
#include "Control.h"


class Monitor{
    public:
        Monitor();
        int isDebug = 0;
        void init(Memory* _mem, Registers* _reg, ALU* _alu, controlUnit* _control);
        void run();
    private:
        Memory* _mem;
        Registers* _reg;
        ALU* _alu;
        controlUnit* _control;
        void doInstruction();
};