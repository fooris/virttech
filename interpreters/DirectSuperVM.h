//
// Created by fooris on 06.12.19.
//

#ifndef VIRTTECH_DIRECTSUPERVM_H
#define VIRTTECH_DIRECTSUPERVM_H

#include "../VM.h"

class DirectSuperVM : public VM {
    bool direct_super_first;

public:
    explicit DirectSuperVM(const char *program, unsigned int size) : VM(program, size) {
        direct_super_first = true;
    }
    void run() override;

    std::string getName() override { return "DirectSuperVM"; }
};


#endif //VIRTTECH_DIRECTSUPERVM_H
