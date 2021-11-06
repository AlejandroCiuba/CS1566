/**
 * Array list for general purposes
 * @author Alejandro Ciuba
 */

#include "array_list.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// ===================== FUNCTIONS =====================

// Initializes the array list
arl* init_arl(int init_capacity) {

    arl* ar = (arl*) malloc(sizeof(arl));
    if(ar == NULL) return NULL;
    ar->array = (void**) malloc(sizeof(void*) * init_capacity);
    if(ar->array == NULL) return NULL;
    for(int i = 0; i < init_capacity; i++) ar->array[i] = NULL;
    ar->size = 0;
    ar->capacity = init_capacity;
    return ar;
}

// Add to end, copies data into array
// Will replace data if already there
arl* add(const void* data, arl* ar, size_t data_size) {

    if(data == NULL || ar == NULL) return ar;
    
    // Make a deep copy of the object
    void* data_cpy = malloc(sizeof(data_size));
    if(data_cpy == NULL) return ar;
    memcpy(data_cpy, data, data_size);

    // If index out of bounds, upsize
    // Else insert
    
    if(ar->size == ar->capacity) {
        ar = upsize(ar, data_size);
        ar->array[ar->size++] = data_cpy;
        return ar;
    }

    // Else insert
    ar->array[ar->size++] = data_cpy;
    return ar;
}

// Replaces value
arl* replace(const void* data, int index, arl* ar, size_t data_size) {

    if(data == NULL || ar == NULL) return ar;
    if(index > ar->size + 1) return ar;

    // Special Case: index is one greater than the size, simple add
    if(index == ar->size + 1) return add(data, ar, data_size);
    else ar->array[index] = memcpy(ar->array[index], data, data_size);
    return ar;
}

// Gets data at index, returning a deep copy, REMEMBER TO FREE!!!
void* get_deep(int index, arl* ar, size_t data_size) {
    if(ar == NULL) return NULL;
    if(index > ar->size) return NULL;
    return (void*) memcpy(malloc(sizeof(data_size)), ar->array[index], data_size);
}

// Gets data at index, returning a shallow copy,
// REMEMBER: CHANGES MADE TO IT WILL BE REFLECTED IN THE ARRAY LIST
void* get_shallow(int index, arl* ar) {
    if(ar == NULL) return NULL;
    if(index > ar->size) return NULL;
    return ar->array[index];
}

// Frees old ar and returns new ar twice the size
arl* upsize(arl* ar, size_t data_size) {

    if(ar == NULL) return NULL;

    // Initialize new array list
    arl* new_ar = init_arl(ar->capacity * 2);

    //Copy all the data
    for(int i = 0; i < ar->size; i++)
        new_ar = add(ar->array[i], new_ar, data_size);
    
    // Free old array and return new_ar
    free_arl(ar);

    return new_ar;
}

// Frees array list
void free_arl(arl* ar) {

    if(ar == NULL) return;
    else if(ar->array == NULL) return;

    // Free everything in ar
    for(int i = 0; i < ar->size; i++)
        if(ar->array[i] != NULL)
            free(ar->array[i]);

    // Free the void pointer array
    free(ar->array);

    // Finally, free the arl pointer
    free(ar);
}