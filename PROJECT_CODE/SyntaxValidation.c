
#include "../Headers/SyntaxValidation.h"
#include "../Headers/SyntaxGraph.h"
#include "../Headers/Stack.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

Stack *stack = NULL;

BOOLEAN SyntaxValidation(LLL_List *tokens, SyntaxGraph *PDA) {
    SyntaxVertex *currentLocation = NULL;
    BOOLEAN isOK = TRUE;
    
    Stack_Init(&stack);
    Stack *sWatch = stack;

    while(tokens && isOK){
        currentLocation = nextSyntaxState(&tokens, PDA);
        if(currentLocation->state == Accepting && isEmptyStack(stack)){
            //TODO: do something after test graph works
        }else{
            isOK = FALSE;
        }
    }

    Stack_Free(stack);
    return isOK;
}

BOOLEAN isNestingValid(Token *t){
    if(isEmptyStack(stack)){
        puts("Parentheses problem");
        exit(1); // handle error of pop from empty stack means parentheses problem
        return FALSE;
    }
    if(*(int*)Stack_Peek(stack) == TOKEN_LBRACE && t->code == TOKEN_RBRACE){
        return TRUE;
    }else if(*(int*)Stack_Peek(stack) == TOKEN_LPAREN && t->code == TOKEN_RPAREN){
        return TRUE;
    }else if(*(int*)Stack_Peek(stack) == TOKEN_LBRACKET && t->code == TOKEN_RBRACKET){
        return TRUE;
    }else{
        puts("Nesting error");
        return FALSE;
    }
}

void nestingHandler(Token* t){
    if(t->code == TOKEN_LBRACE || t->code == TOKEN_LPAREN || t->code == TOKEN_LBRACKET){
        // puts("pushing to stack");
        Stack_Push(stack, &t->code);
    }
    if(t->code == TOKEN_RBRACE || t->code == TOKEN_RPAREN || t->code == TOKEN_RBRACKET){
        isNestingValid(t) == TRUE ?  Stack_Pop(stack): 0;
    }
}

void skipSubGraphStartVertex(SyntaxVertex **currentLocation, SyntaxEdge **nextEdge, Token *t){
    if(*nextEdge != NULL && (*nextEdge)->dest->isSubGraphStart){
        //skip from the start vertrex to the path
        *currentLocation = (*nextEdge)->dest;
        *nextEdge = SyntaxFindNextEdge((*currentLocation)->edge ,t->type);
    }
}

SyntaxVertex *nextSyntaxState(LLL_List **tokens, SyntaxGraph *PDA) {
    
    SyntaxVertex *currentLocation = PDA->startVertex;
    SyntaxEdge *nextEdge = NULL;
    LLL_List *prev = *tokens;

    Token *t = (Token*)(*tokens)->data;
    nestingHandler(t);

    nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);
    skipSubGraphStartVertex(&currentLocation, &nextEdge, t);

    *tokens = (*tokens)->next;
    // printf("\ncurrent token = %s", t->lexeme);
    
    while(nextEdge != NULL && *tokens != NULL){
        t = (Token*)(*tokens)->data;
        nestingHandler(t);
        
        // printf("\ncurrent token = %s", t->lexeme);

        currentLocation = nextEdge->dest;
        nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);
        skipSubGraphStartVertex(&currentLocation, &nextEdge, t);
        
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