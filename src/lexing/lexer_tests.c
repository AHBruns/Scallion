//
// Created by Alex Bruns on 3/28/19.
//

#include <stdio.h>

#include "lexer.h"
#include "../structures/linked_list.h"

int main(int argc, char ** argv) {
    struct NODE * head = ll_gen_head();
    lex(argv[1], head, 0, 0);
    struct NODE * ptr = head->next;
    printf("[");
    while (ptr != head) {
        printf(" %s,", ((struct N_SYM *)ptr->contents)->name);
        ptr = ptr->next;
    }
    printf("\b ]\n");
}