//
// Created by Alex Bruns on 3/26/19.
//

#ifndef SCALLION_PARSING_H
#define SCALLION_PARSING_H

// library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

// local includes
#include "exprs.h"
#include "alexBrunsDataStructs.h"
#include "exprUtils.h"
#include "results.h"
#include "debugGlobals.h"
#include "globals.h"

struct NAMED_RESULT * simplify(struct NAMED_TOKEN *);
void const_parse(struct NAMED_TOKEN *, struct NAMED_RESULT *);
int type_2_int(struct NAMED_RESULT *);
void plus_parse(struct NAMED_TOKEN *, struct NAMED_RESULT *);
void sub_parse(struct NAMED_TOKEN *, struct NAMED_RESULT *);
void mod_parse(struct NAMED_TOKEN *, struct NAMED_RESULT *);
void div_parse(struct NAMED_TOKEN *, struct NAMED_RESULT *);
void mul_parse(struct NAMED_TOKEN *, struct NAMED_RESULT *);

/*** PARSING ***/
struct NAMED_RESULT * simplify(struct NAMED_TOKEN * tt) {
    struct NAMED_RESULT * nr = calloc(1, sizeof(struct NAMED_RESULT));
    if (strcmp(tt->name, "CONST") == 0) {
        const_parse(tt, nr);
    } else if (strcmp(tt->name, "PLUS") == 0) {
        plus_parse(tt, nr);
    } else if (strcmp(tt->name, "SUB") == 0) {
        sub_parse(tt, nr);
    } else if (strcmp(tt->name, "MOD") == 0) {
        mod_parse(tt, nr);
    } else if (strcmp(tt->name, "DIV") == 0) {
        div_parse(tt, nr);
    } else if (strcmp(tt->name, "MUL") == 0) {
        mul_parse(tt, nr);
    }
    return nr;
}

void const_parse(struct NAMED_TOKEN * tt, struct NAMED_RESULT * nr) {
    strcpy(nr->name, "R_INT");
    nr->result.Int.v = tt->token->Const->v;
}

int type_2_int(struct NAMED_RESULT * r) {
    if (strcmp(r->name, "R_FLOAT") == 0) {
        return 0;
    } else if (strcmp(r->name, "R_INT") == 0) {
        return 1;
    } else if (strcmp(r->name, "R_CHAR") == 0) {
        return 2;
    } else if (strcmp(r->name, "R_BOOL") == 0) {
        return 3;
    } else if (strcmp(r->name, "R_STRING") == 0) {
        return 4;
    } else if (strcmp(r->name, "R_LIST") == 0) {
        return 5;
    } else {
        return -1;
    }
}

