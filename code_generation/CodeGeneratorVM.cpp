//
// Created by fooris on 06.12.19.
//

#include "CodeGeneratorVM.h"
#include "TranslationUnit.h"

void CodeGeneratorVM::run() {
    TranslationUnit translationUnit(program, size);
    int i = 0;
    while (state->ip < size) {
        code_t bb = translationUnit.GetCode(state->ip);
        if (!bb) {
            translationUnit.TranslateBasicBlock(state->ip);
//            translationUnit.PrintGeneratedBasicBlock(state->ip);
            bb = translationUnit.GetCode(state->ip);
        }
//        std::cout << "run bb at " << state->ip << "(= " << (int*) bb << ")" <<std::endl;
        a = bb(a, state);
//        std::cout << "a = " << a << " ip = " << state->ip << std::endl;
//        std::cout << "---------------------\n";

    }
    l = state->l;
    ip = state->ip;
}

void CodeGeneratorVM::reset() {
    VM::reset();
    state->ip = 0;
    state->l = 0;
}
