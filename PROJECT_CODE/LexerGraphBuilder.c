
#include "../Headers/LexerGraphBuilder.h"
#include "../Headers/Token.h"
#include "../Headers/HashTable.h"
// #include "../Headers/ADT_STRUCTS.h"

LexerVertex *createVertexWithIdenifierEdges(LexerGraph *DFA, char *lexeme){
    LexerVertex *newVertex = createVertex(Accepting, TOKEN_IDENTIFIER, DFA);

    if((*lexeme < 'a' && *lexeme > 'z') || (*lexeme > 'A' && *lexeme < 'Z')){
        return newVertex;
    }

    char c;
    for(c = 'a'; c <= 'z'; c++){
        addEdge(newVertex, c, DFA->identifierVertex);
    }
    for(c = 'A'; c <= 'Z'; c++){
        addEdge(newVertex, c, DFA->identifierVertex);
    }
    // printf("Adding edges to vertex %c\n", *lexeme);
    return newVertex;
}

void AddLexerConnectedComponent(Token* token, LexerGraph *DFA){
    LexerVertex *current = DFA->startVertex,
           *newVertex = NULL;
    LexerEdge *nextPath = NULL;
    char *lexeme = token->lexeme;
    printf("\nAdding connected component for token %s\n", token->lexeme);

    while (*(lexeme)){
        nextPath = findNext(current->edge, *lexeme);
        if (nextPath == NULL){
            newVertex = createVertexWithIdenifierEdges(DFA, lexeme);
            addEdge(current, *lexeme, newVertex);
            nextPath = findNext(current->edge, *lexeme);
            current = newVertex;
        }else if(nextPath->dest->state == Trap || nextPath->dest == DFA->identifierVertex){
            newVertex = createVertexWithIdenifierEdges(DFA, lexeme);
            nextPath->dest = newVertex;
            current = newVertex;
        }else{
            current = nextPath->dest;
        }
        lexeme++;
    }
    current->state = Accepting;
    current->tokenCode = token->code;
    printf("\nToken %s added successfully\n", token->lexeme);
}

void initIdenifierSubgraph(LexerGraph *DFA){
    DFA->identifierVertex = createVertex(Accepting, TOKEN_IDENTIFIER, DFA);
    LexerVertex *current = DFA->identifierVertex;
    LexerVertex *next = createVertex(Accepting, TOKEN_IDENTIFIER, DFA);
    char c;

    for(c = 'a'; c <= 'z'; c++){
        addEdge(DFA->startVertex, c, current);
        addEdge(current, c, current);
    }
    for(c = 'A'; c <= 'Z'; c++){
        addEdge(DFA->startVertex, c, current);
        addEdge(current, c, current);
    }
    for(c = '0'; c <= '9'; c++){
        addEdge(current, c, next);
        addEdge(next, c, next);
    }
}
void initNumbersSubgraph(LexerGraph *DFA){
    LexerVertex *start = DFA->startVertex;
    LexerVertex *numbersVertex = createVertex(Accepting, TOKEN_NUMBER, DFA);

    for(char c = '0'; c <= '9'; c++){
        addEdge(start, c, numbersVertex);
        addEdge(numbersVertex, c, numbersVertex);
    }
}


LexerGraph *CreateDFA(HashTable *symbolTable) {
    printf("init DFA...\n");
    
    LexerGraph *DFA = createGraph();
    DFA->startVertex = createVertex(Accepting, NO_TOKEN, DFA);

    initNumbersSubgraph(DFA);
    initIdenifierSubgraph(DFA);

    printf("init DFA done\n");
    printf("Adding connected components...\n");

    // Longest lexemes first (to preserve prefixes)
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_BOOLEAN), DFA);   // "boolean"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_RETURN), DFA);    // "return"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_DOUBLE), DFA);    // "double"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_ELSIF), DFA);     // "elsif"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_UNTIL), DFA);     // "until"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_BLANK), DFA);     // "blank"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_SHORT), DFA);     // "short"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_FLOAT), DFA);     // "float"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_FALSE), DFA);     // "false"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_ELSE), DFA);      // "else"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_FUNC), DFA);      // "func"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_LONG), DFA);      // "long"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_CHAR), DFA);      // "char"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_BYTE), DFA);      // "byte"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_TRUE), DFA);      // "true"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_AND), DFA);       // "and"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_NOT), DFA);       // "not"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_INT), DFA);       // "int"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_IF), DFA);        // "if"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_OR), DFA);        // "or"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_EQUAL), DFA);     // "=="
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_NOTEQUAL), DFA);  // "!="
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_GTE), DFA);       // ">="
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_LTE), DFA);       // "<="
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_ASSIGN), DFA);    // "->"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_GT), DFA);        // ">"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_LT), DFA);        // "<"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_ADD), DFA);       // "+"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_SUB), DFA);       // "-"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_MUL), DFA);       // "*"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_DIV), DFA);       // "/"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_MOD), DFA);       // "%"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_POW), DFA);       // "^"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_ROOT), DFA);      // "~"

    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_SEMICOLON), DFA); // ";"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_COMMA), DFA);     // ","
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_LBRACE), DFA);    // "{"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_RBRACE), DFA);    // "}"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_LBRACKET), DFA);  // "["
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_RBRACKET), DFA);  // "]"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_LPAREN), DFA);    // "("
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_RPAREN), DFA);    // ")"
    AddLexerConnectedComponent(hashTableSearch(symbolTable, TOKEN_MAIN), DFA);      // "main"

    printf("Connected components added successfully.\n");
    printf("DFA created successfully.\n");

    LexerEdge *nextPath = NULL;
    LexerVertex *current = DFA->startVertex;
    nextPath = findNext(current->edge, 'i');
    if(nextPath != NULL){
        printf("Found edge to vertex %c\n", nextPath->dest->state);
    }else{
        printf("No edge found\n");
    }

    return DFA;
}