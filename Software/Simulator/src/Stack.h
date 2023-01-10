#pragma once

#include <stdint.h>
#include <stdlib.h>

class Stack{
    public:
        Stack();
        Stack(uint16_t len);
        void push(uint8_t data);
        uint8_t pop();
    private:
        uint8_t* data;
        uint16_t size;
};