//
// Created by Alex Bruns on 3/9/19.
//

// library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

// local includes
#include "integerExpr.h"
#include "alexBrunsDataStructs.h"
#include "exprUtils.h"

// prototypes
char * read_in(FILE *);
struct NAMED_TOKEN * tokenize(char *, struct NAMED_TOKEN *);
struct NAMED_TOKEN * const_tokenize(char *);
struct NAMED_TOKEN * plus_tokenize(char *, struct NAMED_TOKEN *);
struct NAMED_TOKEN * sub_tokenize(char *, struct NAMED_TOKEN *);
struct NAMED_TOKEN * mul_tokenize(char *, struct NAMED_TOKEN *);
struct NAMED_TOKEN * div_tokenize(char *, struct NAMED_TOKEN *);

// globals
struct NODE head = { NULL, NULL, NULL };

int main(int argc, char ** argv) {
    assert(argc == 2);
    const char * input_file_name = argv[1];
    FILE * input_file_ptr = fopen(input_file_name, "r");
    char * input_file_on_heap = read_in(input_file_ptr);
    printf("-  - -- ---> START STRING INPUT\n"
           "'%s'\n"
           "-  - -- ---> END STRING INPUT\n", input_file_on_heap);
    struct NAMED_TOKEN * expr_head = tokenize(input_file_on_heap, NULL);
    printf("-  - -- ---> START TOKENIZED OUTPUT\n");
    printExpr(expr_head);
    printf("\n-  - -- ---> END TOKENIZED OUTPUT\n");
    free(input_file_on_heap);
    return 0;
}

char * read_in(FILE * input_file_ptr) {
    fseek(input_file_ptr, 0L, SEEK_END);
    long file_size = ftell(input_file_ptr);
    assert(file_size > 0);
    rewind(input_file_ptr);
    char * input_file_on_heap = malloc(file_size*sizeof(char));
    assert(input_file_on_heap);
    printf("%ld\n%u\n", file_size, (unsigned int) file_size);
    fscanf(input_file_ptr, "%s", input_file_on_heap);
    return input_file_on_heap;
}

struct NAMED_TOKEN * tokenize(char * raw_input_expr, struct NAMED_TOKEN * lh) {
    printf("DEBUG:\ttokenize on ('%s', %s)\n", raw_input_expr, lh->name);

    /*** parenthesis pass    ***/
        // TODO

    /*** exponentiation pass ***/
    printf("pow pass -------\n");
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
            pow->lh = tokenFromIndex(statement_start, &head);
            pow->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            pow->lh = tokenFromIndex(statement_start, &head);
            pow->rh = const_tokenize(statement_end);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            pow->lh = const_tokenize(statement_start);
            pow->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            pow->lh = const_tokenize(statement_start);
            pow->rh = const_tokenize(statement_end);
        }
        n->token = nt;
        insertNode(n, &head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, &head);
        r = strstr(raw_input_expr, "**");
    }

    /*** multiplication pass ***/
    printf("mul pass -------\n");
    r = strstr(raw_input_expr, "*");
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
            mul->lh = tokenFromIndex(statement_start, &head);
            mul->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            mul->lh = tokenFromIndex(statement_start, &head);
            mul->rh = const_tokenize(statement_end);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            mul->lh = const_tokenize(statement_start);
            mul->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            mul->lh = const_tokenize(statement_start);
            mul->rh = const_tokenize(statement_end);
        }
        n->token = nt;
        insertNode(n, &head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, &head);
        r = strstr(raw_input_expr, "*");
    }

    /*** division pass ***/
    printf("div pass -------\n");
    r = strstr(raw_input_expr, "/");
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
            div->lh = tokenFromIndex(statement_start, &head);
            div->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            div->lh = tokenFromIndex(statement_start, &head);
            div->rh = const_tokenize(statement_end);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            div->lh = const_tokenize(statement_start);
            div->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            div->lh = const_tokenize(statement_start);
            div->rh = const_tokenize(statement_end);
        }
        n->token = nt;
        insertNode(n, &head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, &head);
        r = strstr(raw_input_expr, "/");
    }


    /*** plus pass ***/
    printf("plus pass -------\n");
    r = strstr(raw_input_expr, "+");
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
            plus->lh = tokenFromIndex(statement_start, &head);
            plus->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            plus->lh = tokenFromIndex(statement_start, &head);
            plus->rh = const_tokenize(statement_end);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            plus->lh = const_tokenize(statement_start);
            plus->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            plus->lh = const_tokenize(statement_start);
            plus->rh = const_tokenize(statement_end);
        }
        n->token = nt;
        insertNode(n, &head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, &head);
        r = strstr(raw_input_expr, "+");
    }

    /*** sub pass ***/
    printf("sub pass -------\n");
    r = strstr(raw_input_expr, "-");
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
            sub->lh = tokenFromIndex(statement_start, &head);
            sub->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
            ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL;
        } else if (*statement_start == '\a') {
            sub->lh = tokenFromIndex(statement_start, &head);
            sub->rh = const_tokenize(statement_end);
            struct NODE * ptr = nodeFromIndex(statement_start, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else if (*statement_end == '\a') {
            sub->lh = const_tokenize(statement_start);
            sub->rh = tokenFromIndex(statement_end, &head);
            struct NODE * ptr = nodeFromIndex(statement_end, &head);
            ptr->index = NULL; // -1 denotes a sub expr token (that is a token which is wrapped by an outer token)
        } else {
            sub->lh = const_tokenize(statement_start);
            sub->rh = const_tokenize(statement_end);
        }
        n->token = nt;
        insertNode(n, &head);
        raw_input_expr[offset] = '\a';
        for (int i = offset+1; i < (length - statement_length + 1); i++) {
            raw_input_expr[i] = raw_input_expr[i+statement_length-1];
        }
        raw_input_expr[1 + length - statement_length] = '\0';
        shiftNPastIndex(statement_start, statement_length-1, &head);
        r = strstr(raw_input_expr, "-");
    }
    struct NODE * obj_node = nodeFromIndex(raw_input_expr, &head);
    return obj_node->token;
}

