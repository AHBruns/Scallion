//
// Created by Alex Bruns on 3/28/19.
//

#include <stdio.h>

#include "../lexing/lexer.h"
#include "parser.h"
#include "../structures/linked_list.h"
#include "parse_symbols.h"

int main(int argc, char ** argv) {
    struct NODE * head = ll_gen_head();
    lex(argv[1], head, 0, 0);
    struct NODE * ptr = head->next;
    printf("lexicographical representation:\n");
    printf("[");
    while (ptr != head) {
        printf(" %s,", ((struct N_SYM *)ptr->contents)->name);
        ptr = ptr->next;
    }
    printf("\b ]\n\n");
    ptr = head->next;
    ptr = parse(ptr);
    printf("abstract syntax tree representation:\n");
    printf_expr_tree(ptr->contents);
}
