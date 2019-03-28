//
// Created by Alex Bruns on 3/26/19.
//

#ifndef SCALLION_TOKENIZATION_H
#define SCALLION_TOKENIZATION_H

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
#include "globalsDebug.h"
#include "globals.h"

char ** sequencize(char *);
struct NAMED_TOKEN * tokenize(char *, struct NODE *, struct BINDING * env[MAX_CONCURRENT_BINDINGS]);
void const_tokenize_pass(char *, struct NODE *);
struct NAMED_TOKEN * const_tokenize(char *);
void inject_env(char *, struct NODE *, struct BINDING * env[MAX_CONCURRENT_BINDINGS]);
void new_binding_tokenize(char *, struct NODE *, struct BINDING * env[MAX_CONCURRENT_BINDINGS]);
void paren_tokenize(char *, struct NODE *, struct BINDING * env[MAX_CONCURRENT_BINDINGS]);
void pow_tokenize(char *, struct NODE *);
void mul_tokenize(char *, struct NODE *);
void fdiv_tokenize(char *, struct NODE *);
void div_tokenize(char *, struct NODE *);
void mod_tokenize(char *, struct NODE *);
void plus_tokenize(char *, struct NODE *);
void sub_tokenize(char *, struct NODE *);

/*** TOKENIZATION ***/
char ** sequencize(char * raw_input_str) {
    assert(raw_input_str != NULL);
    char ** lines = (char **)calloc(MAX_LINES, sizeof(char *)); // max lines is 100 for now
    assert(lines != NULL);
    int lines_idx = 0;
    char * ptr2 = raw_input_str;
    char * ptr1 = raw_input_str;
    while (ptr1 != NULL) {
        ptr1 = strchr(ptr1, '\n');
        if (ptr1 != NULL) {
            int line_length = ptr1 - ptr2;
            if (line_length > 0) {
                char * line = (char*)calloc(line_length+1, sizeof(char));
                assert(line != NULL);
                memcpy(line, ptr2, line_length);
                line[line_length] = '\0';
                assert(lines_idx < MAX_LINES);
                lines[lines_idx] = line;
                lines_idx++;
            }
            ptr1++;
        } else {
            int line_length = strlen(ptr2);
            if (line_length > 0) {
                char *line = (char *) calloc(line_length + 1, sizeof(char));
                strcpy(line, ptr2);
                assert(lines_idx < MAX_LINES);
                lines[lines_idx] = line;
                lines_idx++;
            }
        }
        ptr2 = ptr1;
    }
    return lines;
}

