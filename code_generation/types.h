//
// Created by fooris on 08.12.19.
//

#ifndef VIRTTECH_TYPES_H
#define VIRTTECH_TYPES_H

struct state_t {
    unsigned int ip;
    int l;
};

typedef int (*code_t)(int, state_t*);

#endif //VIRTTECH_TYPES_H
