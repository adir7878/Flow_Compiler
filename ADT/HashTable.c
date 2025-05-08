#include "ADT_STRUCTS.h"

typedef struct HashTable{
    void **table; // Array of linked lists for the hash table
    int size; // Size of the hash table
}HashTable;

typedef struct HashNode{
    void *data; // Pointer to the data stored in the node
    struct HashNode *next; // Pointer to the next node in the linked list
    int id; // ID of the node
}HashNode;

