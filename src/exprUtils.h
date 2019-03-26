//
// Created by Alex Bruns on 3/11/19.
//

#ifndef SCALLION_EXPRUTILS_H
#define SCALLION_EXPRUTILS_H

#include <math.h>
#include <string.h>

#include "exprs.h"
#include "alexBrunsDataStructs.h"
#include "results.h"
#include "globalsDebug.h"

void DBPRINTESCSTR(char * s) {
    if (DEBUG_TOGGLE) {
        int i = 0;
        while (*s != '\0') {
            if (*s == '\a') {
                printf("PTR_%d", i);
                i++;
            }
            printf("%c", *s);
            s++;
        }
        printf("\n");
    }
}

void printEscStr(char * s) {
    int i = 0;
    while (*s != '\0') {
        if (*s == '\a') {
            printf("PTR_%d", i);
            i++;
        }
        printf("%c", *s);
        s++;
    }
    printf("\n");
}

void printExpr(struct NAMED_TOKEN * head) {
    if (strcmp(head->name, "PLUS") == 0) {
        printf("PLUS (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "SUB") == 0) {
        printf("SUB (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "MUL") == 0) {
        printf("MUL (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "FDIV") == 0) {
        printf("FDIV (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "DIV") == 0) {
        printf("DIV (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "MOD") == 0) {
        printf("MOD (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "POW") == 0) {
        printf("POW (");
        printExpr(head->token->Plus->lh);
        printf(", ");
        printExpr(head->token->Plus->rh);
        printf(")");
    } else if (strcmp(head->name, "CONST") == 0) {
        printf("CONST (%d)", head->token->Const->v);
    }
}

void printResult(struct NAMED_RESULT * nr) {
    if (strcmp(nr->name, "R_INT") == 0) {
        printf("type: integer, value: %d\n", nr->result.Int.v);
    } else if (strcmp(nr->name, "R_FLOAT") == 0) {
        if (round(nr->result.Float.v) == nr->result.Float.v) {
            printf("type: floating point , value: %ld\n", (long)nr->result.Float.v);
        } else {
            printf("type: floating point , value: %f\n", nr->result.Float.v);
        }
    } else if (strcmp(nr->name, "R_CHAR") == 0) {
        printf("type: char, value: %c\n", nr->result.Char.v);
    } else if (strcmp(nr->name, "R_BOOL") == 0) {
        if (nr->result.Bool.v == 1) {
            printf("type: bool, value: True\n");
        } else {
            printf("type: bool, value: False\n");
        }
    } else {
        printf("%s\n", nr->name);
    }
}

// parens helpers
void printParenPairList(struct PAREN_PAIR pairs[100]) {
    printf("\n\n");
    for (int i = 0; i < 100; i++) {
        if (pairs[i].depth < 0) {
            printf("- - - - - - -\nstart: %s\nend:%s\ndepth: %d\n- - - - - - -\n", pairs[i].beginning, pairs[i].ending, pairs[i].depth);
        }
    }
    printf("\n");
}

// basic linked list implementation

struct NODE * nodeFromIndex(void * index, struct NODE * head) {
    struct NODE * ptr = head;
    while (ptr != NULL) {
        if (ptr->index == index) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

struct NAMED_TOKEN * tokenFromIndex(void * index, struct NODE * head) {
    struct NODE * ptr = head;
    while (ptr != NULL) {
        if (ptr->index == index) {
            return ptr->token;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void insertNode(struct NODE * node2insert, struct NODE * head) {
    void * n = node2insert->index;
    struct NODE * ptr = head;
    while (ptr->next != NULL && ptr->index < n) { ptr = ptr->next; }
    struct NODE * tmp = ptr->next;
    ptr->next = node2insert;
    node2insert->next = tmp;
}

// deprecated
void shiftNPastIndex(void * index, long shift, struct NODE * head) {
    struct NODE * ptr = head;
    while (ptr != NULL) {
        if (ptr->index > index) {
            ptr->index -= shift;
        }
        ptr = ptr->next;
    }
}

// min inclusive, max inclusive
void shiftNUpInRange(void * min, void * max, unsigned long shift, struct NODE * head) {
    struct NODE * ptr = head;
    while (ptr != NULL) {
        if (ptr->index >= min && ptr->index <= max) {
            ptr->index += shift;
        }
        ptr = ptr->next;
    }
}

// min inclusive, max inclusive
void shiftNDownInRange(void * min, void * max, unsigned long shift, struct NODE * head) {
    struct NODE * ptr = head;
    while (ptr != NULL) {
        if (ptr->index >= min && ptr->index <= max) {
            ptr->index -= shift;
        }
        ptr = ptr->next;
    }
}

void freeList(struct NODE * head) {
    if (head) {
        struct NODE * tmp =  head->next;
        free(head);
        freeList(tmp);
    }
}

void DBPRINTLIST(struct NODE * head) {
    if (DEBUG_TOGGLE) {
        for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
        printf("--\n");
        struct NODE *ptr = head;
        int i = 0;
        while (ptr) {
            if (DEBUG_TOGGLE) {
                for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
            }
            printf("%d: ", i);
            if (ptr->token != NULL && ptr->index != NULL) {
                printf("%p ", ptr->index);
                printExpr(ptr->token);
            }
            ptr = ptr->next;
            i++;
            printf("\n");
        }
        if (DEBUG_TOGGLE) {
            for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
        }
        printf("--\n");
    }
}


void printList(struct NODE * head) {
    for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
    printf("--\n");
    struct NODE * ptr = head;
    int i = 0;
    while (ptr) {
        if (DEBUG_TOGGLE) {
            for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
        }
        printf("%d: ", i);
        if (ptr->token != NULL && ptr->index != NULL) {
            printf("%p ", ptr->index);
            printExpr(ptr->token);
        }
        ptr = ptr->next;
        i++;
        printf("\n");
    }
    if (DEBUG_TOGGLE) {
        for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
    }
    printf("--\n");
}

#endif //SCALLION_EXPRUTILS_H
