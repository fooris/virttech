//
// Created by fooris on 06.12.19.
//

#ifndef VIRTTECH_NAIVEVM_H
#define VIRTTECH_NAIVEVM_H

#include "../VM.h"

class NaiveVM : public VM {

public:
    const std::string kName = "NaiveVM";

    explicit NaiveVM(const char *program, unsigned int size) : VM(program, size) {}

    void run() override;

    std::string getName() override { return "NaiveVM";}
};


#endif //VIRTTECH_NAIVEVM_H
