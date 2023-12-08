/* 
 * set.c - c file for CS50 set module
 *
 * A *set* maintains an unordered collection of (key,item) pairs;
 * any given key can only occur in the set once. It starts out empty 
 * and grows as the caller inserts new (key,item) pairs.  The caller 
 * can retrieve items by asking for their key, but cannot remove or 
 * update pairs.  Items are distinguished by their key.
 * 
 * Aya Hajjeh, CS50, Fall22, Lab3, Oct 6
 */

#ifndef __SET_H
#define __SET_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "set.h"
#include "memory.h"

/**************** global types ****************/
typedef struct setNode {
    char* key;
    void* item;
    setNode_t* nextNode;
} setNode_t;

typedef struct set   // opaque to users of the module
{
    setNode_t* head;
} set_t;

/**************** functions ****************/

/**************** set_new ****************/
/* Create a new (empty) set.
 * 
 * We return:
 *   pointer to a new set, or NULL if error.
 * We guarantee:
 *   The set is initialized empty.
 * Caller is responsible for:
 *   later calling set_delete.
 */
set_t* set_new(void){
    set_t* new = mem_malloc(sizeof(set_t));
    if (new == NULL) return NULL;
    new->head = NULL;
    return new;
}

setNode_t* findNode(setNode_t* node, char* key){
    if (node == NULL || key == NULL) return NULL;
    if (strcmp(node->key, key) == 0) return node;
    else return findNode(node->nextNode, key);
}

/**************** set_insert ****************/
/* Insert item, identified by a key (string), into the given set.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer, and pointer to item.
 * We return:
 *  false if key exists, any parameter is NULL, or error;
 *  true iff new item was inserted.
 * Caller is responsible for:
 *   later calling set_delete to free the memory used by key strings.
 * Notes:
 *   The key string is copied for use by the set; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool set_insert(set_t* set, const char* key, void* item){
    if (set == NULL || key == NULL || item == NULL) return NULL;
    setNode_t* node = findNode(set->head, key);
    if (node != NULL) return false;
    node = mem_malloc(sizeof(setNode_t));
    node->item = mem_malloc(sizeof(item));
    node->item = item;
    node->key = mem_malloc(sizeof(key));
    node->key = key;
    node->nextNode = set->head;
    set->head = node;
    return true;
}

/**************** set_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer.
 * We return:
 *   a pointer to the desired item, if found; 
 *   NULL if set is NULL, key is NULL, or key is not found.
 * Notes:
 *   The item is *not* removed from the set.
 *   Thus, the caller should *not* free the pointer that is returned.
 */
void* set_find(set_t* set, const char* key){
    if (set == NULL || key == NULL) return NULL;
    setNode_t* node = findNode(set, key);
    if (node == NULL) return NULL;
    return node->item;
}

/**************** set_print ****************/
/* Print the whole set; provide the output file and func to print each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   FILE open for writing,
 *   valid pointer to function that prints one item.
 * We print:
 *   nothing if NULL fp. Print (null) if NULL set.
 *   print a set with no items if NULL itemprint. 
 *  otherwise, 
 *   print a comma-separated list of items surrounded by {brackets}.
 * Notes:
 *   The set and its contents are not changed.
 *   The 'itemprint' function is responsible for printing (key,item).
 */
void set_print(set_t* set, FILE* fp, 
               void (*itemprint)(FILE* fp, const char* key, void* item) ){
    if (fp == NULL) return;
    if (set == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }
    if (itemprint == NULL) {
        fprintf(fp, "{}\n");
        return;
    }
    fprintf(fp, "{");
    for (setNode_t* node = set->head; node != NULL; node=node->nextNode){
        (*itemprint)(fp, node->key, node->item);
        fputc(',', fp);
    }
    fprintf(fp,")\n");
}

/**************** set_iterate ****************/
/* Iterate over the set, calling a function on each item.
 * 
 * Caller provides:
 *   valid set pointer,
 *   arbitrary argument (pointer) that is passed-through to itemfunc,
 *   valid pointer to function that handles one item.
 * We do:
 *   nothing, if set==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc on each item, with (arg, key, item).
 * Notes:
 *   the order in which set items are handled is undefined.
 *   the set and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void set_iterate(set_t* set, void* arg,
                 void (*itemfunc)(void* arg, const char* key, void* item) ){
    if (set == NULL || itemfunc == NULL) return;
        for (setNode_t* node = set->head; node != NULL; node=node->nextNode){
        (*itemfunc)(arg, node->key, node->item);
    }
}

/**************** set_delete ****************/
/* Delete set, calling a delete function on each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if set==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the set itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in set_insert.
 */
void set_delete(set_t* set, void (*itemdelete)(void* item)){
    if (set == NULL || itemdelete == NULL) return;
    setNode_t* temp;
    for (setNode_t* node = set->head;node != NULL;){
        temp=node->nextNode;
        mem_free(node->key);
        (*itemdelete)(node->item);
        mem_free(node);
        node=temp;
    }
    mem_free(set);
}

#endif // __SET_H
