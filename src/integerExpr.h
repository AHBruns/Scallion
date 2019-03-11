//
// Created by Alex Bruns on 3/9/19.
//

#ifndef SCALLION_INTEGEREXPR_H
#define SCALLION_INTEGEREXPR_H
struct NAMED_TOKEN;

struct CONST   { int v; };
struct PBIND   { char * lh; struct NAMED_TOKEN * rh; };
struct MBIND   { char * lh; struct NAMED_TOKEN * rh; };
struct MODBIND { char * lh; struct NAMED_TOKEN * rh; };
struct DIVBIND { char * lh; struct NAMED_TOKEN * rh; };
struct MULBIND { char * lh; struct NAMED_TOKEN * rh; };
struct BIND    { char * lh; struct NAMED_TOKEN * rh; };
struct EQ      { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct GT      { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct LT      { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct MOD     { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct PLUS    { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct SUB     { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct DIV     { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct FDIV    { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct MUL     { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct POW     { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };

union TOKEN {
    struct CONST *Const;
    struct DEC *Dec;
    struct INC *Inc;
    struct PBIND *Pbind;
    struct MBIND *Mbind;
    struct MODBIND *Modbind;
    struct DIVBIND *Divbind;
    struct MULBIND *Mulbind;
    struct BIND *Bind;
    struct EQ *Eq;
    struct GT *Gt;
    struct LT *Lt;
    struct MOD *Mod;
    struct PLUS *Plus;
    struct SUB *Sub;
    struct DIV *Div;
    struct MUL *Mul;
    struct POW *Pow;
    struct FDIV *Fdiv;
};

struct NAMED_TOKEN {
    char name[10];
    union TOKEN *token;
};
#endif //SCALLION_INTEGEREXPR_H
