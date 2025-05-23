
typedef struct LLL_List{
    void *data;
    struct LLL_List *next;
    struct LLL_List *prev;
}LLL_List;

void LLL_Add_Next(LLL_List *node, void *data);
LLL_List* LLL_createNode(void *data);
void LLL_SetNext(LLL_List *node, LLL_List *next);
void LLL_remove_next(LLL_List *node);
void freeLLL(LLL_List *node);

#pragma once