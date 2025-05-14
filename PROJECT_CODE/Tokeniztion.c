

#include "../Headers/Tokeniztion.h"

#define SPACIAL_SYMBOLS "[({})];,=<>"


LexerVertex *nextState(FILE **sourceCode, LexerGraph *DFA, HashTable *ErrorTable) {
    int symbol = '\0';
    LexerVertex *current = DFA->startVertex;
    LexerEdge *nextEdge = NULL;
    BOOLEAN endLoop = FALSE;

    // Skip spaces 
    while ((symbol = fgetc(*sourceCode)) != EOF && isspace(symbol));
    
    nextEdge = findNext(current->edge, symbol);
    if (!nextEdge){
        endLoop = TRUE;
    }
//int x -> 123;( )  
    while (symbol != EOF && endLoop == FALSE) {
        if (!nextEdge){
            ungetc(symbol, *sourceCode); // go one back when no next edge such as ' '.
            endLoop = TRUE; // end loop
        }else{
            current = nextEdge->dest;
            symbol = fgetc(*sourceCode);
            nextEdge = findNext(current->edge, symbol);
        }
    }

    // printf("state: %d", current->state);

    if(current->state == Trap){
        // printf("Error: %s\n", ((Error*)hashTableSearch(ErrorTable, current->id))->errorMessage);
        return NULL;
    }else{
        return current;
    }
}

LLL_List *TokenizeCode(FILE *sourceCode, LexerGraph *DFA, HashTable *symbolTable, HashTable *ErrorTable) {
    LLL_List *head = LLL_createNode(NULL),
             *tail = head,
             *newNode = NULL;

    Token *token = NULL;
    LexerVertex *currentVertex = DFA->startVertex;

    while(!feof(sourceCode)){
        currentVertex = nextState(&sourceCode, DFA, ErrorTable);
        if(currentVertex != NULL){
            token = (Token*)hashTableSearch(symbolTable, currentVertex->tokenCode);
            
            if(token != NULL){
                // printf("token added: %s\n", token->lexeme);
                newNode = LLL_createNode(token);
                tail->next = newNode;
                tail = newNode;
            }
        }else{
            freeLLL(head);
            freeHashTable(symbolTable);
            freeHashTable(ErrorTable);
            freeLexerGraph(DFA);
            exit(1); // Exit the program when encounter an error
        }
    }
    tail = head;
    head = head->next;
    free(tail);
    
    return head;
}

