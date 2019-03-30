//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_PARSER_H
#define SCALLION_PARSER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "parse_symbols.h"
#include "../structures/linked_list.h"
#include "../globals/globals.h"
#include "../utils/utils.h"
#include "../lexing/lex_symbols.h"

struct NODE * parse(struct NODE * program_head);
struct NODE * parse_paren_pass(struct NODE * program_head);
struct NODE * parse_constants_pass(struct NODE * program_head);
struct NODE * parse_plus_minus_pass(struct NODE * program_head);
struct NODE * parse_mul_div_mod_pass(struct NODE * program_head);
struct NODE * parse_exp_pass(struct NODE * program_head);

struct NODE * parse(struct NODE * program_head) {
    program_head = parse_paren_pass(program_head);
    program_head = parse_constants_pass(program_head);
    // pass for variables
    program_head = parse_exp_pass(program_head);
    program_head = parse_mul_div_mod_pass(program_head);
    program_head = parse_plus_minus_pass(program_head);
    return program_head;
}

// ERROR CODES
// -1 : unmatched open parenthesis
// -2 : unmatched close parenthesis
struct NODE * parse_paren_pass(struct NODE * program_head) {
    struct NODE * ptr = program_head;
    struct NODE * paren_head = NULL;
    int depth = 0;
    bool remove_node_flag = false;
    bool remove_sym_flag = false;
    while (ptr->contents) {
        struct N_SYM * symbol = ptr->contents;
        remove_node_flag = false;
        remove_sym_flag = false;
        if (depth == 0) {
            if (streq(symbol->name, "OP")) {
                depth--;
                paren_head = ll_gen_head();
                remove_node_flag = true;
                remove_sym_flag = true;
            } else if (streq(symbol->name, "CP")) {
                printf("[ERROR] => unmatched parenthesis on line %ld at character %ld\n",
                        symbol->sym.integer.line_num,
                        symbol->sym.integer.char_num+1);
                exit(-2);
            }
        } else  {
            if (streq(symbol->name, "CP")) {
                depth++;
                if (depth == 0) {
                    remove_node_flag = true;
                    paren_head = parse(paren_head->next);
                    ll_add_at(ptr->prev, paren_head->contents, "N_EXPR");
                } else {
                    remove_node_flag = true;
                    ll_add(paren_head, symbol, "N_SYM");
                }
            } else if (streq(symbol->name, "OP")) {
                depth--;
                remove_node_flag = true;
                ll_add(paren_head, symbol, "N_SYM");
            } else {
                remove_node_flag = true;
                ll_add(paren_head, symbol, "N_SYM");
            }
        }
        ptr = ptr->next;
        if (remove_node_flag) ll_remove(ptr, symbol);
        if (remove_sym_flag) free(symbol);
    }
    if (depth < 0) {
        printf("[ERROR] => unmatched parenthesis on line %ld at character %ld\n",
                ((struct N_SYM *)paren_head->next->contents)->sym.integer.line_num,
                ((struct N_SYM *)paren_head->next->contents)->sym.integer.char_num);
        exit(-1);
    }
    return ptr->next;
}

struct NODE * parse_constants_pass(struct NODE * program_head) {
    struct NODE * ptr = program_head;
    while (ptr->contents) {
        if (streq(ptr->contents_type, "N_SYM")) {
            struct N_SYM * symbol = ptr->contents;
            if (streq(symbol->name, "INTEGER")) {
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "INTEGER_E");
                expr->expr.integer.line_num = symbol->sym.integer.line_num;
                expr->expr.integer.char_num = symbol->sym.integer.char_num;
                expr->expr.integer.value = symbol->sym.integer.value;
                ptr->contents = expr;
                strcpy(ptr->contents_type, "N_EXPR");
                free(symbol);
            } else if (streq(symbol->name, "DECIMAL")) {
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "DECIMAL_E");
                expr->expr.decimal.line_num = symbol->sym.decimal.line_num;
                expr->expr.decimal.char_num = symbol->sym.decimal.char_num;
                expr->expr.decimal.value = symbol->sym.decimal.value;
                ptr->contents = expr;
                strcpy(ptr->contents_type, "N_EXPR");
                free(symbol);
            } else if (streq(symbol->name, "BOOL")) {
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "BOOL_E");
                expr->expr._bool.line_num = symbol->sym.boolean.line_num;
                expr->expr._bool.char_num = symbol->sym.boolean.char_num;
                expr->expr._bool.value = symbol->sym.boolean.value;
                ptr->contents = expr;
                strcpy(ptr->contents_type, "N_EXPR");
                free(symbol);
            }
        }
        ptr = ptr->next;
    }
    return program_head;
}

