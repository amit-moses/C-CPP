#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define ERR_ARGS "The program receives 1 or 4 arguments only.\n"
#define ERR_TEST "Usage: cipher test\n"
#define ERR_COMMAND "The given command is invalid.\n"
#define ERR_SHIFT "The given shift value is invalid.\n"
#define ERR_FILE "The given file is invalid.\n"
#define ENCODE "cipher"
#define DECODE "decipher"
#define TEST "test"
enum MAGIC {
    LOW_ARG = 2,
    MAX_ARG = 5,
    RADIX = 10
};

int cipher_tests() {
    /* Run cipher tests */
    if (test_cipher_non_cyclic_lower_case_positive_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_cipher_cyclic_lower_case_special_char_positive_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_cipher_non_cyclic_lower_case_special_char_negative_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_cipher_cyclic_lower_case_negative_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_cipher_cyclic_upper_case_positive_k() != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int decipher_tests() {
    /* Run decipher tests */
    if (test_decipher_non_cyclic_lower_case_positive_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_decipher_cyclic_lower_case_special_char_positive_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_decipher_non_cyclic_lower_case_special_char_negative_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_decipher_cyclic_lower_case_negative_k() != 0) {
        return EXIT_FAILURE;
    }
    if (test_decipher_cyclic_upper_case_positive_k() != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int valid_args(int argc, char *argv[]) {
    /* this function checks the validation of the arguments */
    if (argc != LOW_ARG && argc != MAX_ARG) {
        fprintf(stderr, ERR_ARGS);
        return EXIT_FAILURE;
    }
    if (argc == LOW_ARG) {
        if (strcmp(argv[1], TEST) != 0) {
            fprintf(stderr, ERR_TEST);
            return EXIT_FAILURE;
        } else {
            /* checks al tests */
            return cipher_tests() || decipher_tests();
        }
    }

    if (strcmp(argv[1], ENCODE) != 0 && strcmp(argv[1], DECODE) != 0) {
        fprintf(stderr, ERR_COMMAND);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[2], "0") != 0) {
        int number = strtol(argv[2], NULL, RADIX);
        if (!number || strchr(argv[2], '.')) {
            fprintf(stderr, ERR_SHIFT);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int valid_file(char input[], char output[], int act, int k) {
    /* this function checks the validation of the files */
    char line[MAX_LINE] = {0};
    FILE *f_in = fopen(input, "r");
    FILE *f_ou = fopen(output, "w");
    if (f_in == NULL || f_ou == NULL) {
        fprintf(stderr, ERR_FILE);
        return EXIT_FAILURE;
    }

    /* files and params correct - encode or decode */
    if (act) {
        while (fgets(line, MAX_LINE, f_in)) {
            cipher(line, k);
            fputs(line, f_ou);
        }
    } else {
        while (fgets(line, MAX_LINE, f_in)) {
            decipher(line, k);
            fputs(line, f_ou);
        }
    }

    fclose(f_ou);
    fclose(f_in);
    return EXIT_SUCCESS;

}

int main(int argc, char *argv[]) {
    if (valid_args(argc, argv)) {
        return EXIT_FAILURE;
    } else if (strcmp(argv[1], TEST) != 0) {
        int act = argv[1][0] == ENCODE[0];
        int shift = strtol(argv[2], NULL, RADIX);
        return valid_file(argv[3], argv[4], act, shift);
    }
    return EXIT_SUCCESS;
}