void plus_parse(struct NAMED_TOKEN * tt, struct NAMED_RESULT * nr) {
    struct NAMED_RESULT * lhr = simplify(tt->token->Plus->lh);
    struct NAMED_RESULT * rhr = simplify(tt->token->Plus->rh);
    int lhrt = type_2_int(lhr);
    int rhrt = type_2_int(rhr);
    assert(lhrt < 4); // both args must be arith types (for now, implicit conversion will be implemented later)
    assert(rhrt < 4);
    assert(lhrt >= 0); // check for unknown types
    assert(rhrt >= 0);
    switch (lhrt) {
        case 0:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    nr->result.Float.v = lhr->result.Float.v + rhr->result.Float.v;
                    break;
                case 1:
                    // implicityly converts the rh int arg to a float
                    nr->result.Float.v = lhr->result.Float.v + rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh char arg to a float
                    nr->result.Float.v = lhr->result.Float.v + rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the rh bool arg to either the float 0.0 or 1.0
                    nr->result.Float.v = lhr->result.Float.v + rhr->result.Bool.v;
                    break;
            }
            break;
        case 1:
            strcpy(nr->name, "R_INT");
            switch (rhrt) {
                case 0:
                    // implicitly converts the rh arg from an int to a float
                    nr->result.Float.v = lhr->result.Int.v + rhr->result.Float.v;
                    break;
                case 1:
                    nr->result.Int.v = lhr->result.Int.v + rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh arg from a char to an int
                    nr->result.Int.v = lhr->result.Int.v + rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the lh arg from a bool to an int whose value is either '000...001' or '000...000'
                    nr->result.Int.v = lhr->result.Int.v + rhr->result.Bool.v;
                    break;
            }
            break;
        case 2:
            strcpy(nr->name, "R_CHAR");
            switch (rhrt) {
                case 0:
                    // implicitly converts the lh arg from a char to a float
                    nr->result.Float.v = lhr->result.Char.v + rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly converts the lh arg from a char to an int
                    nr->result.Int.v = lhr->result.Char.v + rhr->result.Int.v;
                    break;
                case 2:
                    nr->result.Char.v = lhr->result.Char.v + rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the rh arg from a bool to a char whose value is either '00000001' or '00000000'
                    nr->result.Char.v = lhr->result.Char.v + rhr->result.Bool.v;
                    break;
            }
            break;
        case 3:
            strcpy(nr->name, "R_BOOL");
            switch (rhrt) {
                case 0:
                    // implicitly convert the lh bool arg to a float whose value is 0.0 or 1.0
                    nr->result.Float.v = lhr->result.Bool.v + rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly convert the lh bool arg to an int whose value is '000...001' or '000...000'
                    nr->result.Int.v = lhr->result.Bool.v + rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly convert the lh bool arg to a char whose value is '00000001' or '00000000'
                    nr->result.Char.v = lhr->result.Bool.v + rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the + operator to an & operator
                    nr->result.Bool.v = lhr->result.Bool.v & rhr->result.Bool.v;
                    break;
            }
            break;
    }
}

void sub_parse(struct NAMED_TOKEN * tt, struct NAMED_RESULT * nr) {
    struct NAMED_RESULT * lhr = simplify(tt->token->Plus->lh);
    struct NAMED_RESULT * rhr = simplify(tt->token->Plus->rh);
    int lhrt = type_2_int(lhr);
    int rhrt = type_2_int(rhr);
    assert(lhrt < 4); // both args must be arith types (for now, implicit conversion will be implemented later)
    assert(rhrt < 4);
    assert(lhrt >= 0); // check for unknown types
    assert(rhrt >= 0);
    switch (lhrt) {
        case 0:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    nr->result.Float.v = lhr->result.Float.v - rhr->result.Float.v;
                    break;
                case 1:
                    // implicityly converts the rh int arg to a float
                    nr->result.Float.v = lhr->result.Float.v - rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh char arg to a float
                    nr->result.Float.v = lhr->result.Float.v - rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the rh bool arg to either the float 0.0 or 1.0
                    nr->result.Float.v = lhr->result.Float.v - rhr->result.Bool.v;
                    break;
            }
            break;
        case 1:
            strcpy(nr->name, "R_INT");
            switch (rhrt) {
                case 0:
                    // implicitly converts the rh arg from an int to a float
                    nr->result.Float.v = lhr->result.Int.v - rhr->result.Float.v;
                    break;
                case 1:
                    nr->result.Int.v = lhr->result.Int.v - rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh arg from a char to an int
                    nr->result.Int.v = lhr->result.Int.v - rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the lh arg from a bool to an int whose value is either '000...001' or '000...000'
                    nr->result.Int.v = lhr->result.Int.v - rhr->result.Bool.v;
                    break;
            }
            break;
        case 2:
            strcpy(nr->name, "R_CHAR");
            switch (rhrt) {
                case 0:
                    // implicitly converts the lh arg from a char to a float
                    nr->result.Float.v = lhr->result.Char.v - rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly converts the lh arg from a char to an int
                    nr->result.Int.v = lhr->result.Char.v - rhr->result.Int.v;
                    break;
                case 2:
                    nr->result.Char.v = lhr->result.Char.v - rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the rh arg from a bool to a char whose value is either '00000001' or '00000000'
                    nr->result.Char.v = lhr->result.Char.v - rhr->result.Bool.v;
                    break;
            }
            break;
        case 3:
            strcpy(nr->name, "R_BOOL");
            switch (rhrt) {
                case 0:
                    // implicitly convert the lh bool arg to a float whose value is 0.0 or 1.0
                    nr->result.Float.v = lhr->result.Bool.v - rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly convert the lh bool arg to an int whose value is '000...001' or '000...000'
                    nr->result.Int.v = lhr->result.Bool.v - rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly convert the lh bool arg to a char whose value is '00000001' or '00000000'
                    nr->result.Char.v = lhr->result.Bool.v - rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the - operator to an ^ operator
                    nr->result.Bool.v = lhr->result.Bool.v ^ rhr->result.Bool.v;
                    break;
            }
            break;
    }
}

