#include "Memory.h"
#include "Registers.h"
#include "ALU.h"
#include "Control.h"


class Monitor{
    public:
        Monitor();
        void init(Memory* _mem, Registers* _reg, ALU* _alu, controlUnit* _control);
        void run();
    private:
        Memory* _mem;
        Registers* _reg;
        ALU* _alu;
        controlUnit* _control;
};