struct NAMED_TOKEN * tokenize(char * raw_input_expr, struct NODE * node, struct BINDING * env[MAX_CONCURRENT_BINDINGS]) {
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr initially: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** inject bindings ***/
    inject_env(raw_input_expr, node, env);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after inject_env: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** constants pass ***/
    const_tokenize_pass(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after const_tokenize_pass: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** parenthesis pass ***/
    paren_tokenize(raw_input_expr, node, env);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after paren_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** exponentiation pass ***/
    pow_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after pow_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** multiplication pass ***/
    mul_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after mul_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** floor division pass ***/
    fdiv_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after fdiv_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** division pass ***/
    div_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after div_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** modulo pass ***/
    mod_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after mod_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** plus pass ***/
    plus_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after plus_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** sub pass ***/
    sub_tokenize(raw_input_expr, node);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after sub_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    /*** binding pass ***/
    new_binding_tokenize(raw_input_expr, node, env);
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr after new_binding_tokenize: ");
    DBPRINTESCSTR(raw_input_expr);
    DBPRINTLIST(node);
    // END DB
    struct NAMED_TOKEN * ret = nodeFromIndex(raw_input_expr, node)->token;
    return ret;
}

void const_tokenize_pass(char * raw_input_expr, struct NODE * head) {
    // START DB
    DBPRINTF("%s\n", raw_input_expr);
    // END DB
    for (int i = 0; *(raw_input_expr+i) != '\0'; i++) {
        // START DB
        DBPRINTF("i: %d\n", i);
        // END DB
        if (*(raw_input_expr + i) == '0' ||
            *(raw_input_expr + i) == '1' ||
            *(raw_input_expr + i) == '2' ||
            *(raw_input_expr + i) == '3' ||
            *(raw_input_expr + i) == '4' ||
            *(raw_input_expr + i) == '5' ||
            *(raw_input_expr + i) == '6' ||
            *(raw_input_expr + i) == '7' ||
            *(raw_input_expr + i) == '8' ||
            *(raw_input_expr + i) == '9') {
            struct NAMED_TOKEN * nt = const_tokenize(raw_input_expr+i);
            // START DB
            DBPRINTF("found const: %d\n", nt->token->Const->v);
            // END DB
            struct NODE * n = malloc(sizeof(struct NODE));
            n->index = raw_input_expr+i;
            n->token = nt;
            int const_size = strcspn(raw_input_expr+i, NON_INT_CHARS);
            // START DB
            DBPRINTF("const_size: %d\n", const_size);
            DBPRINTF("%s\n", raw_input_expr);
            // END DB
            shiftNDownInRange(raw_input_expr+i, raw_input_expr+i+const_size, const_size-1, head);
            insertNode(n, head);
            int j = 1;
            DBPRINTF("%s\n", raw_input_expr);
            raw_input_expr[i] = '\a';
            DBPRINTF("%s\n", raw_input_expr);
            for (; *(raw_input_expr+i+j+const_size-1) != '\0'; j++) {
                raw_input_expr[i+j] = raw_input_expr[i+j+const_size-1];
            }
            raw_input_expr[i+j] = '\0';
            // START DB
            DBPRINTF("raw_input_expr: %s\n", raw_input_expr);
            // END DB
        }
    }
}

struct NAMED_TOKEN * const_tokenize(char * raw_input_expr) {
    int const_length = strcspn(raw_input_expr, NON_INT_CHARS); // length till non-int character
    long str_length = strlen(raw_input_expr);
    long int value = 0;
    for (int i = 0; i < const_length; i++) {
        switch (raw_input_expr[i]) {
            case '0':
                break;
            case '1':
                value = value + powl(10L, (const_length - i - 1));
                break;
            case '2':
                value = value + (2 * powl(10L, (const_length - i - 1)));
                break;
            case '3':
                value = value + (3 * powl(10L, (const_length - i - 1)));
                break;
            case '4':
                value = value + (4 * powl(10L, (const_length - i - 1)));
                break;
            case '5':
                value = value + (5 * powl(10L, (const_length - i - 1)));
                break;
            case '6':
                value = value + (6 * powl(10L, (const_length - i - 1)));
                break;
            case '7':
                value = value + (7 * powl(10L, (const_length - i - 1)));
                break;
            case '8':
                value = value + (8 * powl(10L, (const_length - i - 1)));
                break;
            case '9':
                value = value + (9 * powl(10L, (const_length - i - 1)));
                break;
        }
    }
    struct NAMED_TOKEN * nt = malloc(sizeof(struct NAMED_TOKEN));
    union TOKEN * t = malloc(sizeof(union TOKEN));
    struct CONST * conststruct = malloc(sizeof(struct CONST));
    strcpy((nt->name), "CONST");
    nt->token = t;
    t->Const = conststruct;
    conststruct->v = value;
    return nt;
}

void inject_env(char * raw_input_expr, struct NODE * head, struct BINDING * env[MAX_CONCURRENT_BINDINGS]) {
    char * binding_loc = strchr(raw_input_expr, '=');
    int binding_offset = 0;
    if (!binding_loc) {
        return;
    } else {
        if (strchr(OP_CHARS, binding_loc[1]) || strchr(OP_CHARS, *(binding_loc-1)))
            binding_offset = binding_loc - raw_input_expr;
    }

    int i = 0;
    while (i < MAX_CONCURRENT_BINDINGS && env[i] != NULL) {
        // START DB
        DBPRINTF("inject_env:\n");
        DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
        DBPRINTF("raw_input_expr: ");
        DBPRINTESCSTR(raw_input_expr);
        // END DB
        char * name = env[i]->name;
        int name_length = strlen(name);
        char * loc = strstr(raw_input_expr+binding_offset, name);
        while (loc != NULL) {
            if (strchr(OP_CHARS, *(loc-1)) != NULL && (strchr(OP_CHARS, *(loc+name_length)) != NULL || *(loc+name_length) == '\0')) {
                int offset = loc - raw_input_expr;
                struct NODE * n = malloc(sizeof(struct NODE));
                n->index = loc;
                n->token = env[i]->expr;
                insertNode(n, head);
                raw_input_expr[offset] = '\a';
                int length = strlen(raw_input_expr);
                for (int j = offset+1; j < (length - name_length + 1); j++) {
                    raw_input_expr[j] = raw_input_expr[j+name_length-1];
                }
                raw_input_expr[1 + length - name_length] = '\0';
                shiftNDownInRange(loc, loc+strlen(loc), name_length-1, head);
            }
            loc = strstr(raw_input_expr+binding_offset, name);
        }
        i++;
    }
}

void new_binding_tokenize(char * raw_input_expr, struct NODE * head, struct BINDING * env[MAX_CONCURRENT_BINDINGS]) {
    // START DB
    DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
    DBPRINTF("raw_input_expr: ");
    DBPRINTESCSTR(raw_input_expr);
    // END DB
    char * binding_loc = strchr(raw_input_expr, '=');
    int statement_length = 0;
    if (!binding_loc) {
        return;
    } else {
        if (strchr(OP_CHARS, binding_loc[1]) || strchr(OP_CHARS, *(binding_loc-1)))
            return;
        statement_length = (binding_loc - raw_input_expr);
        char * name = (char *)calloc(statement_length+1, sizeof(char));
        assert(name!=NULL);
        memmove(name, raw_input_expr, statement_length);
        name[statement_length] = '\0';
        memmove(raw_input_expr, binding_loc+1, strlen(binding_loc));
        // START DB
        DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
        DBPRINTF("raw_input_expr: ");
        DBPRINTESCSTR(raw_input_expr);
        DBPRINTF("offset: %d\n", statement_length+1);
        DBPRINTLIST(head);
        // END DB
        shiftNDownInRange(raw_input_expr, raw_input_expr+strlen(raw_input_expr)+statement_length, statement_length+1, head);
        int i = 0;
        while (i < MAX_CONCURRENT_BINDINGS && env[i] != NULL) i++;
        struct BINDING * bd = malloc(sizeof(struct BINDING));
        assert(bd != NULL);
        bd->expr = nodeFromIndex(raw_input_expr, head)->token;
        strcpy(bd->name, name);
        env[i] = bd;
    }
    return;
}

void paren_tokenize(char * raw_input_expr, struct NODE * head, struct BINDING * env[MAX_CONCURRENT_BINDINGS]) {
    int length = strlen(raw_input_expr);
    char * open_parens[100]; // If you have more than 100 nested parenthesis in a single expression, I don't want you coding, let alone using my tool
    struct PAREN_PAIR pairs[100] = { {NULL, NULL, 0} };
    int count = 0;
    int open_parens_idx = 0;
    int pairs_idx = 0;
    for (int i = 0; i < length; i++) {
        if (raw_input_expr[i] == '(') {
            open_parens[open_parens_idx] = raw_input_expr+i;
            open_parens_idx++;
            count--;
        } else if (raw_input_expr[i] == ')') {
            assert(count < 0);
            pairs[pairs_idx].beginning = open_parens[open_parens_idx-1];
            pairs[pairs_idx].ending = raw_input_expr+i;
            pairs[pairs_idx].depth = count;
            pairs_idx++;
            open_parens_idx--;
            count++;
        }
    }
    assert(count == 0);
    for (int i = 99; i >= 0; i--) {
        if (pairs[i].depth == -1) {
            // START DB
            DBPRINTF("doing stuff in paren_tokenize\n");
            // END DB
            int subexpr_length = pairs[i].ending - pairs[i].beginning;
            char subexpr[subexpr_length];
            memmove(subexpr, pairs[i].beginning+1, subexpr_length-1);
            subexpr[subexpr_length-1] = '\0';
            struct NODE subexpr_head = { NULL, NULL, NULL };
            struct BINDING * envcpy[MAX_CONCURRENT_BINDINGS];
            memmove(envcpy, env, MAX_CONCURRENT_BINDINGS * sizeof(struct BINDING *));
            // START DB
            DBPRINTF("----------------\n");
            DBPRINTF("pairs[i]: ");
            DBPRINTESCSTR((pairs[i].beginning));
            DBPRINTF("pairs[i] loc: %p\n", pairs[i].beginning);
            DBPRINTF("subexpr: ");
            DBPRINTESCSTR((subexpr));
            DBPRINTF("subexpr loc: %p\n", subexpr);
            DBPRINTF("ptrexpr: ");
            DBPRINTESCSTR(raw_input_expr);
            DBPRINTF("ptrexpr loc: %p\n", raw_input_expr);
            DBPRINTF("before shift\n");
            DBPRINTLIST(head);
            // END DB
            if (subexpr > raw_input_expr) {
                shiftNUpInRange(pairs[i].beginning, pairs[i].ending, (unsigned long)subexpr-(unsigned long)(pairs[i].beginning)-1, head);
            } else {
                shiftNDownInRange(pairs[i].beginning, pairs[i].ending, (unsigned long)(pairs[i].beginning)-(unsigned long)subexpr+1, head);
            }
            // START DB
            DBPRINTF("before\n");
            DBPRINTLIST(head);
            GLOBAL_TABS++;
            // END DB
            struct NAMED_TOKEN * nt = tokenize(subexpr, head, envcpy);
            // START DB
            GLOBAL_TABS--;
            DBPRINTF("after\n");
            DBPRINTLIST(head);
            // END DB
            if (subexpr > raw_input_expr) {
                shiftNDownInRange(subexpr, subexpr+strlen(subexpr), (unsigned long)subexpr-(unsigned long)(pairs[i].beginning), head);
            } else {
                shiftNUpInRange(subexpr, subexpr+strlen(subexpr), (unsigned long)(pairs[i].beginning)-(unsigned long)subexpr, head);
            }
            // START DB
            DBPRINTF("after shift back\n");
            DBPRINTLIST(head);
            DBPRINTF("----------------\n");
            // END DB
            int offset = pairs[i].beginning - raw_input_expr;
            int length = strlen(raw_input_expr);
            shiftNDownInRange(pairs[i].ending, pairs[i].ending+strlen(pairs[i].ending), subexpr_length, head);
            raw_input_expr[offset] = '\a';
            for (int i = offset+1; i < (length - subexpr_length + 1); i++) {
                raw_input_expr[i] = raw_input_expr[i+subexpr_length];
            }
            raw_input_expr[1 + length - subexpr_length] = '\0';

        }
    }
}

void pow_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "**");
    while (r) {
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 2;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct POW * pow = malloc(sizeof(struct POW));
        strcpy((nt->name), "POW");
        nt->token = t;
        t->Pow = pow;
        if (*statement_start == '\a' && *statement_end == '\a') {
            pow->lh = tokenFromIndex(statement_start, head);
            pow->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            pow->lh = tokenFromIndex(statement_start, head);
            pow->rh = const_tokenize(r + 2);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            pow->lh = const_tokenize(statement_start);
            pow->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            pow->lh = const_tokenize(statement_start);
            pow->rh = const_tokenize(r + 2);
        }
        n->token = nt;
        insertNode(n, head);
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "**");
    }
}

