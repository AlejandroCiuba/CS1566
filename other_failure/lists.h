/**
 * Array List and Linked List for general use
 * @author Alejandro Ciuba
 */

#ifndef LISTS_H
#define LISTs_H

typedef enum bool{false, true};

//===================== LINKED LIST =====================
// Define the nodes for an array list
typedef struct node {
    void* data;
    node* next;
} node;

// Define the array list header struct
typedef struct linked_list {
    node* head;
    node* tail;
    int size;
} list;

// Various operations for a linked list
// Returns a linked list head with nothing
list* init_list();

// Appends node to the end of the linked list
void append(const void* n);

// Delete the first node with the data
void delete();

// Gets the first instance of a node with that data
bool contains(const void* data);

// Frees linked list
void free_list(list* head);

#endif