#include "cipher.h"

#define LETTER_NUM 26


char coder_help(char cha, int move, char start) {
    /* this function shift the current given char by cyclic */
    move = (cha + move - start) % LETTER_NUM + LETTER_NUM;
    return (char) (start + move % LETTER_NUM);
}

char coder(char cha, int move) {
    /* this function get char, and moves, returns the new crypt char */
    if ('A' <= cha && cha <= 'Z') {
        return coder_help(cha, move, 'A');
    } else if ('a' <= cha && cha <= 'z') {
        return coder_help(cha, move, 'a');
    }
    return cha;
}

// See full documentation in header file
void cipher(char s[], int k) {
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] = coder(s[i], k);
    }
}

// See full documentation in header file
void decipher(char s[], int k) {
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] = coder(s[i], (-1) * k);
    }
}
