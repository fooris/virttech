//
// Created by fooris on 06.12.19.
//

#ifndef VIRTTECH_DIRECTVM_H
#define VIRTTECH_DIRECTVM_H

#include "../VM.h"

class DirectVM : public VM {

public:
    explicit DirectVM(const char *program, unsigned int size) : VM(program, size) {}

    void run() override;

    std::string getName() override { return "DirectVM"; }
};


#endif //VIRTTECH_DIRECTVM_H
