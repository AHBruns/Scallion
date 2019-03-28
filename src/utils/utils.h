//
// Created by Alex Bruns on 3/28/19.
//

#ifndef SCALLION_UTILS_H
#define SCALLION_UTILS_H

int char_count_of_integer(long v) {
    if (v == 0) return 1; // edge case
    int len = 0;
    if (v < 0) {
        len++; // for the negative sign
        v *= -1;
    }
    while (v != 0) {
        v /= 10;
        len++;
    }
    return len;
}

bool streq(char * s1, char * s2) {
    if (strcmp(s1, s2) == 0) return true;
    else return false;
}

#endif //SCALLION_UTILS_H
