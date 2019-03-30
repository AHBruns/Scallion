//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_PARSE_SYMBOLS_H
#define SCALLION_PARSE_SYMBOLS_H

#include "../utils/utils.h"

int EXPR_PRINT_DEPTH = 0;

struct ADD_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct SUB_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
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
struct BOOL_E {
    long line_num;
    long char_num;
    bool value;
};
struct EXP_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct MUL_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct DIV_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct GTE_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct GT_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct LT_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct LTE_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct AND_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct BWAND_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct OR_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct BWOR_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
struct MOD_E {
    long line_num;
    long char_num;
    void * lh;
    void * rh;
};
// vv TODO
//struct BOOLEAN { long line_num; long char_num; bool value; };
//struct STRING { long line_num; long char_num; char * value; };
//struct NAME { long line_num; long char_num; char * value; };
//struct EQ { long line_num; long char_num; };
//struct NEQ { long line_num; long char_num; };
//struct NOT { long line_num; long char_num; };
//struct BWNOT { long line_num; long char_num; };
//struct IF { long line_num; long char_num; };
//struct ELSE { long line_num; long char_num; };

union EXPR {
    struct ADD_E add;
    struct SUB_E sub;
    struct DECIMAL_E decimal;
    struct INTEGER_E integer;
    struct BOOL_E _bool;
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
    return ((struct N_EXPR *)calloc(1, sizeof(struct N_EXPR)));
}

void helper(struct N_EXPR * root) {
    if (streq(root->name, "INTEGER_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("INTEGER_E (%ld)", root->expr.integer.value);
    } else if (streq(root->name, "DECIMAL_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("DECIMAL_E (%lf)", root->expr.decimal.value);
    } else if (streq(root->name, "BOOL_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        if (root->expr._bool.value) printf("BOOL_E (true)");
        else printf("BOOL_E (false)");
    } else if (streq(root->name, "PLUS_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("PLUS_E (\n");
        EXPR_PRINT_DEPTH++;
        helper(root->expr.add.lh);
        printf(",\n");
        helper(root->expr.add.rh);
        EXPR_PRINT_DEPTH--;
        printf("\n");
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf(")");
    } else if (streq(root->name, "MINUS_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("MINUS_E (\n");
        EXPR_PRINT_DEPTH++;
        helper(root->expr.sub.lh);
        printf(",\n");
        helper(root->expr.sub.rh);
        EXPR_PRINT_DEPTH--;
        printf("\n");
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf(")");
    } else if (streq(root->name, "MUL_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("MUL_E (\n");
        EXPR_PRINT_DEPTH++;
        helper(root->expr.sub.lh);
        printf(",\n");
        helper(root->expr.sub.rh);
        EXPR_PRINT_DEPTH--;
        printf("\n");
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf(")");
    } else if (streq(root->name, "DIV_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("DIV_E (\n");
        EXPR_PRINT_DEPTH++;
        helper(root->expr.sub.lh);
        printf(",\n");
        helper(root->expr.sub.rh);
        EXPR_PRINT_DEPTH--;
        printf("\n");
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf(")");
    } else if (streq(root->name, "MOD_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("MOD_E (\n");
        EXPR_PRINT_DEPTH++;
        helper(root->expr.sub.lh);
        printf(",\n");
        helper(root->expr.sub.rh);
        EXPR_PRINT_DEPTH--;
        printf("\n");
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf(")");
    } else if (streq(root->name, "EXP_E")) {
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf("EXP_E (\n");
        EXPR_PRINT_DEPTH++;
        helper(root->expr.sub.lh);
        printf(",\n");
        helper(root->expr.sub.rh);
        EXPR_PRINT_DEPTH--;
        printf("\n");
        for (int i = 0; i < EXPR_PRINT_DEPTH; i++) printf("   ");
        printf(")");
    }
}

void printf_expr_tree(struct N_EXPR * root) {
    EXPR_PRINT_DEPTH = 0;
    helper(root);
    printf("\n");
}

#endif //SCALLION_SYMBOLS_H
