/**
 * Array list for general purposes
 * Running this on Valgrind generates
 * NO memory leaks, NO errors
 * @author Alejandro Ciuba
 */

#include "array_list.h"
#include <stdlib.h>
#include <string.h>

// ===================== FUNCTIONS =====================

// Initializes the array list
arl* init_arl(int init_capacity, size_t data_size) {

    arl* ar = (arl*) malloc(sizeof(arl));
    if(ar == NULL) return NULL;
    ar->array = (void**) malloc(sizeof(void*) * init_capacity);
    if(ar->array == NULL) return NULL;
    for(int i = 0; i < init_capacity; i++) ar->array[i] = NULL;
    ar->size = 0;
    ar->capacity = init_capacity;
    ar->data_size = data_size;
    return ar;
}

// Append to end, copies data into array
// Will replace data if already there
arl* append(const void* data, arl* ar) {

    if(data == NULL || ar == NULL) return ar;
    
    // Make a deep copy of the object
    void* data_cpy = malloc(ar->data_size);
    if(data_cpy == NULL) return ar;
    memcpy(data_cpy, data, ar->data_size);
    
    // If index out of bounds, upsize
    // Else insert
    if(ar->size == ar->capacity) {
        ar = upsize(ar);
        // Check if upsize failed
        if(ar->size == ar->capacity) return ar;
        ar->array[ar->size++] = data_cpy;
        return ar;
    }

    // Else insert
    ar->array[ar->size++] = data_cpy;
    return ar;
}

// Replaces value
arl* replace(const void* data, int index, arl* ar) {

    if(data == NULL || ar == NULL) return ar;
    if(index > ar->capacity) return ar;
    if(index > ar->size + 1) return ar;

    // Special Case: index is one greater than the size, simple append
    if(index == ar->size + 1) return append(data, ar);
    else ar->array[index] = memcpy(ar->array[index], data, ar->data_size);
    return ar;
}

// Removes at index, shifts array down
arl* delete(int index, arl* ar) {

    if(index < 0 || ar == NULL) return ar;
    if(index > ar->capacity) return ar;
    if(ar->array[index] == NULL) return ar;

    // Free stuff at that memory location
    free(ar->array[index]);

    // Shift
    for(int i = index; i < ar->size - 1; i++)
        ar->array[i] = ar->array[i + 1];

    // Decrement size and make last spot NULL
    ar->array[--(ar->size)] = NULL;
    
    // Special Case: We can downsize
    if(ar->size <= ar->capacity / 2) return downsize(ar);
    else return ar;
}

// Gets data at index, returning a deep copy, REMEMBER TO FREE!!!
void* get_deep(int index, arl* ar) {
    if(ar == NULL) return NULL;
    if(index > ar->size) return NULL;
    void* data_cpy = malloc(ar->data_size);
    if(data_cpy == NULL) return NULL;
    return (void*) memcpy(data_cpy, ar->array[index], ar->data_size);
}

// Gets data at index, returning a shallow copy,
// REMEMBER: CHANGES MADE TO IT WILL BE REFLECTED IN THE ARRAY LIST
void* get_shallow(int index, arl* ar) {
    if(ar == NULL) return NULL;
    if(index > ar->size) return NULL;
    return ar->array[index];
}

// Frees old ar and returns new ar twice the size
arl* upsize(arl* ar) {

    if(ar == NULL) return NULL;

    // Initialize new array list
    arl* new_ar = init_arl(ar->capacity * 2, ar->data_size);
    if(new_ar == NULL) return ar;

    // Copy all the data
    for(int i = 0; i < ar->size; i++)
        if((new_ar = append(ar->array[i], new_ar)) == NULL) return ar;
    
    // Free old array and return new_ar
    free_arl(ar);

    return new_ar;
}

// Frees old ar and returns new ar twice as small
arl* downsize(arl* ar) {

    if(ar == NULL) return NULL;

    // Initialize new array list
    arl* new_ar = init_arl(ar->capacity / 2, ar->data_size);
    if(new_ar == NULL) return ar;

    //Copy all the data
    for(int i = 0; i < new_ar->capacity; i++)
        if((new_ar = append(ar->array[i], new_ar)) == NULL) return ar;
    
    // Free old array and return new_ar
    free_arl(ar);
    return new_ar;
}

// Frees array list
void free_arl(arl* ar) {

    if(ar == NULL) return;
    else if(ar->array == NULL) {free(ar); return;}

    // Free everything in ar
    for(int i = 0; i < ar->size; i++)
        if(ar->array[i] != NULL)
            free(ar->array[i]);

    // Free the void pointer array
    free(ar->array);

    // Finally, free the arl pointer
    free(ar);
}