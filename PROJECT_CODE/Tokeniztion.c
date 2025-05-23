

#include "../Headers/Tokeniztion.h"
#include "Tokeniztion.h"

#define SPACIAL_SYMBOLS "[({})];,=<>"

STRING *str;
HashTable *symTable;

LexerVertex *nextState(FILE **sourceCode, LexerGraph *DFA, HashTable *ErrorTable) {
    int symbol = 0;
    str = str_init();
    STRING *s = str;
    LexerVertex *current = DFA->startVertex;
    LexerEdge *nextEdge = NULL;
    BOOLEAN endLoop = FALSE;

    // Skip spaces 
    while ((symbol = fgetc(*sourceCode)) != EOF && isspace(symbol));
    
    nextEdge = findNext(current->edge, symbol);
    if (!nextEdge){
        endLoop = TRUE;
    }

    while (symbol != EOF && endLoop == FALSE) {
        if (!nextEdge){
            ungetc(symbol, *sourceCode); // go one back when no next edge such as ' '.
            endLoop = TRUE; // end loop
        }else{
            current = nextEdge->dest;
            str_append(str, symbol);
            symbol = fgetc(*sourceCode);
            nextEdge = findNext(current->edge, symbol);
        }
    }


    if(current->state == Trap){
        // printf("Error: %s\n", ((Error*)hashTableSearch(ErrorTable, current->id))->errorMessage);
        return NULL;
    }else{
        return current;
    }
}

LLL_List *TokenizeCode(FILE *sourceCode, LexerGraph *DFA, HashTable *symbolTable, HashTable *ErrorTable) {
    LLL_List *head = LLL_createNode(NULL),
             *tail = head;

    Token *token = NULL;
    LexerVertex *currentVertex = DFA->startVertex;

    symTable = createHashTable(50);

    while(!feof(sourceCode)){
        currentVertex = nextState(&sourceCode, DFA, ErrorTable);
        if(currentVertex != NULL){
            token = (Token*)hashTableSearch(symbolTable, currentVertex->tokenCode);

            if(token != NULL){
                if(token->code == TOKEN_IDENTIFIER || token->code == TOKEN_NUMBER){
                    STRING *s = str;
                    Token *t = hashTableSearch(symTable, str_hashcode(str));
                    
                    if(!t){
                        t = createToken(token->code, token->type, str->str);
                        hashTableInsert(symTable, str_hashcode(str), t);
                    }
                    token = t;
                }
                LLL_Add_Next(tail, token);
                tail = tail->next;
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

STRING* str_init() {
    STRING *s = (STRING*)malloc(sizeof(STRING)); 
    if (!s) return NULL;

    s->str = (char*)malloc(1 * sizeof(char)); 
    if (!s->str) {
        free(s);
        return NULL;
    }

    s->str[0] = '\0';
    s->size = 0;
    return s;
}

void str_append(STRING *s, char c) {
    if (!s || !s->str) {  
        fprintf(stderr, "Error: NULL string in str_append!\n");
        return;
    }

   
    char *new_str = (char*)realloc(s->str, (s->size + 2) * sizeof(char));
    if (!new_str) { 
        fprintf(stderr, "Error: realloc failed in str_append!\n");
        return;
    }

    s->str = new_str;
    s->str[s->size++] = c;
    s->str[s->size] = '\0';   
}

void str_free(STRING *s){
    free(s->str);
    free(s);
}

