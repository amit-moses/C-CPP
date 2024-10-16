#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

#define MAX_INPUT 60
#define BASE 10
#define MIN_DISTANCE 0
#define MAX_DISTANCE 1000
#define MIN_DURATION 10
#define MAX_DURATION 100
#define ENTER1 "Enter number of lines. Then enter\n"
#define ENTER2 "Enter line info. Then enter\n"
#define ERR_LINE "Error: Number of lines should be a positive integer\n"
#define ERR_NAME "Error: bus name should contains only digits "\
"and small chars\n"
#define ERR1 "Error: duration should be an integer between %d "\
"and %d (includes).\n"
#define ERR2 "Error: distance should be an integer between %d "\
"and %d (includes).\n"
#define ERR_FORMAT "Error: 3 arguments should be given.\n"
#define TEST_MESSAGE "TEST %d %s: %s\n"
#define PASSED "PASSED"
#define FAILED "FAILED"
#define TESTERR5 "Not sorted by name"
#define TESTERR1 "Not sorted by distance"
#define TESTERR3 "Not sorted by duration"
#define TESTOK5 "The array is sorted by name"
#define TESTOK1 "The array is sorted by distance"
#define TESTOK3 "The array is sorted by duration"
#define TESTOKEQ "The array has the same items after sorting"

#define TESTERR_EQUAL "Not equal"
#define TEST1 1
#define TEST3 3
#define TEST5 5
#define SORT_DURATION "by_duration"
#define SORT_DISTANCE "by_distance"
#define SORT_NAME "by_name"
#define TEST "test"
#define ARGS_ERROR "Usage: by_duration/by_distance/by_name/test\n"

/* This function gets char and returns if is number */
int is_number(char ch) {
    return '0' <= ch && ch <= '9';
}

/* This function gets string and return its contain just numbers */
int is_all_numbers(const char *input, int args) {
    for (int i = 0; i < args; i++) {
        if (!is_number(*(input + i))) {
            return 0;
        }
    }
    return 1;
}

/* This function checks if the given char before comma is valid
 * and the rest of is just numbers*/
int in_range(char ch, int comma) {
    int check = is_number(ch);
    return comma ? check : (check || ('a' <= ch && ch <= 'z'));
}

/* This function gets pointer to string and check if is valid
 * returns the index of the first comma if is valid else 0*/
int valid_input(const char *ptr, int length) {
    int comma_count = 0, first_comma = 0;
    for (int i = 0; i < length; i++) {
        if (!in_range(*(ptr + i), comma_count)) {
            if (*(ptr + i) == ',' && i) {
                comma_count++;
                if (!first_comma) {
                    first_comma = i;
                }
            } else {
                /* printing the errors before return */
                if (!first_comma) {
                    fprintf(stdout, ERR_NAME);
                } else if (comma_count == 1) {
                    fprintf(stdout, ERR2, MIN_DISTANCE, MAX_DISTANCE);
                } else {
                    fprintf(stdout, ERR1, MIN_DURATION, MAX_DURATION);
                }
                return 0;
            }
        }
    }
    if (comma_count == 2) {
        return first_comma;
    } else {
        fprintf(stdout, ERR_FORMAT);
        return 0;
    }
}

/* This function gets input, checks validation of the input
 * if is valid creating BusLine struct and locating in the array*/
int valid_numbers(char *input, int input_size, BusLine *ptr) {
    /* check name validation  */
    int comma = valid_input(input, input_size);
    if (!comma) {
        return EXIT_FAILURE;
    }
    char *endptr = input;

    /* check distance validation  */
    endptr += comma + 1;
    int dist = strtol(endptr, &endptr, BASE);
    if (MIN_DISTANCE > dist || dist > MAX_DISTANCE) {
        fprintf(stdout, ERR2, MIN_DISTANCE, MAX_DISTANCE);
        return EXIT_FAILURE;
    }

    /* check duration validation */
    endptr++;
    int dur = strtol(endptr, &endptr, BASE);
    if (MIN_DURATION > dur || dur > MAX_DURATION) {
        fprintf(stdout, ERR1, MIN_DURATION, MAX_DURATION);
        return EXIT_FAILURE;
    }

    /* push to array */
    strncpy(ptr->name, input, comma);
    *(ptr->name + comma) = '\0';
    ptr->distance = dist;
    ptr->duration = dur;
    return EXIT_SUCCESS;
}

/* this function gets the next empty position in the array
 * and takes from the user the data, if is valid pushing to array  */
int input_bus_line(BusLine *ptr) {
    char input[MAX_INPUT] = {0};
    fprintf(stdout, ENTER2);
    if (fgets(input, MAX_INPUT, stdin)) {
        return valid_numbers(input, (int) strlen(input) - 1, ptr);
    } else {
        return EXIT_FAILURE;
    }
}

