#pragma once

#include "../Headers/ADT_STRUCTS.h"
#include "../Headers/HashTable.h"

/* Compute a bucket index from an integer id */
static int hashFunction(int id, int size) {
    return (id < 0 ? -id : id) % size;
}

/* Allocate and initialize a new hash table of given size */
HashTable* createHashTable(int size) {
    HashTable* ht = malloc(sizeof(HashTable));
    ht->size = size;
    ht->table = calloc(size, sizeof(void*));
    return ht;
}

/* Insert a new data pointer under the given id */
void hashTableInsert(HashTable* ht, int id, void* data) {
    int idx = hashFunction(id, ht->size);
    HashNode* node = malloc(sizeof(HashNode));
    node->id = id;
    node->data = data;
    node->next = ht->table[idx];
    ht->table[idx] = node;
}

/* Look up the data pointer stored under the given id */
void* hashTableSearch(HashTable* ht, int id) {
    int idx = hashFunction(id, ht->size);
    HashNode* node = ht->table[idx];
    while (node) {
        if (node->id == id) return node->data;
        node = node->next;
    }
    return NULL;
}

/* Remove the entry with the given id from the table */
void hashTableDelete(HashTable* ht, int id) {
    int idx = hashFunction(id, ht->size);
    HashNode* node = ht->table[idx];
    HashNode* prev = NULL;
    while (node) {
        if (node->id == id) {
            if (prev) prev->next = node->next;
            else ht->table[idx] = node->next;
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

/* Free all nodes and the hash table structure itself */
void freeHashTable(HashTable* ht) {
    if(!ht) return;
    for (int i = 0; i < ht->size; ++i) {
        HashNode* node = ht->table[i];
        while (node) {
            HashNode* tmp = node->next;
            free(node);
            node = tmp;
        }
    }
    free(ht->table);
    free(ht);
}