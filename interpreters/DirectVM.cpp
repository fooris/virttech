//
// Created by fooris on 06.12.19.
//

#include "DirectVM.h"

#define VM_DIRECT_FETCH() goto *dtt[ip++]

void DirectVM::run() {
    static const void *jump[] = {&&halt_d, &&clra_d, &&inc3a_d, &&deca_d, &&setl_d, &&back7_d};

    const void *dtt[size];

    for (unsigned int i = 0; i < size; ++i) {
        dtt[i] = jump[program[i]];
    }

    VM_DIRECT_FETCH();

    clra_d:
    a = 0;
    VM_DIRECT_FETCH();

    inc3a_d:
    a += 3;
    VM_DIRECT_FETCH();

    deca_d:
    --a;
    VM_DIRECT_FETCH();

    setl_d:
    l = a;
    VM_DIRECT_FETCH();

    back7_d:
    if (--l > 0) ip -= 7;
    VM_DIRECT_FETCH();

    halt_d:
    return;
}
