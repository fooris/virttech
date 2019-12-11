//
// Created by fooris on 22.11.19.
//

#include "VM.h"

VM::VM(const char *program, unsigned int size) {
    this->program = program;
    this->size = size;
    a = 0;
    l = 0;
    ip = 0;
}

void VM::reset() {
    a = 0;
    l = 0;
    ip = 0;
}






