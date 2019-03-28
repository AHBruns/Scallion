//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_SYMBOLS_H
#define SCALLION_SYMBOLS_H

#include <stdbool.h>

struct OP { long line_num; long char_num; };
struct CP { long line_num; long char_num; };
struct OB { long line_num; long char_num; };
struct CB { long line_num; long char_num; };
struct PLUS { long line_num; long char_num; };
struct MINUS { long line_num; long char_num; };
struct DECIMAL { long line_num; long char_num; double value; };
struct INTEGER { long line_num; long char_num; long value; };
struct EXP { long line_num; long char_num; };
struct MUL { long line_num; long char_num; };

struct BOOLEAN { long line_num; long char_num; bool value; };
struct STRING { long line_num; long char_num; char * value; };
struct NAME { long line_num; long char_num; char * value; };
struct DIV { long line_num; long char_num; };
struct EQ { long line_num; long char_num; };
struct NEQ { long line_num; long char_num; };
struct GT { long line_num; long char_num; };
struct LT { long line_num; long char_num; };
struct GTE { long line_num; long char_num; };
struct LTE { long line_num; long char_num; };
struct AND { long line_num; long char_num; };
struct OR { long line_num; long char_num; };
struct NOT { long line_num; long char_num; };
struct BWAND { long line_num; long char_num; };
struct BWOR { long line_num; long char_num; };
struct BWNOT { long line_num; long char_num; };
struct IF { long line_num; long char_num; };
struct ELSE { long line_num; long char_num; };

union SYM {
    struct OP op; // N_SYM name "OP"
    struct CP cp; // N_SYM name "CP"
    struct OB ob; // N_SYM name "OB"
    struct CB cb; // N_SYM name "CB"
    struct PLUS plus; // N_SYM name "PLUS"
    struct MINUS minus; // N_SYM name "MINUS"
    struct DECIMAL decimal; // N_SYM name "DECIMAL"
    struct INTEGER integer; // N_SYM name "INTEGER"
    struct EXP exp; // N_SYM name "EXP"
    struct MUL mul; // N_SYM name "MUL"

    struct BOOLEAN boolean;
    struct STRING string;
    struct NAME name;
    struct DIV div;
    struct EQ eq;
    struct NEQ neq;
    struct GT gt;
    struct LT lt;
    struct GTE gte;
    struct LTE lte;
    struct AND _and;
    struct OR _or;
    struct BWAND bwand;
    struct BWOR bwor;
    struct BWNOT bwnot;
    struct NOT _not;
    struct IF _if;
    struct ELSE _else;
};

struct N_SYM {
    char name [10];
    union SYM sym;
};

// helpers
struct N_SYM * gen_symbol() {
    return (struct N_SYM *)calloc(1, sizeof(struct N_SYM));
}

#endif //SCALLION_SYMBOLS_H