void mod_parse(struct NAMED_TOKEN * tt, struct NAMED_RESULT * nr) {
    struct NAMED_RESULT * lhr = simplify(tt->token->Plus->lh);
    struct NAMED_RESULT * rhr = simplify(tt->token->Plus->rh);
    int lhrt = type_2_int(lhr);
    int rhrt = type_2_int(rhr);
    assert(lhrt < 4); // both args must be arith types (for now, implicit conversion will be implemented later)
    assert(rhrt < 4);
    assert(lhrt >= 0); // check for unknown types
    assert(rhrt >= 0);
    switch (lhrt) {
        case 0:
            strcpy(nr->name, "R_FLOAT");
            // calling this undefined for now. will implement floating point mod later.
            assert(lhrt != 0 && rhrt != 0);
            break;
        case 1:
            strcpy(nr->name, "R_INT");
            switch (rhrt) {
                case 0:
                    // calling this undefined for now. will implement floating point mod later.
                    assert(lhrt != 0 && rhrt != 0);
                    break;
                case 1:
                    nr->result.Int.v = lhr->result.Int.v % rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh arg from a char to an int
                    nr->result.Int.v = lhr->result.Int.v % rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the lh arg from a bool to an int whose value is either '000...001' or '000...000'
                    nr->result.Int.v = lhr->result.Int.v % rhr->result.Bool.v;
                    break;
            }
            break;
        case 2:
            switch (rhrt) {
                case 0:
                    // calling this undefined for now. will implement floating point mod later.
                    assert(lhrt != 0 && rhrt != 0);
                    break;
                case 1:
                    // implicitly converts the lh arg from a char to an int
                    strcpy(nr->name, "R_INT");
                    nr->result.Int.v = lhr->result.Char.v % rhr->result.Int.v;
                    break;
                case 2:
                    strcpy(nr->name, "R_CHAR");
                    nr->result.Char.v = lhr->result.Char.v % rhr->result.Char.v;
                    break;
                case 3:
                    // implicitly converts the rh arg from a bool to a char whose value is either '00000001' or '00000000'
                    strcpy(nr->name, "R_CHAR");
                    nr->result.Char.v = lhr->result.Char.v % rhr->result.Bool.v;
                    break;
            }
            break;
        case 3:
            switch (rhrt) {
                case 0:
                    // calling this undefined for now. will implement floating point mod later.
                    assert(lhrt != 0 && rhrt != 0);
                    break;
                case 1:
                    // implicitly convert the lh bool arg to an int whose value is '000...001' or '000...000'
                    strcpy(nr->name, "R_INT");
                    nr->result.Int.v = lhr->result.Bool.v % rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly convert the lh bool arg to a char whose value is '00000001' or '00000000'
                    strcpy(nr->name, "R_Char");
                    nr->result.Char.v = lhr->result.Bool.v % rhr->result.Char.v;
                    break;
                case 3:
                    // undefined behavior for modding a bool with a bool
                    assert(lhrt != 3 || rhrt != 3);
                    break;
            }
            break;
    }
}