void mul_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "*");
    while (r) {
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct MUL * mul = malloc(sizeof(struct MUL));
        strcpy((nt->name), "MUL");
        nt->token = t;
        t->Mul = mul;
        if (*statement_start == '\a' && *statement_end == '\a') {
            mul->lh = tokenFromIndex(statement_start, head);
            mul->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            mul->lh = tokenFromIndex(statement_start, head);
            mul->rh = const_tokenize(r + 1);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            mul->lh = const_tokenize(statement_start);
            mul->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            mul->lh = const_tokenize(statement_start);
            mul->rh = const_tokenize(r + 1);
        }
        n->token = nt;
        insertNode(n, head);
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "*");
    }
}

void fdiv_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "//");
    while (r) {
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 2;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct FDIV * fdiv = malloc(sizeof(struct FDIV));
        strcpy((nt->name), "FDIV");
        nt->token = t;
        t->Fdiv = fdiv;
        if (*statement_start == '\a' && *statement_end == '\a') {
            fdiv->lh = tokenFromIndex(statement_start, head);
            fdiv->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            fdiv->lh = tokenFromIndex(statement_start, head);
            fdiv->rh = const_tokenize(r + 2);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            fdiv->lh = const_tokenize(statement_start);
            fdiv->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            fdiv->lh = const_tokenize(statement_start);
            fdiv->rh = const_tokenize(r + 2);
        }
        n->token = nt;
        insertNode(n, head);
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "//");
    }
}

