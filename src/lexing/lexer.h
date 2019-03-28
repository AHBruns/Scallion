//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_LEXER_H
#define SCALLION_LEXER_H

#include <stdlib.h>
#include <string.h>

#include "symbols.h"
#include "../structures/linked_list.h"
#include "../utils/utils.h"
#include "../globals/globals.h"

void lex(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_newline(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_op(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_cp(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_ob(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_cb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_plus(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_minus(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_star(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_slash(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num)
void lex_str(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_number(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
bool lex_number_helper1(char * program);
int lex_number_helper2(char * program);
void lex_alpha_str(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);

void lex(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    while (*program != '\0') {
        switch (*program) {
            case '\n':
                return lex_newline(program, head, line_num, char_num);
            case '(':
                return lex_op(program, head, line_num, char_num);
            case ')':
                return lex_cp(program, head, line_num, char_num);
            case '[':
                return lex_ob(program, head, line_num, char_num);
            case ']':
                return lex_cb(program, head, line_num, char_num);
            case '+':
                return lex_plus(program, head, line_num, char_num);
            case '-':
                return lex_minus(program, head, line_num, char_num);
            case '*':
                return lex_star(program, head, line_num, char_num);
            case '/':
                return lex_slash(program, head, line_num, char_num);
            default:
                return lex_str(program, head, line_num, char_num);
        }

    }
}

// handles \n
void lex_newline(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    lex(program+1, head, line_num+1, 0);
}

// handles (
void lex_op(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "OP");
    symbol->sym.op.line_num = line_num;
    symbol->sym.op.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles )
void lex_cp(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "CP");
    symbol->sym.cp.line_num = line_num;
    symbol->sym.cp.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles [
void lex_ob(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "OB");
    symbol->sym.ob.line_num = line_num;
    symbol->sym.ob.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles ]
void lex_cb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "CB");
    symbol->sym.cb.line_num = line_num;
    symbol->sym.cb.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles +
void lex_plus(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "PLUS");
    symbol->sym.plus.line_num = line_num;
    symbol->sym.plus.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles -
void lex_minus(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "MINUS");
    symbol->sym.minus.line_num = line_num;
    symbol->sym.minus.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles * and **
void lex_star(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.minus.line_num = line_num;
    symbol->sym.minus.char_num = char_num;
    int chars_consumed = 0;
    if (*(program+1) == '*') {
        strcpy(symbol->name, "EXP");
        chars_consumed = 2;
    } else {
        strcpy(symbol->name, "MUL");
        chars_consumed = 1;
    }
    ll_add(head, symbol);
    lex(program+chars_consumed, head, line_num, char_num+chars_consumed);
}

void lex_slash(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.minus.line_num = line_num;
    symbol->sym.minus.char_num = char_num;
    strcpy(symbol->name, "DIV");
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles all alphanumeric symbols
void lex_str(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    if (strchr(VALID_DECIMAL_CHARS, *program)) lex_number(program, head, line_num, char_num);
    else lex_alpha_str(program, head, line_num, char_num);
}

// handles integer and decimal numbers
void lex_number(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.minus.line_num = line_num;
    symbol->sym.minus.char_num = char_num;
    int chars_consumed = lex_number_helper2(program);
    if (lex_number_helper1(program)) { // decimal branch
        strcpy(symbol->name, "DECIMAL");
        symbol->sym.decimal.value = atof(program);;

    } else { // integer branch
        strcpy(symbol->name, "INTEGER");
        symbol->sym.integer.value = atoi(program);
    }
    ll_add(head, symbol);
    lex(program+chars_consumed, head, line_num, char_num+chars_consumed);
}

bool lex_number_helper1(char * program) {
    char * ptr = program;
    while (strchr(VALID_DECIMAL_CHARS, *ptr)) {
        if (!strchr(VALID_INTEGER_CHARS, *ptr)) return true;
        ptr++;
    }
    return false;
}

int lex_number_helper2(char * program) {
    int count = 0;
    char * ptr = program;
    while (strchr(VALID_DECIMAL_CHARS, *ptr)) {
        count++;
        ptr++;
    }
    return count;
}

// handles all alphabetical symbols
void lex_alpha_str(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    // TODO
}




#endif //SCALLION_LEXER_H