void div_parse(struct NAMED_TOKEN * tt, struct NAMED_RESULT * nr) {
    struct NAMED_RESULT * lhr = simplify(tt->token->Plus->lh);
    struct NAMED_RESULT * rhr = simplify(tt->token->Plus->rh);
    int lhrt = type_2_int(lhr);
    int rhrt = type_2_int(rhr);
    assert(lhrt < 4); // both args must be arith types (for now, implicit conversion will be implemented later)
    assert(rhrt < 4);
    assert(lhrt >= 0); // check for unknown types
    assert(rhrt >= 0);
    switch (lhrt) {
        case 0:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    nr->result.Float.v = lhr->result.Float.v / rhr->result.Float.v;
                    break;
                case 1:
                    // implicityly converts the rh int arg to a float
                    nr->result.Float.v = lhr->result.Float.v / rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh char arg to a float
                    nr->result.Float.v = lhr->result.Float.v / rhr->result.Char.v;
                    break;
                case 3:
                    // division by a boolean is undefined
                    assert(rhrt != 3);
                    break;
            }
            break;
        case 1:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    nr->result.Float.v = lhr->result.Int.v / rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Int.v / (double)rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Int.v / (double)rhr->result.Char.v;
                    break;
                case 3:
                    // division by a boolean is undefined
                    assert(rhrt != 3);
                    break;
            }
            break;
        case 2:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Char.v / rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Char.v / (double)rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Char.v / (double)rhr->result.Char.v;
                    break;
                case 3:
                    // division by a boolean is undefined
                    assert(rhrt != 3);
                    break;
            }
            break;
        case 3:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Bool.v / (double)rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Bool.v / (double)rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts both args to floats
                    nr->result.Float.v = (double)lhr->result.Bool.v / (double)rhr->result.Char.v;
                    break;
                case 3:
                    // undefined behavior for modding a bool with a bool
                    assert(lhrt != 3 || rhrt != 3);
                    break;
            }
            break;
    }
}

void mul_parse(struct NAMED_TOKEN * tt, struct NAMED_RESULT * nr) {
    struct NAMED_RESULT * lhr = simplify(tt->token->Plus->lh);
    struct NAMED_RESULT * rhr = simplify(tt->token->Plus->rh);
    int lhrt = type_2_int(lhr);
    int rhrt = type_2_int(rhr);
    assert(lhrt < 4); // both args must be arith types (for now, implicit conversion will be implemented later)
    assert(rhrt < 4);
    assert(lhrt >= 0); // check for unknown types
    assert(rhrt >= 0);
    switch (lhrt) {
        case 0:
            strcpy(nr->name, "R_FLOAT");
            switch (rhrt) {
                case 0:
                    nr->result.Float.v = lhr->result.Float.v * rhr->result.Float.v;
                    break;
                case 1:
                    // implicityly converts the rh int arg to a float
                    nr->result.Float.v = lhr->result.Float.v * rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts the rh char arg to a float
                    nr->result.Float.v = lhr->result.Float.v * rhr->result.Char.v;
                    break;
                case 3:
                    // multiplication by a boolean is undefined
                    assert(rhrt != 3);
                    break;
            }
            break;
        case 1:
            switch (rhrt) {
                case 0:
                    // implicitly converts left hand args to float
                    strcpy(nr->name, "R_FLOAT");
                    nr->result.Float.v = (double)lhr->result.Int.v * rhr->result.Float.v;
                    break;
                case 1:
                    strcpy(nr->name, "R_INT");
                    nr->result.Int.v = lhr->result.Int.v * rhr->result.Int.v;
                    break;
                case 2:
                    // implicitly converts right hand arg to Int
                    strcpy(nr->name, "R_INT");
                    nr->result.Int.v = lhr->result.Int.v * (int)rhr->result.Char.v;
                    break;
                case 3:
                    // multiplication by a boolean is undefined
                    assert(rhrt != 3);
                    break;
            }
            break;
        case 2:
            switch (rhrt) {
                case 0:
                    // implicitly converts first arg to float
                    strcpy(nr->name, "R_FLOAT");
                    nr->result.Float.v = (double)lhr->result.Char.v * rhr->result.Float.v;
                    break;
                case 1:
                    // implicitly converts left hand arg to int
                    strcpy(nr->name, "R_Int");
                    nr->result.Int.v = (int)lhr->result.Char.v * rhr->result.Int.v;
                    break;
                case 2:
                    strcpy(nr->name, "R_CHAR");
                    nr->result.Char.v = lhr->result.Char.v * rhr->result.Char.v;
                    break;
                case 3:
                    // multiplication by a boolean is undefined
                    assert(rhrt != 3);
                    break;
            }
            break;
        case 3:
            // multiplication by a boolean is undefined
            assert(lhrt != 3);
            break;
    }
}
#endif //SCALLION_PARSING_H
