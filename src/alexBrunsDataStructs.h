//
// Created by Alex Bruns on 3/9/19.
//

// local includes
#include "integerExpr.h"

#ifndef SCALLION_ALEXBRUNSDATASTRUCTS_H
#define SCALLION_ALEXBRUNSDATASTRUCTS_H
struct NODE {
    void * index;
    struct NAMED_TOKEN * token;
    struct NODE * next;
};
#endif //SCALLION_ALEXBRUNSDATASTRUCTS_H
