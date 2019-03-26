//
// Created by Alex Bruns on 3/25/19.
//

#ifndef SCALLION_DEBUGGLOBALS_H
#define SCALLION_DEBUGGLOBALS_H

#include <stdio.h>
#include <stdarg.h>

int GLOBAL_TABS = 0;
int DEBUG_TOGGLE = 0;

void DBPRINTF(char* fmt,...) {
    if (DEBUG_TOGGLE) {
        char formatted_string[250];
        for (int tc = 0; tc < GLOBAL_TABS; tc++) { printf("\t"); }
        va_list argptr;
        va_start(argptr,fmt);
        vprintf(fmt, argptr);
        va_end(argptr);
    }
}

#endif //SCALLION_DEBUGGLOBALS_H