struct NAMED_TOKEN * const_tokenize(char * raw_input_expr) {
    printf("DEBUG:\tconst_tokenize on ('%s')\n", raw_input_expr);
    int const_length = strcspn(raw_input_expr, "0123456789") + 1;
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

struct NAMED_TOKEN * plus_tokenize(char * raw_input_expr, struct NAMED_TOKEN * lh) {
    printf("DEBUG:\tplus_tokenize on ('%s', %s)\n", raw_input_expr, lh->name);
    struct NAMED_TOKEN * nt = malloc(sizeof(struct NAMED_TOKEN));
    union TOKEN * t = malloc(sizeof(union TOKEN));
    struct PLUS * plus = malloc(sizeof(struct PLUS));
    strcpy((nt->name), "PLUS");
    nt->token = t;
    t->Plus = plus;
    plus->lh = lh;
    plus->rh = tokenize(raw_input_expr+1, NULL);
    return nt;
}

struct NAMED_TOKEN * sub_tokenize(char * raw_input_expr, struct NAMED_TOKEN * lh) {
    if (!lh) { return NULL; } // TODO : handle negation (-b instead of a-b)
    printf("DEBUG:\tsub_tokenize on ('%s', %s)\n", raw_input_expr, lh->name);
    struct NAMED_TOKEN * nt = malloc(sizeof(struct NAMED_TOKEN));
    union TOKEN * t = malloc(sizeof(union TOKEN));
    struct SUB * sub = malloc(sizeof(struct SUB));
    strcpy((nt->name), "SUB");
    nt->token = t;
    t->Sub = sub;
    sub->lh = lh;
    sub->rh = tokenize(raw_input_expr+1, NULL);
    return nt;
}

struct NAMED_TOKEN * mul_tokenize(char * raw_input_expr, struct NAMED_TOKEN * lh) {
    if (*(raw_input_expr+1) == '*') { return NULL; } // TODO : handle ** (pow)
    printf("DEBUG:\tmul_tokenize on ('%s', %s)\n", raw_input_expr, lh->name);
    struct NAMED_TOKEN * nt = malloc(sizeof(struct NAMED_TOKEN));
    union TOKEN * t = malloc(sizeof(union TOKEN));
    struct MUL * mul = malloc(sizeof(struct MUL));
    strcpy((nt->name), "MUL");
    nt->token = t;
    t->Mul = mul;
    mul->lh = lh;
    mul->rh = tokenize(raw_input_expr+1, NULL);
    return nt;
}

struct NAMED_TOKEN * div_tokenize(char * raw_input_expr, struct NAMED_TOKEN * lh) {
    if (*(raw_input_expr+1) == '/') { return NULL; } // TODO : handle // (floored division)
    printf("DEBUG:\tdiv_tokenize on ('%s', %s)\n", raw_input_expr, lh->name);
    struct NAMED_TOKEN * nt = malloc(sizeof(struct NAMED_TOKEN));
    union TOKEN * t = malloc(sizeof(union TOKEN));
    struct DIV * div = malloc(sizeof(struct DIV));
    strcpy((nt->name), "DIV");
    nt->token = t;
    t->Div = div;
    div->lh = lh;
    div->rh = tokenize(raw_input_expr+1, NULL);
    return nt;
}



