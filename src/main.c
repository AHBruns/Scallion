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
#include "exprs.h"
#include "alexBrunsDataStructs.h"
#include "exprUtils.h"
#include "results.h"
#include "globalsDebug.h"
#include "parsing.h"
#include "tokenization.h"
#include "globals.h"

// prototypes
char * read_in(FILE *);

int main(int argc, char ** argv) {
    assert(argc == 2);
    const char * input_file_name = argv[1];
    FILE * input_file_ptr = fopen(input_file_name, "r");
    char * input_file_on_heap = read_in(input_file_ptr);
    char ** expr_sequence = sequencize(input_file_on_heap);
    int i = 0;
    struct BINDING * env[MAX_CONCURRENT_BINDINGS] = {0};
    while (i < MAX_LINES && (long) expr_sequence[i] != 0) {
        printf("%s\n", expr_sequence[i]);
        struct NAMED_TOKEN *expr_head = tokenize(expr_sequence[i], &HEAD, env);
        printExpr(expr_head);
        printf("\n");
        free(expr_sequence[i]);
        i++;
        DBPRINTLIST(&HEAD);
        struct NAMED_RESULT * nr = simplify(expr_head);
        DBPRINTLIST(&HEAD);
        DBPRINTF("%s\n", nr->name);
        printResult(nr);
        freeList(HEAD.next);
        HEAD.index = NULL;
        HEAD.token = NULL;
        HEAD.next = NULL;
        printf("\n");
    }
    free(expr_sequence);
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