#include "Keyboard.h"
#include <thread>
#include <iostream>


Keyboard::Keyboard(){
    std::thread RamThread(loop, NULL);
}

void Keyboard::loop(){
    int inputIdx = 0;
    while(!isQuit){
        char inputChar;
        std::cin.getline(&inputChar, 1);
        this->keyQueue[inputIdx++] = inputChar;
    }
}

char Keyboard::getChar(){
    if(inputIdx == outputIdx){
        return 0;
    }

    return this->keyQueue[outputIdx++];
}