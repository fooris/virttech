//
// Created by fooris on 06.12.19.
//

#ifndef VIRTTECH_CODEGENERATORVM_H
#define VIRTTECH_CODEGENERATORVM_H


#include "../VM.h"
#include "types.h"


#define MAXSIZE 1000000

class CodeGeneratorVM : public VM {

    state_t *state;

public:
    explicit CodeGeneratorVM(const char *program, unsigned int size) : VM(program, size) {
        state = new state_t;
        state->ip = 0;
        state->l = 0;
    }

    ~CodeGeneratorVM() {
        delete state;
    }

    void run() override;

    void reset() override;

    std::string getName() override { return "CodeGeneratorVM"; }
};


#endif //VIRTTECH_CODEGENERATORVM_H
