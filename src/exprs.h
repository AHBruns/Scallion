//
// Created by Alex Bruns on 3/9/19.
//

#ifndef SCALLION_EXPRS_H
#define SCALLION_EXPRS_H
struct NAMED_TOKEN;

struct CONST   { int v; };
struct BCONST  { int v; };
struct FCONST  { double v; };
struct SCONST  { char * v; };
struct CCONST  { char v; };
struct LCONST  { char * type; int size; struct NAMED_TOKEN * v; };
struct PBIND   { char * lh; struct NAMED_TOKEN * rh; };
struct MBIND   { char * lh; struct NAMED_TOKEN * rh; };
struct MODBIND { char * lh; struct NAMED_TOKEN * rh; };
struct DIVBIND { char * lh; struct NAMED_TOKEN * rh; };
struct MULBIND { char * lh; struct NAMED_TOKEN * rh; };
struct BIND    { char * lh; struct NAMED_TOKEN * rh; };
struct SEQ     { int size; struct NAMED_TOKEN * exprs; };
struct NOT     { struct NAMED_TOKEN * v; };
struct OR      { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
struct AND     { struct NAMED_TOKEN * lh; struct NAMED_TOKEN * rh; };
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
struct FUNC1   {
    char * name;
    struct NAMED_TOKEN * arg1;
    struct NAMED_TOKEN * exprs; // should be an SEQ
};
struct FUNC2   {
    char * name;
    struct NAMED_TOKEN * arg1;
    struct NAMED_TOKEN * arg2;
    struct NAMED_TOKEN * exprs;
};
struct FUNC3   {
    char * name;
    struct NAMED_TOKEN * arg1;
    struct NAMED_TOKEN * arg2;
    struct NAMED_TOKEN * arg3;
    struct NAMED_TOKEN * exprs;
};

union TOKEN {
    struct CONST *Const;
    struct BCONST *Bconst; // TODO
    struct FCONST Fconst; // TODO
    struct SCONST Sconst; // TODO
    struct CCONST Cconst; // TODO
    struct LCONST Lconst; // TODO
    struct DEC *Dec; // TODO
    struct INC *Inc; // TODO
    struct PBIND *Pbind; // TODO
    struct MBIND *Mbind; // TODO
    struct MODBIND *Modbind; // TODO
    struct DIVBIND *Divbind; // TODO
    struct MULBIND *Mulbind; // TODO
    struct BIND *Bind; // TODO
    struct EQ *Eq; // TODO
    struct GT *Gt; // TODO
    struct LT *Lt; // TODO
    struct MOD *Mod;
    struct PLUS *Plus;
    struct SUB *Sub;
    struct DIV *Div;
    struct MUL *Mul;
    struct POW *Pow;
    struct FDIV *Fdiv;
    struct NOT *Not; // TODO
    struct SEQ *Seq; // TODO
    struct OR *Or; // TODO
    struct AND *And; // TODO
    struct FUNC1 *Func1; // TODO
    struct FUNC2 *Func2; // TODO
    struct FUNC3 *Func3; // TODO
};

struct NAMED_TOKEN {
    char name[10];
    union TOKEN *token;
};
#endif //SCALLION_EXPRS_H
