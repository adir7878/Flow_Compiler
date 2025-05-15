
#include "../Headers/SyntaxValidation.h"
#include "../Headers/SyntaxGraph.h"


BOOLEAN SyntaxValidation(LLL_List *tokens, SyntaxGraph *PDA) {
    SyntaxVertex *currentLocation = NULL;
    BOOLEAN isOK = TRUE;

    while(tokens && isOK){
        currentLocation = nextSyntaxState(&tokens, PDA);
        if(currentLocation->state == Accepting){
            //TODO: do something after test graph works
        }else{
            isOK = FALSE;
        }
    }
    return isOK;
}

SyntaxVertex *nextSyntaxState(LLL_List **tokens, SyntaxGraph *PDA) {
    
    SyntaxVertex *currentLocation = PDA->startVertex;
    SyntaxEdge *nextEdge = NULL;
    Token *t = (Token*)(*tokens)->data;
    nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);
    printf("\ncurrent token = %s, code: %d, Cat: %d\n", t->lexeme, t->code, t->type);
    printf("edge weight - %d\n",nextEdge->type);
    *tokens = (*tokens)->next;
    
    while(nextEdge != NULL && *tokens != NULL){
        t = (Token*)(*tokens)->data;
        
        currentLocation = nextEdge->dest;
        nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);
        printf("current token = %s, code: %d, Cat: %d\n", t->lexeme, t->code, t->type);
        printf("edge weight - %d\n",nextEdge->type);

        if(nextEdge->dest->isSubGraphStart){
            currentLocation = nextEdge->dest;
            nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);
            printf("current token = %s, code: %d, Cat: %d\n", t->lexeme, t->code, t->type);
            printf("edge weight - %d\n",nextEdge->type);
        }
        
        *tokens = (*tokens)->next;
    }
    if(nextEdge != NULL){
        currentLocation = nextEdge->dest;
    }
    
    return currentLocation;
}