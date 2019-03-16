//
// Created by Alex Bruns on 3/9/19.
//

// library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

// local public includes
#include "uthash.h"

// local includes
#include "exprs.h"
#include "alexBrunsDataStructs.h"
#include "exprUtils.h"
#include "results.h"

// prototypes
char * read_in(FILE *);
char ** sequencize(char *);
struct NAMED_TOKEN * tokenize(char *, struct NODE *);
struct NAMED_TOKEN * const_tokenize(char *);
void binding_tokenize(char *, struct NODE *);
void paren_tokenize(char *, struct NODE *);
void pow_tokenize(char *, struct NODE *);
void mul_tokenize(char *, struct NODE *);
void fdiv_tokenize(char *, struct NODE *);
void div_tokenize(char *, struct NODE *);
void mod_tokenize(char *, struct NODE *);
void plus_tokenize(char *, struct NODE *);
void sub_tokenize(char *, struct NODE *);
//int parse(struct NAMED_TOKEN *);

// globals
struct NODE HEAD = { NULL, NULL, NULL };
char * OP_CHARS = "+-/%*^&!><=&|~^";
int MAX_LINES = 100;

int main(int argc, char ** argv) {
    assert(argc == 2);
    const char * input_file_name = argv[1];
    FILE * input_file_ptr = fopen(input_file_name, "r");
    char * input_file_on_heap = read_in(input_file_ptr);
    char ** expr_sequence = sequencize(input_file_on_heap);
    int i = 0;
    while (i < MAX_LINES && (long) expr_sequence[i] != 0) {
        printf("\n-  - -- ---> START INPUT LINE\n"
               "%s\n"
               "-  - -- ---> END INPUT LINE\n", expr_sequence[i]);
        struct NAMED_TOKEN *expr_head = tokenize(expr_sequence[i], &HEAD);
        printf("\n");
        printf("-  - -- ---> START TOKENIZED LINE\n");
        printExpr(expr_head);
        printf("\n-  - -- ---> END TOKENIZED LINE\n\n");
    }
    free(input_file_on_heap);
    return 0;
}

char * read_in(FILE * input_file_ptr) {
    assert(input_file_ptr != NULL);
    fseek(input_file_ptr, 0L, SEEK_END);
    long numbytes = ftell(input_file_ptr);
    fseek(input_file_ptr, 0L, SEEK_SET);
    char * input_file_on_heap = (char*)calloc(numbytes, sizeof(char));
    assert(input_file_on_heap != NULL);
    fread(input_file_on_heap, sizeof(char), numbytes, input_file_ptr);
    fclose(input_file_ptr);
    return input_file_on_heap;
}

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

struct NAMED_TOKEN * tokenize(char * raw_input_expr, struct NODE * node) {
    /*** binding pass ***/
    binding_tokenize(raw_input_expr, node);
    /*** parenthesis pass ***/
    paren_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after paren_tokenize: %s\n", raw_input_expr);
    /*** exponentiation pass ***/
    pow_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after pow_tokenize: %s\n", raw_input_expr);
    /*** multiplication pass ***/
    mul_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after mul_tokenize: %s\n", raw_input_expr);
    /*** floor division pass ***/
    fdiv_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after fdiv_tokenize: %s\n", raw_input_expr);
    /*** division pass ***/
    div_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after div_tokenize: %s\n", raw_input_expr);
    /*** modulo pass ***/
    mod_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after mod_tokenize: %s\n", raw_input_expr);
    /*** plus pass ***/
    plus_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after plus_tokenize: %s\n", raw_input_expr);
    /*** sub pass ***/
    sub_tokenize(raw_input_expr, node);
    // printf("raw_input_expr after sub_tokenize: %s\n", raw_input_expr);
    struct NAMED_TOKEN * ret = nodeFromIndex(raw_input_expr, node)->token;
    freeList(node->next); // you don't free the head because its not on the heap
    return ret;
}

struct NAMED_TOKEN * const_tokenize(char * raw_input_expr) {
    int const_length = strcspn(raw_input_expr, "+-*/%=><()abcdefghijklmnopqrstuvwxyz\a"); // length till non-int character
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

// TODO | implement actual env rather than just destroying bindings
void binding_tokenize(char * raw_input_expr, struct NODE * head) {
    printf("before: %s\n", raw_input_expr);
    char * binding_loc = strchr(raw_input_expr, '=');
    if (!binding_loc) {
        return;
    } else {
        if (strchr(OP_CHARS, binding_loc[1]) || strchr(OP_CHARS, *(binding_loc-1)))
            return;
        memmove(raw_input_expr, binding_loc+1, strlen(binding_loc));
    }
    printf("after: %s\n", raw_input_expr);
}

void paren_tokenize(char * raw_input_expr, struct NODE * head) {
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
            int subexpr_length = pairs[i].ending - pairs[i].beginning;
            char subexpr[subexpr_length];
            memmove(subexpr, pairs[i].beginning+1, subexpr_length-1);
            subexpr[subexpr_length-1] = '\0';
            struct NODE subexpr_head = { NULL, NULL, NULL };
            struct NAMED_TOKEN * nt = tokenize(subexpr, &subexpr_head);
            struct NODE * node = malloc(sizeof(struct NODE));
            node->token = nt;
            node->index = pairs[i].beginning;
            insertNode(node, head);
            int offset = pairs[i].beginning - raw_input_expr;
            int length = strlen(raw_input_expr);
            raw_input_expr[offset] = '\a';
            for (int i = offset+1; i < (length - subexpr_length + 1); i++) {
                raw_input_expr[i] = raw_input_expr[i+subexpr_length];
            }
            raw_input_expr[1 + length - subexpr_length] = '\0';
            shiftNPastIndex(pairs[i].beginning, subexpr_length, head);
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
        r = strstr(raw_input_expr, "%");
    }
}

void plus_tokenize(char * raw_input_expr, struct NODE * head) {
    char * r = strstr(raw_input_expr, "+");
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
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
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, head);
        r = strstr(raw_input_expr, "-");
    }
}

/*** PARSING ***/
//int parse(struct NAMED_TOKEN * root) { // TODO : add env
//    int sig = atoi(root->name);
//    int const_int = atoi("CONST");
//    int plus_int = atoi("PLUS");
//    int sub_int = atoi("SUB");
//    switch (sig) {
//        case (const_int): return (root->token->Const->v);
//        case (plus_int): return (parse (root->token->Plus->lh) + parse (root->token->Plus->rh));
//        case (sub_int): return (parse (root->token->Sub->lh) - parse (root->token->Sub->rh));
//    }
//}