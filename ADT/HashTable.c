
#include "../Headers/ADT_STRUCTS.h"
#include "../Headers/HashTable.h"

int hashFunction(int id, int size) {
    return (id < 0 ? -id : id) % size;
}
// djb2 by Dan Bernstein.
int str_hashcode(STRING *s){
    unsigned long hash = 5381;
    int c;

    char *temp = s->str;
    while ((c = *temp++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

HashTable* createHashTable(int size) {
    HashTable* ht = malloc(sizeof(HashTable));
    ht->size = size;
    ht->table = calloc(size, sizeof(void*));
    return ht;
}

void hashTableInsert(HashTable* ht, int id, void* data) {
    int idx = hashFunction(id, ht->size);
    HashNode* node = malloc(sizeof(HashNode));
    node->id = id;
    node->data = data;
    node->next = ht->table[idx];
    ht->table[idx] = node;
}

void* hashTableSearch(HashTable* ht, int id) {
    int idx = hashFunction(id, ht->size);
    HashNode* node = ht->table[idx];
    while (node) {
        if (node->id == id) return node->data;
        node = node->next;
    }
    return NULL;
}

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