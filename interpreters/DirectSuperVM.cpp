//
// Created by fooris on 06.12.19.
//

#include <array>
#include "DirectSuperVM.h"

#define VM_DIRECT_FETCH() goto *dtt[ip++]

void DirectSuperVM::run() {
    static std::array<void *, 0x0505> sit;
    if (direct_super_first) {
        // ... halt (except for back7)
        for (int i = 0; i < 5; ++i) {
            sit[i] = &&halt;
        }
        // halt ...
        for (int i = 0; i < 6; ++i) {
            sit[i << 8] = &&halt;
        }
        // back7 ...
        for (int i = 0; i < 6; ++i) {
            sit[0x0005 + (i << 8)] = &&back7;
        }
        sit[0x0101] = &&clra_clra;
        sit[0x0201] = &&clra_inc3a;
        sit[0x0301] = &&clra_deca;
        sit[0x0401] = &&clra_setl;
        sit[0x0501] = &&clra_back7;

        sit[0x0102] = &&inc3a_clra;
        sit[0x0202] = &&inc3a_inc3a;
        sit[0x0302] = &&inc3a_deca;
        sit[0x0402] = &&inc3a_setl;
        sit[0x0502] = &&inc3a_back7;

        sit[0x103] = &&deca_clra;
        sit[0x203] = &&deca_inc3a;
        sit[0x303] = &&deca_deca;
        sit[0x403] = &&deca_setl;
        sit[0x503] = &&deca_back7;

        sit[0x104] = &&setl_clra;
        sit[0x204] = &&setl_inc3a;
        sit[0x304] = &&setl_deca;
        sit[0x404] = &&setl_setl;
        sit[0x504] = &&setl_back7;
    }

    const void *dtt[size];

    for (unsigned int i = 0; i < size - 1; ++i) {
        dtt[i] = sit[*(unsigned short *) (program + i)];
    }
    dtt[size - 1] = sit[0];

    if (size > 0) VM_DIRECT_FETCH();
    else return;

    // clra ...

    clra_clra:
    a = 0;
    ++ip;
    VM_DIRECT_FETCH();

    clra_inc3a:
    a = 3;
    ++ip;
    VM_DIRECT_FETCH();

    clra_deca:
    a = -1;
    ++ip;
    VM_DIRECT_FETCH();

    clra_setl:
    l = a = 0;
    ++ip;
    VM_DIRECT_FETCH();

    clra_back7:
    a = 0;
    if (--l > 0) {
        ip -= 7;
    }
    ++ip;
    VM_DIRECT_FETCH();


    // inca ...

    inc3a_clra:
    a = 0;
    ++ip;
    VM_DIRECT_FETCH();

    inc3a_inc3a:
    a += 6;
    ++ip;
    VM_DIRECT_FETCH();

    inc3a_deca:
    a += 2;
    ++ip;
    VM_DIRECT_FETCH();

    inc3a_setl:
    a += 3;
    l = a;
    ++ip;
    VM_DIRECT_FETCH();

    inc3a_back7:
    a += 3;
    if (--l > 0) {
        ip -= 7;
    }
    ++ip;
    VM_DIRECT_FETCH();

    // deca ...

    deca_clra:
    a = 0;
    ++ip;
    VM_DIRECT_FETCH();

    deca_inc3a:
    a += 2;
    ++ip;
    VM_DIRECT_FETCH();

    deca_deca:
    a -= 2;
    ++ip;
    VM_DIRECT_FETCH();

    deca_setl:
    l = --a;
    ++ip;
    VM_DIRECT_FETCH();

    deca_back7:
    --a;
    if (--l > 0) {
        ip -= 7;
    }
    ++ip;
    VM_DIRECT_FETCH();

    //setl ...

    setl_clra:
    l = a;
    a = 0;
    ++ip;
    VM_DIRECT_FETCH();

    setl_inc3a:
    l = a;
    a += 3;
    ++ip;
    VM_DIRECT_FETCH();

    setl_deca:
    l = a;
    --a;
    ++ip;
    VM_DIRECT_FETCH();

    setl_setl:
    l = a;
    ++ip;
    VM_DIRECT_FETCH();

    setl_back7:
    l = a;
    if (--l > 0) {
        ip -= 7;
    }
    ++ip;
    VM_DIRECT_FETCH();

    back7:
    if (--l > 0) {
        ip -= 7;
    }
    VM_DIRECT_FETCH();

    halt:
    return;
}
