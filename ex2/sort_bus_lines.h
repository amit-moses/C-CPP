#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

#include <string.h>

#define NAME_LEN 21

typedef struct BusLine {
    char name[NAME_LEN];
    int distance, duration;
} BusLine;

typedef enum SortType {
    DISTANCE,
    DURATION
} SortType;

/**
 * This function sorting the array using bubble sort algorithm
 */
void bubble_sort(BusLine *start, BusLine *end);

/**
 * This function sorting the array using quick sort algorithm
 */
void quick_sort(BusLine *start, BusLine *end, SortType sort_type);

/**
 * This transfer the higher values to the right and smaller to left
 * return the place of the value which placed before in the right
 */
BusLine *partition(BusLine *start, BusLine *end, SortType sort_type);

#endif
