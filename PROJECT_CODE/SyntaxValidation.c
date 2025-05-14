
#include "../Headers/SyntaxValidation.h"


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
    nextEdge = SyntaxFindNextEdge(currentLocation->edge ,((Token*)*tokens)->type);
    while(nextEdge != NULL && *tokens != NULL){
        currentLocation = nextEdge->dest;
        *tokens = (*tokens)->next;
        nextEdge = SyntaxFindNextEdge(currentLocation->edge ,((Token*)*tokens)->type);
    }
    
    return currentLocation;
}