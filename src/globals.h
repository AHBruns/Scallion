//
// Created by Alex Bruns on 3/26/19.
//

#ifndef SCALLION_GLOBALS_H
#define SCALLION_GLOBALS_H

// globals
struct NODE HEAD = { NULL, NULL, NULL };
const char * TOKEN_CHAR = "\a";
const char * OP_CHARS = "+-/%*^&!><=&|~^)({}][";
const char * LOWERCASE_ALPHA_CHARS = "abcdefghijklmnopqrstuvwxyz";
const char * UPPERCASE_ALPHA_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char * ALL_ALPHA_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char * NON_INT_CHARS = "+-/%*^&!><=&|~^)({}][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\a";
const int MAX_LINES = 100;
const int MAX_CONCURRENT_BINDINGS = 100;

#endif //SCALLION_GLOBALS_H
