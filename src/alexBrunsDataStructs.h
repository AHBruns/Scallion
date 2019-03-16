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
#endif // SCALLION_ALEXBRUNSDATASTRUCTS_H
