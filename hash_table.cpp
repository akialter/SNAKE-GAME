// ============================================
// The HashTable class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


/*
Student Name: Phat Bui
Date: 11/01/2022
=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
    e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
    e.g. "_HashTable", "_HashTableEntry"

    Recall that in C, we have to type "struct" together with the name of the struct
    in order to initialize a new variable. To avoid this, in hash_table.h
    we use typedef to provide new "nicknames" for "struct _HashTable" and
    "struct _HashTableEntry". As a result, we can create new struct variables
    by just using:
        - "HashTable myNewTable;"
    or
        - "HashTableEntry myNewHashTableEntry;"
    
    The preceding underscore "_" simply provides a distinction between the names
    of the actual struct defition and the "nicknames" that we use to initialize
    new structs.
    [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
    the first letter is lower-case.
    e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
        It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
    as a distinction between function local variables and struct members.
    e.g. "num_buckets" is a member of "HashTable".
*/


/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    // allocate space for a HashTableEntry to store key & value
    HashTableEntry *newEntry = (HashTableEntry *) malloc(sizeof(HashTableEntry));
    // store values of HashTableEntry and return pointer
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = NULL;
    return newEntry;
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    // hash the passed in key to get the index of the corresponding bucket
    unsigned int index = hashTable->hash(key);

    // create a HashTableEntry to iterate the bucket of the key, initially
    // pointed at the head of the bucket
    HashTableEntry *currEntry = hashTable->buckets[index];
    
    // iterate buckets[index]
    while (currEntry) {
        // if we found the HashTableEntry with matched key
        if (currEntry->key == key) {
            return currEntry;  // return the pointer to the entry
        }
        // if not, set to the next HashTableEntry
        currEntry = currEntry->next; 
    }

    return NULL; // entry matched with key not found
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
    // loop through each bucket of the hash table to remove all items.
    for (int i = 0; i < hashTable->num_buckets; i++) {
        // set currEntry to be the first HashTableEntry of buckets[i]
        HashTableEntry *currEntry = hashTable->buckets[i];
        
        // iterate the buckets[i]
        while(currEntry) {
            // save the next entry pointer
            HashTableEntry *temp = currEntry->next; 
            // free current entry & its value
            free(currEntry->value);
            free(currEntry);
            // set currEntry to next entry pointer
            currEntry = temp;
        }
    
        // set buckets[i] to null
        hashTable->buckets[i] = NULL;
    }

    // free buckets & hashTable
    free(hashTable->buckets);
    free(hashTable);
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    // check if there is a HashTableEntry corresponding to the key
    HashTableEntry *entry = findItem(hashTable, key);
 
    // if the key is present in the hash table, store new value and return old value
    if (entry) {
        // save old value
        void *oldValue = entry->value;
        // replace old value with new value in the entry
        entry->value = value;
        // return old value
        return oldValue;
    }

    // hash the passed in key to get the index of the corresponding bucket
    unsigned int index = hashTable->hash(key);

    // create HashTableEntry for new value
    HashTableEntry *newEntry = createHashTableEntry(key, value);
    // add to front of the buckets[index]
    newEntry->next = hashTable->buckets[index];
    hashTable->buckets[index] = newEntry;

    // return null
    return NULL;
}

void *getItem(HashTable *hashTable, unsigned int key)
{
    // check if the key is present in the hash table.
    HashTableEntry *entry = findItem(hashTable, key);

    // if the key is present in the hash table, return the entry's value
    // if not, return null
    return (entry) ? entry->value : NULL;
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
    // hash the passed in key to get the index of the corresponding bucket
    unsigned int index = hashTable->hash(key);
    // create a HashTableEntry to iterate the bucket of the key, initially
    // pointed at the head of the bucket
    HashTableEntry *currEntry = hashTable->buckets[index];

    // if buckets[index] is empty, return null
    if (!currEntry) {
        return NULL;
    }

    // if the head of buckets[index] is the entry to be removed
    if (currEntry->key == key) {
        // save the old value
        void* oldValue = currEntry->value;
        // update buckets[index]'s head
        hashTable->buckets[index] = currEntry->next;
        // free the entry
        free(currEntry);
        // return the old value
        return oldValue;
    }

    // iterate buckets[index]
    while (currEntry->next) {
        // if we found the HashTableEntry with matched key
        if (currEntry->next->key == key) {
            // save the old value
            void *oldValue = currEntry->next->value;
            // get the entry to be removed
            HashTableEntry *removedEntry = currEntry->next;
            // update currEntry->next and free removedEntry
            currEntry->next = currEntry->next->next;
            free(removedEntry);
            // return the old value
            return oldValue;
        }
        // if not, set to next HashTableEntry
        currEntry = currEntry->next;
    }

    // key is not present in the hashTable, return null
    return NULL;
}

void deleteItem(HashTable *hashTable, unsigned int key)
{
    // hash the passed in key to get the index of the corresponding bucket
    unsigned int index = hashTable->hash(key);
    // create a HashTableEntry to iterate the bucket of the key, initially
    // pointed at the head of the bucket
    HashTableEntry *currEntry = hashTable->buckets[index];

    // if buckets[index] is empty, return
    if (!currEntry) {
        return;
    }

    // if the head of buckets[index] is the entry to be deleted
    if (currEntry->key == key) {
        // update buckets[index]'s head
        hashTable->buckets[index] = currEntry->next;
        // free the entry and its value
        free(currEntry->value);
        free(currEntry);
        return;
    }

    // iterate buckets[index]
    while (currEntry->next) {
        // if we found the HashTableEntry with matched key
        if (currEntry->next->key == key) {
            // get the entry to be deleted
            HashTableEntry *deletedEntry = currEntry->next;
            // free the deleted entry's value
            free(currEntry->next->value);
            // update currEntry->next and free deletedEntry
            currEntry->next = currEntry->next->next;
            free(deletedEntry);
            return;
        }
        // if not, set to next HashTableEntry
        currEntry = currEntry->next;
    }
}
