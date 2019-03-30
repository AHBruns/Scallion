//
// Created by Alex Bruns on 3/9/19.
//

//
// TODO
// - convert old debug statements to new DBPRINTF function calls
// - finish implementing parsing for existing arith operations
// - implement sequence token
// - figure out how to do functions...
// - bitwise operations
// - refactor tokenization operations to assume args are tokenized constants
//

// library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

// local includes
#include "lexing/lexer.h"
#include "parsing/parser.h"
#include "structures/linked_list.h"
#include "utils/utils.h"

// prototypes
char * read_in(FILE *);

int main(int argc, char ** argv) {
    assert(argc >= 2);
    bool lex_rep_flag = false;
    bool ast_rep_flag = false;
    if (argc >= 3) {
        if (streq(argv[2], "--lex")) lex_rep_flag = true;
        else if (streq(argv[2], "--ast")) ast_rep_flag = true;
        else if (streq(argv[2], "--all")) {
            lex_rep_flag = true;
            ast_rep_flag = true;
        }
    }
    if (argc >= 4) {
        if (streq(argv[3], "--lex")) lex_rep_flag = true;
        else if (streq(argv[3], "--ast")) ast_rep_flag = true;
        else if (streq(argv[3], "--all")) {
            lex_rep_flag = true;
            ast_rep_flag = true;
        }
    }
    const char * input_file_name = argv[1];
    FILE * input_file_ptr = fopen(input_file_name, "r");
    char * program_str = read_in(input_file_ptr);
    struct NODE * program = ll_gen_head();
    lex(program_str, program, 0, 0);
    if (lex_rep_flag) {
        printf("Lexicographical Representation:\n");
        printf_symbols(program);
    }
    program = parse(program->next);
    if (ast_rep_flag) {
        printf("Abstract-Syntax-Tree (AST) Representation:\n");
        printf_expr_tree(program->contents);
    }
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