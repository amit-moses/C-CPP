#include "tests.h"
#include <string.h>

#define K_1 3
#define K_2 2
#define K_3 (-1)
#define K_4 (-3)
#define K_5 29

// See full documentation in header file

int test_cipher_non_cyclic_lower_case_positive_k() {
    char in[] = "abc";
    char out[] = "def";
    cipher(in, K_1);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_cipher_cyclic_lower_case_special_char_positive_k() {
    char in[] = "x!y-z";
    char out[] = "z!a-b";
    cipher(in, K_2);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_cipher_non_cyclic_lower_case_special_char_negative_k() {
    char in[] = "hello, how r u?";
    char out[] = "gdkkn, gnv q t?";
    cipher(in, K_3);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_cipher_cyclic_lower_case_negative_k() {
    char in[] = "amit";
    char out[] = "xjfq";
    cipher(in, K_4);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_cipher_cyclic_upper_case_positive_k() {
    char in[] = "AMIT";
    char out[] = "DPLW";
    cipher(in, K_5);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_decipher_non_cyclic_lower_case_positive_k() {
    char in[] = "def";
    char out[] = "abc";
    decipher(in, K_1);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_decipher_cyclic_lower_case_special_char_positive_k() {
    char in[] = "z!a-b";
    char out[] = "x!y-z";
    decipher(in, K_2);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_decipher_non_cyclic_lower_case_special_char_negative_k() {
    char in[] = "gdkkn, gnv q t?";
    char out[] = "hello, how r u?";
    decipher(in, K_3);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_decipher_cyclic_lower_case_negative_k() {
    char in[] = "xjfq";
    char out[] = "amit";
    decipher(in, K_4);
    return strcmp(in, out) != 0;
}

// See full documentation in header file
int test_decipher_cyclic_upper_case_positive_k() {
    char in[] = "DPLW";
    char out[] = "AMIT";
    decipher(in, K_5);
    return strcmp(in, out) != 0;
}
