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
} arl;

// ===================== FUNCTIONS =====================

// Initializes the array list
arl* init_arl(int init_capacity);

// Add to an index, copies data into array
arl* add(const void* data, arl* ar, size_t data_size);

// Replaces value
arl* replace(const void* data, int index, arl* ar, size_t data_size);

// Gets data at index, returning a deep copy, REMEMBER TO FREE!!!
void* get_deep(int index, arl* ar, size_t data_size);

// Gets data at index, returning a shallow copy,
// REMEMBER: CHANGES MADE TO IT WILL BE REFLECTED IN THE ARRAY LIST
void* get_shallow(int index, arl* ar);

// Frees old ar and returns new ar twice the size
arl* upsize(arl* ar, size_t data_size);

// Frees array list
void free_arl(arl* ar);

#endif