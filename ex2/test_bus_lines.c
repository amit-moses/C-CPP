#include "test_bus_lines.h"

/* This function gets the array and check if specific item has the same
 * name like the given target*/
int search(const BusLine *start, const BusLine *end, const BusLine *target) {
    while (start <= end) {
        if (!strcmp(start->name, target->name)) {
            /* is found */
            return 1;
        }
        start++;
    }
    /* is not found */
    return 0;
}

/* This function gets two arrays and compare number of elements
 * and check if each element from the first array is also in the second*/
int is_equal(const BusLine *start_sorted,
             const BusLine *end_sorted,
             const BusLine *start_original,
             const BusLine *end_original) {

    /* check if the sizes is equal */
    if (end_sorted - start_sorted != end_original - start_original) {
        return 0;
    }

    /* check all elements in the original is also in the second array */
    while (start_original <= end_original) {
        if (!search(start_sorted, end_sorted, start_original)) {
            return 0;
        }
        start_original++;
    }
    return 1;
}

/* This function checks if the array sorted by distance */
int is_sorted_by_distance(const BusLine *start, const BusLine *end) {
    while (start < end) {
        /* compare between two adjacent elements and checks if ascending */
        if (start->distance > (start + 1)->distance) {
            return 0;
        }
        start++;
    }
    return 1;
}

/* This function checks if the array sorted by duration */
int is_sorted_by_duration(const BusLine *start, const BusLine *end) {
    while (start < end) {
        /* compare between two adjacent elements and checks if ascending */
        if (start->duration > (start + 1)->duration) {
            return 0;
        }
        start++;
    }
    return 1;
}

/* This function checks if the array sorted by name */
int is_sorted_by_name(const BusLine *start, const BusLine *end) {
    while (start < end) {
        /* compare between two adjacent elements and checks if ascending */
        if (0 <= strcmp(start->name, (start + 1)->name)) {
            return 0;
        }
        start++;
    }
    return 1;
}