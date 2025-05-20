
#include "../Headers/SyntaxValidation.h"
#include "../Headers/SyntaxGraph.h"
#include "../Headers/Stack.h"



Stack *stack = NULL;

StackActionFunc action[] = {action_none, action_push, action_pop};

BOOLEAN SyntaxValidation(LLL_List *tokens, SyntaxGraph *PDA) {
    SyntaxVertex *currentLocation = NULL;
    BOOLEAN isOK = TRUE;
    
    Stack_Init(&stack);

    while(tokens && isOK){
        currentLocation = nextSyntaxState(&tokens, PDA);
        if(!isEmptyStack(stack)){
            printf("Stack top: %d\n", *(TOKEN_CATEGORY*)Stack_Peek(stack));
            printf("Stack size: %d\n", stack->size);
            isOK = FALSE;
        }
        if(currentLocation->state == Accepting){
            //TODO: do something after test graph works
        }else{
            isOK = FALSE;
        }
    }

    Stack_Free(stack);
    return isOK;
}

SyntaxVertex *nextSyntaxState(LLL_List **tokens, SyntaxGraph *PDA) {
    
    SyntaxVertex *currentLocation = PDA->startVertex;
    SyntaxEdge *nextEdge = NULL;
    LLL_List *prev = *tokens;

    Token *t = (Token*)(*tokens)->data;
    nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);

    if(nextEdge != NULL){
        printf("\nEDGE FOUND FOR %s", t->lexeme);
        action[nextEdge->action](stack, t->type);
    }else{
        printf("\nNO EDGE FOUND FOR %s", t->lexeme);
    }

    *tokens = (*tokens)->next;
    
    while(nextEdge != NULL && *tokens != NULL){
        t = (Token*)(*tokens)->data;

        currentLocation = nextEdge->dest;
        nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);

        if(nextEdge != NULL){
            printf("\nEDGE FOUND FOR %s", t->lexeme);
            action[nextEdge->action](stack, t->type);
        }else{
            printf("\nNO EDGE FOUND FOR %s", t->lexeme);
        }

        // printf("Current token: %s\n", t->lexeme);
        
        prev = *tokens;
        *tokens = (*tokens)->next;
    }
    
    if(nextEdge != NULL){
        currentLocation = nextEdge->dest;
    }else{
        *tokens = prev;
    }
    
    return currentLocation;
}