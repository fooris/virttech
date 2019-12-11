//
// Created by fooris on 07.12.19.
//

#ifndef VIRTTECH_TRANSLATIONUNIT_H
#define VIRTTECH_TRANSLATIONUNIT_H

#include <sys/mman.h>
#include <cstdint>
#include <map>
#include <vector>
#include "types.h"


class TranslationUnit {

    const char *program;
    unsigned int program_size;

    uint8_t *code_cache;
    uint8_t *position;
    const unsigned int kSize = 128 * 1024 * 1024;

    std::map<unsigned int, code_t> translation_table;

    uint8_t *writeHALT(uint8_t *p);

    static uint8_t *writeCLRA(uint8_t *p);

    static uint8_t *writeINC3A(uint8_t *p);

    static uint8_t *writeDECA(uint8_t *p);

    static uint8_t *writeSETL(uint8_t *p);

    static uint8_t *writeBACK7(uint8_t *p, const uint8_t *target);

    unsigned int getStartNextBlock(unsigned int instruction_pointer);

    uint8_t *Translate(unsigned int from, unsigned int to, uint8_t *p);

public:
    TranslationUnit(const char *program, unsigned int size);

    ~TranslationUnit();

    void TranslateBasicBlock(unsigned int instruction_pointer);

    void PrintBasicBlock(unsigned int instruction_pointer);

    code_t GetCode(unsigned int instruction_pointer);

    void PrintGeneratedBasicBlock(unsigned int instruction_pointer);


};


#endif //VIRTTECH_TRANSLATIONUNIT_H
