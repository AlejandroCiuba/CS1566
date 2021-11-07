/**
 * General use array list
 * @author Alejandro Ciuba
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stddef.h>

// The array list
typedef struct array_list {

    // An array of pointers to any data type
    void** array;
    int size;
    int capacity;
    size_t data_size;
} arl;

// ===================== FUNCTIONS =====================

// Initializes the array list
arl* init_arl(int init_capacity, size_t data_size);

// Add to an index, copies data into array
arl* append(const void* data, arl* ar);

// Replaces value
arl* replace(const void* data, int index, arl* ar);

// Removes at index, shifts array down
arl* delete(int index, arl* ar);

// Gets data at index, returning a deep copy, REMEMBER TO FREE!!!
void* get_deep(int index, arl* ar);

// Gets data at index, returning a shallow copy,
// REMEMBER: CHANGES MADE TO IT WILL BE REFLECTED IN THE ARRAY LIST
void* get_shallow(int index, arl* ar);

// Frees old ar and returns new ar twice the size
arl* upsize(arl* ar);

// Frees old ar and returns new ar twice as small
arl* downsize(arl* ar);

// Frees array list
void free_arl(arl* ar);

#endif