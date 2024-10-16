#include "sort_bus_lines.h"

/* This function replace between two given lines in array */
void swap_lines(BusLine *line1, BusLine *line2) {
    BusLine temp = *line1;
    *line1 = *line2;
    *line2 = temp;
}

/* This function sorting the array using bubble sort algorithm */
void bubble_sort(BusLine *start, BusLine *end) {
    for (BusLine *pos1 = start; pos1 < end; pos1++) {
        for (BusLine *pos2 = start; pos2 < end - (pos1 - start); pos2++) {
            /* transfer the high value to the end of array */
            if (0 < strcmp(pos2->name, (pos2 + 1)->name)) {
                swap_lines(pos2, pos2 + 1);
            }
        }
    }
}


/* This transfer the higher values to the right and smaller to left
 * return the place of the value which placed before in the right*/
BusLine *partition(BusLine *start, BusLine *end, SortType sort_type) {
    for (BusLine *right = start; right < end; right++) {
        /* compare between the given sort type act */
        if (sort_type ?
            right->duration <= end->duration :
            right->distance <= end->distance) {
            swap_lines(start, right);
            start++;
        }
    }
    swap_lines(end, start);
    return start;
}


/* This function sorting the array using quick sort algorithm */
void quick_sort(BusLine *start, BusLine *end, SortType sort_type) {
    if (start < end) {
        /* transfer the higher value to the right and smaller to left */
        BusLine *pivot = partition(start, end, sort_type);
        if (start < pivot) {
            /* make it to the left items */
            quick_sort(start, pivot - 1, sort_type);
        }
        if (pivot < end) {
            /* make it to the right items */
            quick_sort(pivot + 1, end, sort_type);
        }
    }
}