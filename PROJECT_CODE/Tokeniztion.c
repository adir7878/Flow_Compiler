

#include "../Headers/Tokeniztion.h"


Vertex *nextState(FILE **sourceCode, LexerGraph *DFA, HashTable *ErrorTable) {
    int symbol = '\0';
    Vertex *current = DFA->startVertex;
    Edge *nextEdge = NULL;

    // Skip whitespace characters
    while ((symbol = fgetc(*sourceCode)) != EOF && isspace(symbol));
    // if (symbol == EOF) return NULL;

    // Check for end of line or end of file
    while (symbol != EOF && current->state != Trap) {
        nextEdge = findNext(current->edge, symbol);
        
        printf("Current state: %d, symbol: %c\n", current->state, symbol);
        
        if (!nextEdge){
            // printf("No edge found for symbol: %c\n", symbol);
            ungetc(symbol, *sourceCode);
            break;
        }
        current = nextEdge->dest;
        symbol = fgetc(*sourceCode);
    }

    if(current->state == Trap){
        // printf("Error: %s\n", ((Error*)hashTableSearch(ErrorTable, current->id))->errorMessage);
        return NULL;
    }else if(current->state == Intermediate){
        printf("Error: Unfinished Token\n");
        return NULL;
    }else{
        return current;
    }
}

LLL_List *TokenizeCode(FILE *sourceCode, LexerGraph *DFA, HashTable *symbolTable, HashTable *ErrorTable) {
    // Ensure the function is used to avoid unused-function warning
    printf("Tokenizing code...\n");

    LLL_List *head = LLL_createNode(NULL),
             *tail = head,
             *newNode = NULL;

    Token *token = NULL;
    Vertex *currentVertex = DFA->startVertex;

    while(!feof(sourceCode)){
        currentVertex = nextState(&sourceCode, DFA, ErrorTable);
        if(currentVertex != NULL){
            token = (Token*)hashTableSearch(symbolTable, currentVertex->tokenCode);
            if(token != NULL){
                // printf("\nToken found: %s\n", token->lexeme);
                newNode = LLL_createNode(token);
                tail->next = newNode;
                tail = newNode;
            }
        }else{
            //TODO: Handle error case, free all ADT
            freeLLL(head);
            freeHashTable(symbolTable);
            freeHashTable(ErrorTable);
            freeLexerGraph(DFA);
            exit(1);
        }
    }
    tail = head;
    head = head->next;
    free(tail);

    // No need anymore the lexer graph
    freeLexerGraph(DFA);
    
    return head;
}