void div_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "/");
    while (r) {
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct DIV * div = malloc(sizeof(struct DIV));
        strcpy((nt->name), "DIV");
        nt->token = t;
        t->Div = div;
        if (*statement_start == '\a' && *statement_end == '\a') {
            div->lh = tokenFromIndex(statement_start, head);
            div->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            div->lh = tokenFromIndex(statement_start, head);
            div->rh = const_tokenize(r + 1);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            div->lh = const_tokenize(statement_start);
            div->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            div->lh = const_tokenize(statement_start);
            div->rh = const_tokenize(r + 1);
        }
        n->token = nt;
        insertNode(n, head);
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "/");
    }
}

void mod_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "%");
    while (r) {
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct MOD * mod = malloc(sizeof(struct MOD));
        strcpy((nt->name), "MOD");
        nt->token = t;
        t->Mod = mod;
        if (*statement_start == '\a' && *statement_end == '\a') {
            mod->lh = tokenFromIndex(statement_start, head);
            mod->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            mod->lh = tokenFromIndex(statement_start, head);
            mod->rh = const_tokenize(r + 1);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            mod->lh = const_tokenize(statement_start);
            mod->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            mod->lh = const_tokenize(statement_start);
            mod->rh = const_tokenize(r + 1);
        }
        n->token = nt;
        insertNode(n, head);
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "%");
    }
}

