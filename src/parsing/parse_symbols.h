//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_SYMBOLS_H
#define SCALLION_SYMBOLS_H

struct ADD_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct SUB_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct DECIMAL_E {
    long line_num;
    long char_num;
    double value;
};
struct INTEGER_E {
    long line_num;
    long char_num;
    long value;
};
struct EXP_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct MUL_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct DIV_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct GTE_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct GT_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct LT_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct LTE_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct AND_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct BWAND_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct OR_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct BWOR_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
struct MOD_E {
    long line_num;
    long char_num;
    union EXPR lh;
    union EXPR rh;
};
// vv TODO
struct BOOLEAN { long line_num; long char_num; bool value; };
struct STRING { long line_num; long char_num; char * value; };
struct NAME { long line_num; long char_num; char * value; };
struct EQ { long line_num; long char_num; };
struct NEQ { long line_num; long char_num; };
struct NOT { long line_num; long char_num; };
struct BWNOT { long line_num; long char_num; };
struct IF { long line_num; long char_num; };
struct ELSE { long line_num; long char_num; };

union EXPR {
    struct ADD_E add; // N_EXPR name "ADD"
    struct SUB_E sub;
    struct DECIMAL_E decimal;
    struct INTEGER_E integer;
    struct EXP_E exp;
    struct MUL_E mul;
    struct DIV_E div;
    struct GTE_E gte;
    struct GT_E gt;
    struct LTE_E lte;
    struct LT_E lt;
    struct AND_E _and;
    struct BWAND_E bwand;
    struct OR_E _or;
    struct BWOR_E bwor;
    struct MOD_E mod;
};

struct N_EXPR {
    char name [10];
    union EXPR expr;
};

// helpers
struct N_EXPR * gen_expr() {
    return (struct N_EXPR *)calloc(1, sizeof(struct N_EXPR));
}

#endif //SCALLION_SYMBOLS_H
