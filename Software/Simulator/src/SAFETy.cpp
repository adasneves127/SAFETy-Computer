#include "Registers.h"
#include "Memory.h"
#include <stdio.h>

Registers* _reg;
Memory* _mem;

int main(int argc, char** argv){
    _reg = new Registers();

    _reg->printDebug();

    _reg->set(0, 14);
    _reg->printDebug();
}