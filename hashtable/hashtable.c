/* 
 * hashtable.c - c file for CS50 hashtable module
 *
 * A *hashtable* is a set of (key,item) pairs.  It acts just like a set, 
 * but is far more efficient for large collections.
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 * updated by Xia Zhou, July 2016
 * CS 50, Fall 2022
 * 
 * Aya Hajjeh, CS50, Fall22, Lab3, Oct 6
 */

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "memory.h"
#include "hash.h"

/**************** global types ****************/
typedef struct hashtableNode {
    char* key;
    void* item;
} hashtableNode_t;

typedef struct hashtable {
    int numSlots;
    hashtableNode_t* hashtable;
} hashtable_t;
/**************** functions ****************/

/**************** hashtable_new ****************/
/* Create a new (empty) hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
hashtable_t* hashtable_new(const int num_slots){
    if (num_slots <= 0) return NULL;
    hashtable_t* newHashtable = mem_malloc(sizeof(hashtable_t));
    if (newHashtable==NULL) return NULL;
    newHashtable->numSlots=num_slots;
    newHashtable->hashtable = mem_calloc(num_slots, sizeof(hashtableNode_t));
    if (newHashtable->hashtable == NULL) {
        mem_free(newHashtable);
        return NULL;
    }
    for (int i=0; i<num_slots; i++){
        newHashtable->hashtable[i].item = NULL;
        newHashtable->hashtable[i].key = NULL;
    }
    return newHashtable;
}

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   The key string is copied for use by the hashtable; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool hashtable_insert(hashtable_t* ht, const char* key, void* item){
    if (ht == NULL || key == NULL || item == NULL) return false;
    int index = hash_jenkins(key, ht->numSlots);
    if (ht->hashtable[index].key != NULL) return false;
    ht->hashtable[index].key = mem_malloc(sizeof(key));
    if (ht->hashtable[index].key == NULL) return false;
    strncpy(ht->hashtable[index].key, key, sizeof(key));
    ht->hashtable[index].item = item;
    return true;
}

/**************** hashtable_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the hashtable is unchanged by this operation.
 */
void* hashtable_find(hashtable_t* ht, const char* key){
    if (ht == NULL || key == NULL) return NULL;
    int index = hash_jenkins(key, ht->numSlots);
    //if key is not found, item is gonna be NULL anyway
    return ht->hashtable[index].item;
}

/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to hashtable, 
 *   FILE open for writing,
 *   itemprint that can print a single (key, item) pair.
 * We print:
 *   nothing, if NULL fp.
 *   "(null)" if NULL ht.
 *   one line per hash slot, with no items, if NULL itemprint.
 *   otherwise, one line per hash slot, listing (key,item) pairs in that slot.
 * Note:
 *   the hashtable and its contents are not changed by this function,
 */
void hashtable_print(hashtable_t* ht, FILE* fp, 
                     void (*itemprint)(FILE* fp, const char* key, void* item)){
    if (fp == NULL) return;
    if (ht == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }
    for (int i=0; i<ht->numSlots; i++){
        if (itemprint == NULL)
            fprintf(fp, "\n");
        else if (ht->hashtable[i].key == NULL)
            fprintf(fp, "\n");
        else{
            fprintf(fp, "(%s,", ht->hashtable[i].key);
            (*itemprint)(fp, ht->hashtable[i].key, ht->hashtable[i].item);
            fprintf(fp,")\n");
        }
    }
                     }

/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to hashtable, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which hashtable items are handled is undefined.
 *   the hashtable and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void hashtable_iterate(hashtable_t* ht, void* arg,
                       void (*itemfunc)(void* arg, const char* key, void* item) ){
    if (ht == NULL || itemfunc == NULL) return;
    for (int i=0; i<ht->numSlots; i++){
        if (ht->hashtable[i].key != NULL)
        (*itemfunc)(arg, ht->hashtable[i].key, ht->hashtable[i].item);
    }
                       }

/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item.
 *
 * Caller provides:
 *   valid hashtable pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if hashtable==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the hashtable itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in hashtable_insert.
 */
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) ){
    if (ht == NULL) return;
    for (int i=0; i<ht->numSlots; i++){
        mem_free(ht->hashtable[i].key);
        if (itemdelete != NULL)
            (*itemdelete)(ht->hashtable[i].item);
    }
    mem_free(ht->hashtable);
    mem_free(ht);
}

#endif // __HASHTABLE_H
