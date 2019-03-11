//
// Created by Alex Bruns on 3/11/19.
//

#ifndef SCALLION_EXPRUTILS_H
#define SCALLION_EXPRUTILS_H

#include <string.h>

#include "integerExpr.h"
#include "alexBrunsDataStructs.h"

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
    } else if (strcmp(head->name, "DIV") == 0) {
        printf("DIV (");
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

void shiftNPastIndex(void * index, int shift, struct NODE * head) {
    struct NODE * ptr = head;
    while (ptr != NULL) {
        if (ptr->index > index) {
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

#endif //SCALLION_EXPRUTILS_H