struct NODE * parse_exp_pass(struct NODE * program_head) {
    struct NODE * ptr = program_head;
    while (ptr->contents) {
        if (streq(ptr->contents_type, "N_SYM")) {
            struct N_SYM * symbol = ptr->contents;
            if (streq(symbol->name, "EXP")) {
                if (ptr->prev->contents == NULL || streq(ptr->prev->contents_type, "N_SYM")) {
                    printf("[ERROR] => exponentiation operation missing first argument on line %ld at character %ld\n",
                           symbol->sym.exp.line_num,
                           symbol->sym.exp.char_num);
                    exit(-1);
                }
                if (ptr->next->contents == NULL || streq(ptr->next->contents_type, "N_SYM")) {
                    printf("[ERROR] => exponentiation operation missing second argument on line %ld at character %ld\n",
                           symbol->sym.exp.line_num,
                           symbol->sym.exp.char_num);
                    exit(-2);
                }
                struct N_EXPR * lh = ((struct N_EXPR *)ptr->prev->contents);
                struct N_EXPR * rh = ((struct N_EXPR *)ptr->next->contents);
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "EXP_E");
                expr->expr.exp.lh = lh;
                expr->expr.exp.rh = rh;
                expr->expr.exp.line_num = symbol->sym.exp.line_num;
                expr->expr.exp.char_num = symbol->sym.exp.char_num;
                ll_remove(program_head, ptr->next->contents);
                ptr = ptr->next;
                ll_remove(program_head, ptr->prev->contents);
                ptr->prev->contents = expr;
                ptr = ptr->prev;
            }
        }
        ptr = ptr->next;
    }
    return program_head;
}

// ERROR CODES
// -1 : plus operation missing first argument
// -2 : plus operation missing second argument
// -3 : minus operation missing first argument
// -4 : minus operation missing second argument
struct NODE * parse_plus_minus_pass(struct NODE * program_head) {
    struct NODE * ptr = program_head;
    while (ptr->contents) {
        if (streq(ptr->contents_type, "N_SYM")) {
            struct N_SYM * symbol = ptr->contents;
            if (streq(symbol->name, "PLUS")) {
                if (ptr->prev->contents == NULL || streq(ptr->prev->contents_type, "N_SYM")) {
                    printf("[ERROR] => plus operation missing first argument on line %ld at character %ld\n",
                           symbol->sym.plus.line_num,
                           symbol->sym.plus.char_num);
                    exit(-1);
                }
                if (ptr->next->contents == NULL || streq(ptr->next->contents_type, "N_SYM")) {
                    printf("[ERROR] => plus operation missing second argument on line %ld at character %ld\n",
                           symbol->sym.plus.line_num,
                           symbol->sym.plus.char_num);
                    exit(-2);
                }
                struct N_EXPR * lh = ((struct N_EXPR *)ptr->prev->contents);
                struct N_EXPR * rh = ((struct N_EXPR *)ptr->next->contents);
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "PLUS_E");
                expr->expr.add.lh = lh;
                expr->expr.add.rh = rh;
                expr->expr.add.line_num = symbol->sym.integer.line_num;
                expr->expr.add.char_num = symbol->sym.integer.char_num;
                ll_remove(program_head, ptr->next->contents);
                ptr = ptr->next;
                ll_remove(program_head, ptr->prev->contents);
                ptr->prev->contents = expr;
                ptr = ptr->prev;
            } else if (streq(symbol->name, "MINUS")) {
                if ( ptr->next->contents == NULL || streq(ptr->next->contents_type, "N_SYM")) {
                    printf("[ERROR] => minus operation missing second argument on line %ld at character %ld\n",
                           symbol->sym.minus.line_num,
                           symbol->sym.minus.char_num);
                    exit(-4);
                }
                if (ptr->prev->contents == NULL || streq(ptr->prev->contents_type, "N_SYM")) { // handles negative numbers
                    struct N_EXPR *lh = gen_expr();
                    strcpy(lh->name, "INTEGER_E");
                    lh->expr.integer.value = 0L;
                    struct N_EXPR *rh = ((struct N_EXPR *) ptr->next->contents);
                    struct N_EXPR *expr = gen_expr();
                    strcpy(expr->name, "MINUS_E");
                    expr->expr.sub.lh = lh;
                    expr->expr.sub.rh = rh;
                    expr->expr.sub.line_num = symbol->sym.integer.line_num;
                    expr->expr.sub.char_num = symbol->sym.integer.char_num;
                    ll_remove(program_head, ptr->next->contents);
                    ptr->contents = expr;
                } else { // handle
                    struct N_EXPR *lh = ((struct N_EXPR *) ptr->prev->contents);
                    struct N_EXPR *rh = ((struct N_EXPR *) ptr->next->contents);
                    struct N_EXPR *expr = gen_expr();
                    strcpy(expr->name, "MINUS_E");
                    expr->expr.sub.lh = lh;
                    expr->expr.sub.rh = rh;
                    expr->expr.sub.line_num = lh->expr.integer.line_num;
                    expr->expr.sub.char_num = lh->expr.integer.char_num;
                    ll_remove(program_head, ptr->next->contents);
                    ptr = ptr->next;
                    ll_remove(program_head, ptr->prev->contents);
                    ptr->prev->contents = expr;
                    ptr = ptr->prev;
                }
            }
        }
        ptr = ptr->next;
    }
    return program_head;
}

