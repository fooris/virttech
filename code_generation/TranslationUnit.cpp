//
// Created by fooris on 07.12.19.
//

#include <cassert>
#include <iostream>
#include <iomanip>
#include "TranslationUnit.h"
#include "../VM.h"

TranslationUnit::TranslationUnit(const char *program, unsigned int size) {
    this->program = program;
    program_size = size;

    code_cache = (uint8_t *) mmap(nullptr, kSize, PROT_READ | PROT_WRITE | PROT_EXEC,
                                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    position = code_cache;
}

TranslationUnit::~TranslationUnit() {
    munmap(code_cache, kSize);
}

uint8_t *TranslationUnit::writeHALT(uint8_t *p) {
    // movl   $"program_size", (%rsi)
    *(p++) = 0xc7;
    *(p++) = 0x06;
    *((unsigned int *) p) = program_size;
    p += 4;
    return p;
}

uint8_t *TranslationUnit::writeCLRA(uint8_t *p) {
    // "xor %eax, %eax": set %eax to zero
    *(p++) = 0x31;
    *(p++) = 0xc0;
    return p;
}

uint8_t *TranslationUnit::writeINC3A(uint8_t *p) {
    // "addl %eax, 3": increment %eax by 3
    *(p++) = 0x83;
    *(p++) = 0xc0;
    *(p++) = 3;
    return p;
}

uint8_t *TranslationUnit::writeDECA(uint8_t *p) {
    // "dec %eax": decrement lower 32bit of %rax
    *(p++) = 0xff;
    *(p++) = 0xc8;
    return p;
}

uint8_t *TranslationUnit::writeSETL(uint8_t *p) {
//    // set ms->loop to value of acc: mov %eax,4(%rsi)
//    *(p++) = 0x89;
//    *(p++) = 0x46;
//    *(p++) = 0x04;

    // mov ecx, eax
    *(p++) = 0x89;
    *(p++) = 0xc1;
    return p;
}

uint8_t *TranslationUnit::writeBACK7(uint8_t *p, const uint8_t *target) {

    // dec ecx
    *(p++) = 0xff;
    *(p++) = 0xc9;

    // cmp ecx, 0
    *(p++) = 0x83;
    *(p++) = 0xF9;
    *(p++) = 0x00;

    // jg $diff
    char diff = (char) (target - p - 2);
    *(p++) = 0x7f;
    *(p++) = diff;

    return p;
}

//TODO test
// returns: Index pointing to the first instruction of the next block.
unsigned int TranslationUnit::getStartNextBlock(unsigned int instruction_pointer) {
    unsigned int i;
    for (i = instruction_pointer; i < program_size; ++i) {
        if (program[i] == OP_BACK7) {
            // Is there a jump within the next 6 instructions?
            // i.e. a jump that goes into this BB?
            bool last = true;
            for (unsigned int j = 1; j < 7 && i + j < program_size; ++j) {
                if (program[i + j] == OP_BACK7) {
                    i += j - 1;
                    last = false;
                    break;
                }
            }
            if (last) {
                i++;
                break;
            }
        }
    }
    return i;
}

//TODO test
uint8_t *TranslationUnit::Translate(unsigned int from, unsigned int to, uint8_t *p) {
    //// CHECK if enough space is available
    if (code_cache + kSize <= p + (to - from) *  7){
        std::cerr << "Buffer too full, exiting" << std::endl;
        exit(4242);
    }
    static uint8_t *jump_map[6];
    int c = 0;

    for (unsigned int i = from; i < to; ++i) {
        switch (program[i]) {
            case OP_HALT:
                p = writeHALT(p);
                break;

            case OP_CLRA:
                p = writeCLRA(p);
                break;

            case OP_INC3A:
                p = writeINC3A(p);
                break;

            case OP_DECA:
                p = writeDECA(p);
                break;

            case OP_SETL:
                p = writeSETL(p);
                break;

            case OP_BACK7:
                p = writeBACK7(p, jump_map[(c + 1) % 6]);
                break;

            default:
                assert(0); // should not happen
        }
        jump_map[c++] = p;
        if (c > 5) c = 0;
    }
    return p;
}

//TODO test
void TranslationUnit::TranslateBasicBlock(unsigned int instruction_pointer) {
    // Create new entry in translation table.
    translation_table[instruction_pointer] = (code_t) position;

    // Setup correct state mapping: acc got passed in edi, needs to be in eax
    // "mov %edi, %eax": move 32bit par into %eax (= lower 32bit of %rax).
    *(position++) = 0x89;
    *(position++) = 0xf8;
    // push rcx (not sure if needed)
    *(position++) = 0x51;
    // mov ecx, [rsi + 0x4]
    *(position++) = 0x8b;
    *(position++) = 0x4e;
    *(position++) = 0x04;

    // Find jump targets and until where to translate. ("Composite Basic Block")
    unsigned int start_next_block = getStartNextBlock(instruction_pointer);
    position = Translate(instruction_pointer, start_next_block, position);

    // mov [rsi + 0x4], ecx
    *(position++) = 0x89;
    *(position++) = 0x4e;
    *(position++) = 0x04;

    // pop rcx
    *(position++) = 0x59;

    // Set instruction pointer.
    // movl $instruction_pointer, (%rsi)
    *(position++) = 0xc7;
    *(position++) = 0x06;
    *((unsigned int *) position) = start_next_block;
    position += 4;

    // "ret"
    *(position++) = 0xC3;
}

code_t TranslationUnit::GetCode(unsigned int instruction_pointer) {
    return translation_table[instruction_pointer];
}

void TranslationUnit::PrintBasicBlock(unsigned int instruction_pointer) {
    for (; instruction_pointer < program_size; ++instruction_pointer) {
        switch (program[instruction_pointer]) {
            case OP_HALT:
                std::cout << "HALT" << std::endl;
                return;
            case OP_INC3A:
                std::cout << "INC3a" << std::endl;
                break;
            case OP_CLRA:
                std::cout << "CLRA" << std::endl;
                break;
            case OP_DECA:
                std::cout << "DECA" << std::endl;
                break;
            case OP_SETL:
                std::cout << "SETL" << std::endl;
                break;
            case OP_BACK7:
                std::cout << "BACK7" << std::endl;
                return;
            default:
                assert(0);
        }
    }
}

void TranslationUnit::PrintGeneratedBasicBlock(unsigned int instruction_pointer) {
    auto cur = (uint8_t *) translation_table[instruction_pointer];
    int i = 0;
    while (*(cur) != 0xC3) {
        std::cout << std::setw(2) << std::hex << std::internal << std::setfill('0') << (int) *cur << " ";
        if (++i % 4 == 0) std::cout << std::endl;
        cur++;
    }
    std::cout << std::hex << (int) *cur << std::endl;
}

