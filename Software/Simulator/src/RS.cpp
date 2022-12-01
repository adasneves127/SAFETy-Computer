#include "RS.h"

bool RS::get(){
    return Q;
}

void RS::set(){
    Q = true;
}

void RS::reset(){
   Q = false; 
}