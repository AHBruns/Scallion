//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_PARSER_H
#define SCALLION_PARSER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//#include "../lexing/symbols.h"
#include "../structures/linked_list.h"
#include "../globals/globals.h"
#include "../utils/utils.h"
#include "../lexing/lex_symbols.h"
#include "parse_symbols.h"

void parse(struct NODE * program_head);
void parse_plus(struct NODE * program_head);

void parse(struct NODE * program_head) {
    // pass for control statements
    // pass for parenthesis
    program_head = parse_paren_pass(program_head);
    // pass for exponentiation
    // pass for multiplication & division (equal precedence)
    // pass for addition & subtraction (equal precedence)
}

// ERROR CODES
// -1 : unmatched parenthesis
// -2 : um, wut?
void parse_paren_pass(struct NODE * program_head) {
    struct NODE * paren_head = NULL;
    depth = 0; // 0 is base, negatives are inside one or more parenthesis
    while (program_head->contents) {
        struct N_SYM * symbol = (struct N_SYM *)program_head->contents;
        if (depth == 0) {
            if (streq(symbol->name, "OP")) {
                depth--;
                paren_head = ll_gen_head();
                ll_remove(program_head, symbol);
                free(symbol);
            } else if (streq(symbol->name, "CP")) {
                abort(-1);
            }
        } else if (depth < 0) {
            if (streq(symbol->name, "OP")) {
                depth--;
                ll_add(paren_head, symbol);
            } else if (streq(symbol->name, "CP")) {
                depth++;
                if (depth == 0) {
                    free(symbol);
                    parse(paren_head->next);
                    ll_add_at(program_head, paren_head->next);
                    ll_remove(paren_head, paren_head->next);
                    ll_remove(paren_head);
                    paren_head = NULL;
                } else ll_add(paren_head, symbol);
            } else ll_add(paren_head, symbol);
            ll_remove(program_head, symbol);
        } else {
            abort(-2);
        }
        program_head = program_head->next;
    }
}

void parse_plus(struct NODE * program_head) {
    struct N_EXPR * expr = gen_expr();
    strcpy(expr->name, "ADD");
    expr->expr.add.char_num = program_head->prev
}

#endif //SCALLION_PARSER_H
