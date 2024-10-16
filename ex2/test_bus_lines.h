#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"

/**
 * This function checks if the array sorted by distance
 */
int is_sorted_by_distance(const BusLine *start, const BusLine *end);

/**
 * This function checks if the array sorted by duration
 */
int is_sorted_by_duration(const BusLine *start, const BusLine *end);

/**
 * This function checks if the array sorted by name
 */
int is_sorted_by_name(const BusLine *start, const BusLine *end);

/**
 * This function gets two arrays and compare number of elements
 * and check if each element from the first array is also in the second
 */
int is_equal(const BusLine *start_sorted,
             const BusLine *end_sorted,
             const BusLine *start_original,
             const BusLine *end_original);

#endif
