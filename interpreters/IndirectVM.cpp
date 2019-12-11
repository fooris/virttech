//
// Created by fooris on 06.12.19.
//

#include "IndirectVM.h"

#define VM_INDIRECT_FETCH() goto *jump[program[ip++]]

void IndirectVM::run() {
    static const void *jump[] = {&&halt, &&clra, &&inc3a, &&deca, &&setl, &&back7};


    VM_INDIRECT_FETCH();

    clra:
    a = 0;
    VM_INDIRECT_FETCH();

    inc3a:
    a += 3;
    VM_INDIRECT_FETCH();

    deca:
    --a;
    VM_INDIRECT_FETCH();

    setl:
    l = a;
    VM_INDIRECT_FETCH();

    back7:
    if (--l > 0) ip -= 7;
    VM_INDIRECT_FETCH();

    halt:
    return;
}
