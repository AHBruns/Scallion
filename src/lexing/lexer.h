//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_LEXER_H
#define SCALLION_LEXER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lex_symbols.h"
#include "../structures/linked_list.h"
#include "../globals/globals.h"

void lex(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_space(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_tab(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_newline(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_op(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_cp(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_ob(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_cb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_ocb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_ccb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_plus(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_minus(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_star(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_slash(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_gt_gte(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_lt_lte(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_amper(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_pipe(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_modulo(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_comma(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_semicolon(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_colon(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_str(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
void lex_number(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);
bool lex_number_helper1(char * program);
int lex_number_helper2(char * program);
void lex_alpha_str(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num);

void lex(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    while (*program != '\0') {
        switch (*program) {
            case ' ':
                return lex_space(program, head, line_num, char_num);
            case '\t':
                return lex_tab(program, head, line_num, char_num);
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
            case '{':
                return lex_ocb(program, head, line_num, char_num);
            case '}':
                return lex_ccb(program, head, line_num, char_num);
            case '+':
                return lex_plus(program, head, line_num, char_num);
            case '-':
                return lex_minus(program, head, line_num, char_num);
            case '*':
                return lex_star(program, head, line_num, char_num);
            case '/':
                return lex_slash(program, head, line_num, char_num);
            case '>':
                return lex_gt_gte(program, head, line_num, char_num);
            case '<':
                return lex_lt_lte(program, head, line_num, char_num);
            case '&':
                return lex_amper(program, head, line_num, char_num);
            case '|':
                return lex_pipe(program, head, line_num, char_num);
            case '%':
                return lex_modulo(program, head, line_num, char_num);
            case ',':
                return lex_comma(program, head, line_num, char_num);
            case ';':
                return lex_semicolon(program, head, line_num, char_num);
            case ':':
                return lex_colon(program, head, line_num, char_num);
            default:
                return lex_str(program, head, line_num, char_num);
        }

    }
}

// handles spaces
void lex_space(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    lex(program+1, head, line_num, char_num+1);
}

// handles tabs
void lex_tab(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    lex(program+1, head, line_num, char_num+1);
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

// handles {
void lex_ocb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "OCB");
    symbol->sym.ocb.line_num = line_num;
    symbol->sym.ocb.char_num = char_num;
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles }
void lex_ccb(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    strcpy(symbol->name, "CCB");
    symbol->sym.ccb.line_num = line_num;
    symbol->sym.ccb.char_num = char_num;
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
    symbol->sym.mul.line_num = line_num; // mul & exp are identical is structure so this works
    symbol->sym.mul.char_num = char_num;
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

// handles /
void lex_slash(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.div.line_num = line_num;
    symbol->sym.div.char_num = char_num;
    strcpy(symbol->name, "DIV");
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles > and >=
void lex_gt_gte(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.gt.line_num = line_num; // gt & gte are identical is structure so this works
    symbol->sym.gt.char_num = char_num;
    int chars_consumed = 0;
    if (*(program+1) == '=') {
        chars_consumed = 2;
        strcpy(symbol->name, "GTE");
    } else {
        chars_consumed = 1;
        strcpy(symbol->name, "GT");
    }
    ll_add(head, symbol);
    lex(program+chars_consumed, head, line_num, char_num+chars_consumed);
}

// handles < and <=
void lex_lt_lte(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.lt.line_num = line_num; // lt & lte are identical is structure so this works
    symbol->sym.lt.char_num = char_num;
    int chars_consumed = 0;
    if (*(program+1) == '=') {
        chars_consumed = 2;
        strcpy(symbol->name, "LTE");
    } else {
        chars_consumed = 1;
        strcpy(symbol->name, "LT");
    }
    ll_add(head, symbol);
    lex(program+chars_consumed, head, line_num, char_num+chars_consumed);
}

// handles & and &&
void lex_amper(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym._and.line_num = line_num; // bwand & _and are identical is structure so this works
    symbol->sym._and.char_num = char_num;
    int chars_consumed = 0;
    if (*(program+1) == '&') {
        chars_consumed = 2;
        strcpy(symbol->name, "AND");
    } else {
        chars_consumed = 1;
        strcpy(symbol->name, "BWAND");
    }
    ll_add(head, symbol);
    lex(program+chars_consumed, head, line_num, char_num+chars_consumed);
}

// handles | and ||
void lex_pipe(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym._or.line_num = line_num; // bwor & _or are identical is structure so this works
    symbol->sym._or.char_num = char_num;
    int chars_consumed = 0;
    if (*(program+1) == '|') {
        chars_consumed = 2;
        strcpy(symbol->name, "OR");
    } else {
        chars_consumed = 1;
        strcpy(symbol->name, "BWOR");
    }
    ll_add(head, symbol);
    lex(program+chars_consumed, head, line_num, char_num+chars_consumed);
}

// handles %
void lex_modulo(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.mod.line_num = line_num;
    symbol->sym.mod.char_num = char_num;
    strcpy(symbol->name, "MOD");
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles ,
void lex_comma(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.comma.line_num = line_num;
    symbol->sym.comma.char_num = char_num;
    strcpy(symbol->name, "COMMA");
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles ;
void lex_semicolon(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.semicolon.line_num = line_num;
    symbol->sym.semicolon.char_num = char_num;
    strcpy(symbol->name, "SEMICOLON");
    ll_add(head, symbol);
    lex(program+1, head, line_num, char_num+1);
}

// handles :
void lex_colon(char * program, struct NODE * head, unsigned long line_num, unsigned long char_num) {
    struct N_SYM * symbol = gen_symbol();
    symbol->sym.colon.line_num = line_num;
    symbol->sym.colon.char_num = char_num;
    strcpy(symbol->name, "COLON");
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
    symbol->sym.integer.line_num = line_num; // integer & decimal are identical is structure so this works
    symbol->sym.integer.char_num = char_num;
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
    // TODO : this is going to suck so I'm putting it off
}




#endif //SCALLION_LEXER_H