struct NODE * parse_mul_div_mod_pass(struct NODE * program_head) {
    struct NODE * ptr = program_head;
    while (ptr->contents) {
        if (streq(ptr->contents_type, "N_SYM")) {
            struct N_SYM * symbol = ptr->contents;
            if (streq(symbol->name, "MUL")) {
                if (ptr->prev->contents == NULL || streq(ptr->prev->contents_type, "N_SYM")) {
                    printf("[ERROR] => multiplication operation missing first argument on line %ld at character %ld\n",
                           symbol->sym.mul.line_num,
                           symbol->sym.mul.char_num);
                    exit(-1);
                }
                if (ptr->next->contents == NULL || streq(ptr->next->contents_type, "N_SYM")) {
                    printf("[ERROR] => multiplication operation missing second argument on line %ld at character %ld\n",
                           symbol->sym.mul.line_num,
                           symbol->sym.mul.char_num);
                    exit(-2);
                }
                struct N_EXPR * lh = ((struct N_EXPR *)ptr->prev->contents);
                struct N_EXPR * rh = ((struct N_EXPR *)ptr->next->contents);
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "MUL_E");
                expr->expr.mul.lh = lh;
                expr->expr.mul.rh = rh;
                expr->expr.mul.line_num = symbol->sym.mul.line_num;
                expr->expr.mul.char_num = symbol->sym.mul.char_num;
                ll_remove(program_head, ptr->next->contents);
                ptr = ptr->next;
                ll_remove(program_head, ptr->prev->contents);
                ptr->prev->contents = expr;
                ptr = ptr->prev;
            } else if (streq(symbol->name, "DIV")) {
                if (ptr->prev->contents == NULL || streq(ptr->prev->contents_type, "N_SYM")) {
                    printf("[ERROR] => division operation missing first argument on line %ld at character %ld\n",
                           symbol->sym.div.line_num,
                           symbol->sym.div.char_num);
                    exit(-1);
                }
                if (ptr->next->contents == NULL || streq(ptr->next->contents_type, "N_SYM")) {
                    printf("[ERROR] => division operation missing second argument on line %ld at character %ld\n",
                           symbol->sym.div.line_num,
                           symbol->sym.div.char_num);
                    exit(-2);
                }
                struct N_EXPR * lh = ((struct N_EXPR *)ptr->prev->contents);
                struct N_EXPR * rh = ((struct N_EXPR *)ptr->next->contents);
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "DIV_E");
                expr->expr.div.lh = lh;
                expr->expr.div.rh = rh;
                expr->expr.div.line_num = symbol->sym.div.line_num;
                expr->expr.div.char_num = symbol->sym.div.char_num;
                ll_remove(program_head, ptr->next->contents);
                ptr = ptr->next;
                ll_remove(program_head, ptr->prev->contents);
                ptr->prev->contents = expr;
                ptr = ptr->prev;
            } else if (streq(symbol->name, "MOD")) {
                if (ptr->prev->contents == NULL || streq(ptr->prev->contents_type, "N_SYM")) {
                    printf("[ERROR] => modulo operation missing first argument on line %ld at character %ld\n",
                           symbol->sym.mod.line_num,
                           symbol->sym.mod.char_num);
                    exit(-1);
                }
                if (ptr->next->contents == NULL || streq(ptr->next->contents_type, "N_SYM")) {
                    printf("[ERROR] => modulo operation missing second argument on line %ld at character %ld\n",
                           symbol->sym.mod.line_num,
                           symbol->sym.mod.char_num);
                    exit(-2);
                }
                struct N_EXPR * lh = ((struct N_EXPR *)ptr->prev->contents);
                struct N_EXPR * rh = ((struct N_EXPR *)ptr->next->contents);
                struct N_EXPR * expr = gen_expr();
                strcpy(expr->name, "MOD_E");
                expr->expr.mod.lh = lh;
                expr->expr.mod.rh = rh;
                expr->expr.mod.line_num = symbol->sym.mod.line_num;
                expr->expr.mod.char_num = symbol->sym.mod.char_num;
                ll_remove(program_head, ptr->next->contents);
                ptr = ptr->next;
                ll_remove(program_head, ptr->prev->contents);
                ptr->prev->contents = expr;
                ptr = ptr->prev;
            }
        }
        ptr = ptr->next;
    }
    return program_head;
}

#endif //SCALLION_PARSER_H
