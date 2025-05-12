#pragma once

#include "../ADT/LexerGraph.c"
#include "../ADT/Token.c"

void AddLexerConnectedComponent(TOKEN_CODE* tokenCode, Graph *DFA){
    Vertex *current = DFA->startVertex,
           *newVertex = NULL;
    Edge *nextPath = NULL;
    Token *token = (Token*)hashTableSearch(symbolTable, *tokenCode);
    char *lexeme = token->lexeme;

    while (*(lexeme)){
        nextPath = findNext(current->edge, *lexeme);
        if (nextPath == NULL){
            newVertex = createVertex(Intermediate, NULL, DFA);
            addEdge(current, *lexeme, newVertex);
            current = newVertex;
        }else if(nextPath->dest->state == Trap){
            newVertex = createVertex(Intermediate, NULL, DFA);
            nextPath->dest = newVertex;
            current = newVertex;  
        }else{
            current = nextPath->dest;
        }
        lexeme++;
    }
    current->state = Accepting;
    current->tokenCode = tokenCode;
}

/*
The CreateDFA function constructs and returns a deterministic finite automaton (DFA) represented as a Graph object.
This DFA is initialized with a start vertex and populated with connected components for various token:
categories, including keywords, logical operators, types, arithmetic operators, assignment symbols, delimiters, and boolean literals.
with a placeholder for dynamic components like identifiers and numbers.
*/
Graph *CreateDFA(){
    Graph *DFA = createGraph();

    DFA->startVertex = createVertex(Intermediate, NULL, DFA);

    // Keywords
    AddLexerConnectedComponent(TOKEN_IF, DFA);
    AddLexerConnectedComponent(TOKEN_ELSE, DFA);
    AddLexerConnectedComponent(TOKEN_ELSIF, DFA);
    AddLexerConnectedComponent(TOKEN_UNTIL, DFA);
    AddLexerConnectedComponent(TOKEN_FUNC, DFA);
    AddLexerConnectedComponent(TOKEN_RETURN, DFA);
    AddLexerConnectedComponent(TOKEN_BLANK, DFA);

    // Logical operators
    AddLexerConnectedComponent(TOKEN_AND, DFA);
    AddLexerConnectedComponent(TOKEN_OR, DFA);
    AddLexerConnectedComponent(TOKEN_NOT, DFA);
    AddLexerConnectedComponent(TOKEN_EQUAL, DFA);
    AddLexerConnectedComponent(TOKEN_NOTEQUAL, DFA);
    AddLexerConnectedComponent(TOKEN_GTE, DFA);
    AddLexerConnectedComponent(TOKEN_LTE, DFA);
    AddLexerConnectedComponent(TOKEN_GT, DFA);
    AddLexerConnectedComponent(TOKEN_LT, DFA);

    // Types
    AddLexerConnectedComponent(TOKEN_INT, DFA);
    AddLexerConnectedComponent(TOKEN_SHORT, DFA);
    AddLexerConnectedComponent(TOKEN_LONG, DFA);
    AddLexerConnectedComponent(TOKEN_DOUBLE, DFA);
    AddLexerConnectedComponent(TOKEN_FLOAT, DFA);
    AddLexerConnectedComponent(TOKEN_CHAR, DFA);
    AddLexerConnectedComponent(TOKEN_BYTE, DFA);
    AddLexerConnectedComponent(TOKEN_BOOLEAN, DFA);

    // Arithmetic operators
    AddLexerConnectedComponent(TOKEN_ADD, DFA);
    AddLexerConnectedComponent(TOKEN_SUB, DFA);
    AddLexerConnectedComponent(TOKEN_MUL, DFA);
    AddLexerConnectedComponent(TOKEN_DIV, DFA);
    AddLexerConnectedComponent(TOKEN_MOD, DFA);
    AddLexerConnectedComponent(TOKEN_POW, DFA);
    AddLexerConnectedComponent(TOKEN_ROOT, DFA);

    // Assignment
    AddLexerConnectedComponent(TOKEN_ASSIGN, DFA);

    // Delimiters
    AddLexerConnectedComponent(TOKEN_SEMICOLON, DFA);
    AddLexerConnectedComponent(TOKEN_COMMA, DFA);
    AddLexerConnectedComponent(TOKEN_LBRACE, DFA);
    AddLexerConnectedComponent(TOKEN_RBRACE, DFA);
    AddLexerConnectedComponent(TOKEN_LBRACKET, DFA);
    AddLexerConnectedComponent(TOKEN_RBRACKET, DFA);
    AddLexerConnectedComponent(TOKEN_LPAREN, DFA);
    AddLexerConnectedComponent(TOKEN_RPAREN, DFA);

    // Boolean literals
    AddLexerConnectedComponent(TOKEN_TRUE, DFA);
    AddLexerConnectedComponent(TOKEN_FALSE, DFA);

    return DFA;
}
