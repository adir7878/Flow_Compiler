
typedef struct HashNode {
    int id;
    void *data;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode **table;
    int size;
} HashTable;

HashTable* createHashTable(int size);
void hashTableInsert(HashTable *ht, int id, void *data);
void* hashTableSearch(HashTable *ht, int id);
void hashTableDelete(HashTable *ht, int id);
void freeHashTable(HashTable *ht);

#pragma once