//
// Created by Alex Bruns on 3/11/19.
//

#ifndef SCALLION_RESULTS_H
#define SCALLION_RESULTS_H
struct NAMED_RESULT;

struct R_INT    { int v; };
struct R_FLOAT  { double v; };
struct R_CHAR   { char v; };
struct R_BOOL   { int v; }; // 1 or 0
struct R_STRING { char * v; };
struct R_LIST   { union RESULT * v; };

union RESULT {
    struct R_FLOAT Float;
    struct R_INT Int;
    struct R_CHAR Char;
    struct R_BOOL Bool;
    struct R_STRING String;
    struct R_LIST List;
};

struct NAMED_RESULT {
    char name[10];
    union RESULT result;
};
#endif //SCALLION_RESULTS_H
