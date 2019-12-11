//
// Created by fooris on 06.12.19.
//

#ifndef VIRTTECH_INDIRECTVM_H
#define VIRTTECH_INDIRECTVM_H

#include "../VM.h"

class IndirectVM : public VM {

public:
    explicit IndirectVM(const char *program, unsigned int size) : VM(program, size) {}

    void run() override;

    std::string getName() override { return "IndirectVM"; }
};


#endif //VIRTTECH_INDIRECTVM_H
