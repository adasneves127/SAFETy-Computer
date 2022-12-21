#include "Stack.h"

Stack::Stack(uint16_t len){
    this->data = (uint8_t*)malloc(len);
}

Stack::Stack(){
    this->data = (uint8_t*)malloc(0x8000);
}

void Stack::push(uint8_t val){
    *(this->data) = val;
    this->data++;
}

uint8_t Stack::pop(){
    this->data--;
    return *(this->data);
}