void plus_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "+");
    while (r) {
        // START DB
        DBPRINTF("raw_input_expr: %p\n", raw_input_expr);
        DBPRINTF("raw_input_expr: ");
        DBPRINTESCSTR(raw_input_expr);
        DBPRINTLIST(head);
        // END DB
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        // START DB
        DBPRINTF("new node index: %p\n", n->index);
        // END DB
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct PLUS * plus = malloc(sizeof(struct PLUS));
        strcpy((nt->name), "PLUS");
        nt->token = t;
        t->Plus = plus;
        if (*statement_start == '\a' && *statement_end == '\a') {
            plus->lh = tokenFromIndex(statement_start, head);
            plus->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            plus->lh = tokenFromIndex(statement_start, head);
            plus->rh = const_tokenize(r + 1);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            plus->lh = const_tokenize(statement_start);
            plus->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            plus->lh = const_tokenize(statement_start);
            plus->rh = const_tokenize(r + 1);
        }
        n->token = nt;
        insertNode(n, head);
        // START DB
        DBPRINTF("----------------\n");
        DBPRINTF("raw_input_expr: ");
        DBPRINTESCSTR(raw_input_expr);
        DBPRINTF("before shift back\n");
        DBPRINTLIST(head);
        DBPRINTF("len of valid indexes: %lu\n", strlen(statement_start));
        // END DB
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        // START DB
        DBPRINTF("after shift back\n");
        DBPRINTLIST(head);
        DBPRINTF("----------------\n");
        // END DB
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "+");
    }
}

void sub_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "-");
    while (r) {
        int length = strlen(raw_input_expr);
        char * ptr = r - 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr > raw_input_expr) { ptr -= 1; }
        char *statement_start = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_start += 1;
        }
        ptr = r + 1;
        while (strchr("0123456789\a", *ptr) != NULL && ptr < (raw_input_expr+length-1)) { ptr += 1; }
        char * statement_end = ptr;
        if (!strchr("0123456789\a", *ptr)) {
            statement_end -= 1;
        }
        int statement_length = 1 + statement_end - statement_start;
        int offset = statement_start - raw_input_expr;
        struct NODE * n = malloc(sizeof(struct NODE));
        n->index = statement_start;
        struct NAMED_TOKEN * nt  = malloc(sizeof(struct NAMED_TOKEN));
        union TOKEN * t = malloc(sizeof(union TOKEN));
        struct SUB * sub = malloc(sizeof(struct SUB));
        strcpy((nt->name), "SUB");
        nt->token = t;
        t->Sub = sub;
        if (*statement_start == '\a' && *statement_end == '\a') {
            sub->lh = tokenFromIndex(statement_start, head);
            sub->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            sub->lh = tokenFromIndex(statement_start, head);
            sub->rh = const_tokenize(r + 1);
            struct NODE * ptr = nodeFromIndex(statement_start, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            sub->lh = const_tokenize(statement_start);
            sub->rh = tokenFromIndex(statement_end, head);
            struct NODE * ptr = nodeFromIndex(statement_end, head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            sub->lh = const_tokenize(statement_start);
            sub->rh = const_tokenize(r + 1);
        }
        n->token = nt;
        insertNode(n, head);
        shiftNDownInRange(statement_start+1, statement_start+strlen(statement_start), statement_length-1, head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        r = strstr(raw_input_expr, "-");
    }
}

#endif //SCALLION_TOKENIZATION_H
