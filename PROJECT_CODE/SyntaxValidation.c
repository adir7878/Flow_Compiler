
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
    nextEdge = SyntaxFindNextEdge(currentLocation->edge ,((Token*)*tokens)->type);
    *tokens = (*tokens)->next;
    while(nextEdge != NULL && *tokens != NULL){
        currentLocation = nextEdge->dest;
        nextEdge = SyntaxFindNextEdge(currentLocation->edge ,((Token*)*tokens)->type);
        *tokens = (*tokens)->next;
    }
    
    return currentLocation;
}