/* This function printing the array by the order */
void print_lines(const BusLine *start, int args) {
    for (int i = 0; i < args; i++) {
        fprintf(stdout, "%s,%d,%d\n", (start + i)->name,
                (start + i)->distance, (start + i)->duration);
    }
}

/* This function checks validation of the given arguments returns the act */
int valid_arg(char *arg) {
    if (strcmp(arg, SORT_DISTANCE) == 0) {
        return TEST1;
    }
    if (strcmp(arg, SORT_DURATION) == 0) {
        return TEST3;
    }
    if (strcmp(arg, SORT_NAME) == 0) {
        return TEST5;
    }
    if (strcmp(arg, TEST) == 0) {
        return BASE;
    }
    return 0;
}

/* This function gets start of array, and type of test and test it */
void run_test(BusLine *start, int args, int test_num) {
    BusLine *ptr = malloc(sizeof(BusLine) * args);
    if (ptr) {
        /* make copy of array */
        BusLine *end = ptr + (args - 1);
        memcpy(ptr, start, sizeof(BusLine) * args);

        if (test_num == TEST1) {
            /* sorting the new array by distance and check it */
            quick_sort(ptr, end, DISTANCE);
            int sort = is_sorted_by_distance(ptr, end);
            fprintf(stdout, TEST_MESSAGE,
                    test_num, sort ? PASSED : FAILED,
                    sort ? TESTOK1 : TESTERR1);

        } else if (test_num == TEST3) {
            /* sorting the new array by duration and check it */
            quick_sort(ptr, end, DURATION);
            int sort = is_sorted_by_duration(ptr, end);
            fprintf(stdout, TEST_MESSAGE,
                    test_num, sort ? PASSED : FAILED,
                    sort ? TESTOK3 : TESTERR3);

        } else if (test_num == TEST5) {
            /* sorting the new array by name and check it */
            bubble_sort(ptr, end);
            int sort = is_sorted_by_name(ptr, end);
            fprintf(stdout, TEST_MESSAGE,
                    test_num, sort ? PASSED : FAILED,
                    sort ? TESTOK5 : TESTERR5);
        }

        int equal = is_equal(ptr, end, start, start + (args - 1));
        fprintf(stdout, TEST_MESSAGE,
                test_num + 1, equal ? PASSED : FAILED,
                equal ? TESTOKEQ : TESTERR_EQUAL);
    }
    free(ptr);
}

/* This function check if the given arguments has one of the valid
 * act and return the act, else returns 0*/
int check_args(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stdout, ARGS_ERROR);
        return 0;
    }
    int act = valid_arg(*(argv + 1));
    if (!act) {
        fprintf(stdout, ARGS_ERROR);
        return 0;
    }
    return act;
}

/* this function takes from the user how many lines will be in */
int get_number_lines() {
    char input[MAX_INPUT] = {0};
    int num_lines = 0;
    do {
        fprintf(stdout, ENTER1);
        if (fgets(input, MAX_INPUT, stdin)) {
            /* checks if the given number is positive */
            if (is_all_numbers(input, (int) strlen(input) - 1)) {
                num_lines = strtol(input, NULL, BASE);
            }
            if (!num_lines) {
                fprintf(stdout, ERR_LINE);
            }
        }
    } while (!num_lines);
    return num_lines;
}


int main(int argc, char *argv[]) {
    int act = check_args(argc, argv);
    if (!act) {
        return EXIT_FAILURE;
    }
    int num_lines = get_number_lines();
    BusLine *ptr = malloc(sizeof(BusLine) * num_lines);
    if (ptr) {
        /* taking the lines information from the user */
        for (int i = 0; i < num_lines; i++) {
            while (input_bus_line(ptr + i)) {}
        }

        /* doing the given act */
        if (act == TEST1) {
            quick_sort(ptr, ptr + (num_lines - 1), DISTANCE);
            print_lines(ptr, num_lines);
        } else if (act == TEST3) {
            quick_sort(ptr, ptr + (num_lines - 1), DURATION);
            print_lines(ptr, num_lines);
        } else if (act == TEST5) {
            bubble_sort(ptr, ptr + (num_lines - 1));
            print_lines(ptr, num_lines);
        } else {
            /* run all the tests */
            run_test(ptr, num_lines, TEST1);
            run_test(ptr, num_lines, TEST3);
            run_test(ptr, num_lines, TEST5);
        }
    } else {
        free(ptr);
        return EXIT_FAILURE;
    }
    free(ptr);
    return EXIT_SUCCESS;
}