#include "../ADT/LexerGraph.c"
#include "../ADT/LLL_Node.c"


LLL_List *TokenizeCode(FILE *sourceCode, Graph *DFA){
    //TODO: Implement the function to tokenize the source code using the DFA
    LLL_List *head = LLL_createNode(NULL),
             *tail = head,
             *newNode = NULL;

    Token *token = NULL;
    Vertex *currentVertex = DFA->startVertex;

    while(!feof(sourceCode)){
        currentVertex = nextState(&sourceCode, DFA);
        if(currentVertex != NULL){
            token = currentVertex->token;
            newNode = LLL_createNode(token);
            tail->next = newNode;
            tail = newNode;
        }else{
            //TODO: Handle error case
        }
    }
    tail = head;
    head = head->next;
    free(tail);
    
    return head;
}

Vertex *nextState(FILE **current, Graph *DFA) {
    //TODO: Implement the function to find the next state in the DFA
    Vertex *currentVertex = DFA->startVertex;
    char symbol = fgetc(*current);
    Edge *nextEdge = findNext(currentVertex->edge, symbol);

    while(!feof(*current) && nextEdge != NULL){
        symbol = fgetc(*current);
        nextEdge = findNext(currentVertex->edge, symbol);
        currentVertex = nextEdge->dest;
    }

    if(currentVertex->state == Trap){
        //TODO: Handle the trap state
        // printf("ERROR CODE: %d; Error: %s\n", ,);
        return NULL;
    }else if(currentVertex->state == Intermediate){
        // printf("ERROR CODE: %d; Error: %s\n", ,);
        return NULL;
    }else{
        return currentVertex;
    }
    
} 