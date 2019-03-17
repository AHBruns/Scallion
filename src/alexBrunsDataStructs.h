//
// Created by Alex Bruns on 3/9/19.
//

// library includes
#include <string.h>

// local includes
#include "exprs.h"

#ifndef SCALLION_ALEXBRUNSDATASTRUCTS_H
#define SCALLION_ALEXBRUNSDATASTRUCTS_H
struct NODE {
    void * index;
    struct NAMED_TOKEN * token;
    struct NODE * next;
};

struct PAREN_PAIR {
    char * beginning;
    char * ending;
    int depth;
};

// BINDINGS
struct BINDING {
    char name[79]; // if your variable names are longer than 79 characters you can go fuck yourself
    struct NAMED_TOKEN * expr;
};
#endif // SCALLION_ALEXBRUNSDATASTRUCTS_H
