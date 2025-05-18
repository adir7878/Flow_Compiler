#include "../Headers/ADT_STRUCTS.h"
#include "../Headers/Stack.h"
#include "Stack.h"

void Stack_Init(Stack **stack) {
    *stack = (Stack*)malloc(sizeof(Stack)); 
    (*stack)->data = malloc(sizeof(void*)); 
    (*stack)->top = -1; // Initialize the top index to -1 (empty stack)
    (*stack)->size = 0; // Set the size of the stack
}
void Stack_Push(Stack *stack, void *data) {
    stack->size++;
    stack->data = realloc(stack->data, sizeof(void*) * stack->size);
    stack->data[++stack->top] = data;
}
void *Stack_Pop(Stack *stack) {
    if (stack->top == -1) { // Check if the stack is empty
        printf("Stack underflow\n");
        return NULL;
    }
    // puts("\npoping\n");
    stack->size--;
    return stack->data[stack->top--]; // Pop the data from the stack
}
void *Stack_Peek(Stack *stack) {
    if (stack->top == -1) { // Check if the stack is empty
        printf("Stack is empty\n");
        return NULL;
    }
    return stack->data[stack->top]; // Return the top data without popping it
}
BOOLEAN isEmptyStack(Stack *stack) {
    return stack->top == -1; // Check if the stack is empty
}
void Stack_Free(Stack *stack) {
    free(stack->data); // Free the memory allocated for the stack
    stack->top = -1; // Reset the top index
    stack->size = 0; // Reset the size of the stack
}
