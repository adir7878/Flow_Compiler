#include "../Headers/ADT_STRUCTS.h"
#include "../Headers/LLL_Node.h"

LLL_List* LLL_createNode(void *data) {
    LLL_List *node = malloc(sizeof(LLL_List));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void LLL_SetNext(LLL_List *node, LLL_List *next) {
    if (node == NULL) {
        return;
    }
    LLL_List *temp = node->next;
    node->next = next;
    if (next != NULL) {
        next->prev = node;
    }
    if (temp != NULL) {
        freeLLL(temp);
    }
}

void LLL_Add_Next(LLL_List *node, void *data) {
    if (node == NULL) {
        return;
    }
    
    LLL_List *newNode = LLL_createNode(data);
    if (newNode == NULL) {
        return;
    }
    
    newNode->next = node->next;
    newNode->prev = node;
    
    if (node->next != NULL) {
        node->next->prev = newNode;
    }
    
    node->next = newNode;
}

void LLL_remove_next(LLL_List *node) {
    if (node == NULL || node->next == NULL) {
        return;
    }
    
    LLL_List *toRemove = node->next;
    node->next = toRemove->next;
    
    if (toRemove->next != NULL) {
        toRemove->next->prev = node;
    }
    
    free(toRemove);
}

void freeLLL(LLL_List *node) {
    LLL_List *current = node;
    while (current != NULL) {
        LLL_List *next = current->next;
        
        free(current);
        current = next;
    }
}
