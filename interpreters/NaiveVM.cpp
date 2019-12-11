//
// Created by fooris on 06.12.19.
//

#include "NaiveVM.h"


void NaiveVM::run() {
    while (true) {
        switch (program[ip++]) {
            case OP_CLRA:
                a = 0;
                break;
            case OP_INC3A:
                a += 3;
                break;
            case OP_DECA:
                --a;
                break;
            case OP_SETL:
                l = a;
                break;
            case OP_BACK7:
                if (--l > 0) ip -= 7;
                break;
            case OP_HALT:
                return;
            default:
                return;
        }
    }
}
