#include "ADT_STRUCTS.h"

typedef struct LLL_List{
    void *data;
    struct LLL_List *next;
}LLL_List;

void LLL_Add_Next(LLL_List *node, void *data);
LLL_List* LLL_createNode(void *data);
void LLL_SetNext(LLL_List *node, LLL_List *next);
void LLL_remove_next(LLL_List *node);
void freeLLL(LLL_List *node);


LLL_List* LLL_createNode(void *data) {
    LLL_List *node = malloc(sizeof(LLL_List));
    node->data = data;
    node->next = NULL;
    return node;
}

void LLL_SetNext(LLL_List *node, LLL_List *next) {
    LLL_List *temp = NULL;
    if (node != NULL) {
        temp = node->next;
        node->next = next;
    }
    freeLLL(temp);
}

void LLL_Add_Next(LLL_List *node, void *data) {
    //TODO: Implement the function to add a new node after the given node
}

void LLL_remove_next(LLL_List *node) {
    //TODO: Implement the function to remove the next node from the given node
}

void freeLLL(LLL_List *node) {
    //TODO: Implement the function to free the linked list
}