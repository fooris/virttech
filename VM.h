//
// Created by fooris on 22.11.19.
//

#ifndef VIRTTECH_VM_H
#define VIRTTECH_VM_H

#include <string>

enum Instruction {OP_HALT, OP_CLRA, OP_INC3A, OP_DECA, OP_SETL, OP_BACK7};

class VM {
protected:
    const char *program;
    unsigned int size;
    unsigned int ip;
    int a;
    int l;


public:
    explicit VM(const char *program, unsigned int size);

    virtual void run() = 0;

    virtual void reset();

    virtual std::string getName() = 0;
    virtual int getA() {return a;}
    virtual int getL() { return l;}
    virtual unsigned int getIP() { return ip;}

};


#endif //VIRTTECH_VM_H
