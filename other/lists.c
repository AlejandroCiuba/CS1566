/**
 * List functions for general use
 * @author Alejandro Ciuba
 */

#include "lists.h"
#include "stdlib.h"

//===================== LINKED LIST =====================
// Returns a linked list head with nothing
list* init_list() {

    list* llist = (list*) malloc(sizeof(list));
    if(llist == NULL) return -1;

    llist->head = NULL;
    llist->tail = NULL;
    llist->size = 0;
    return llist;
}

// Appends node to the end of the linked list
// All data should be malloced
void append(const void* data, list* llist) {

    if(data == NULL || llist == NULL) return;

    if(llist->size == 0) {
        llist->head = (node*) malloc(sizeof(node));
        if(llist->head == NULL) return;
        llist->head->data = data;
        llist->tail = llist->head;
        llist->head->next = NULL;
        llist->size++;
        return;
    }

    llist->tail->next = (node*) malloc(sizeof(node));
    if(llist->tail->next == NULL) return;
    llist->tail->next->data = data;
    llist->tail = llist->tail->next;
    llist->size++;
}

// Gets the first instance of a node with that data
bool contains(const void* data);

// Frees linked list
void free_list(list* head);