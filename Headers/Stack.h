
typedef struct Stack{
    void **data; // Array of pointers to the data stored in the stack
    int top; // Index of the top element in the stack
    int size; // Size of the stack
}Stack;

void Stack_Init(Stack *stack, int size);

void Stack_Push(Stack *stack, void *data);

void *Stack_Pop(Stack *stack);

void *Stack_Peek(Stack *stack);

void Stack_Free(Stack *stack);

#pragma once
