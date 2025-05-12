#pragma once

#include "../Headers/LexerGraphBuilder.h"

Vertex *createVertexWithIdenifierEdges(LexerGraph *DFA, char *lexeme){
    Vertex *newVertex = createVertex(Intermediate, NULL, DFA);

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
}

void AddLexerConnectedComponent(Token* token, LexerGraph *DFA){
    Vertex *current = DFA->startVertex,
           *newVertex = NULL;
    Edge *nextPath = NULL;
    char *lexeme = token->lexeme;

    while (*(lexeme)){
        nextPath = findNext(current->edge, *lexeme);
        if (nextPath == NULL){
            newVertex = createVertexWithIdenifierEdges(DFA, lexeme);
            addEdge(current, *lexeme, newVertex);
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
}

void initIdenifierSubgraph(LexerGraph *DFA){
    DFA->identifierVertex = createVertex(Accepting, (TOKEN_CODE*)TOKEN_IDENTIFIER, DFA);
    Vertex *current = DFA->identifierVertex;
    Vertex *next = createVertex(Accepting, (TOKEN_CODE*)TOKEN_IDENTIFIER, DFA);
    char c;

    for(c = 'a'; c <= 'z'; c++){
        addEdge(current, c, current);
    }
    for(c = 'A'; c <= 'Z'; c++){
        addEdge(current, c, current);
    }
    for(c = '0'; c <= '9'; c++){
        addEdge(current, c, next);
        addEdge(next, c, next);
    }
}

/*
The CreateDFA function constructs and returns a deterministic finite automaton (DFA) represented as a LexerGraph object.
This DFA is initialized with a start vertex and populated with connected components for various token:
categories, including keywords, logical operators, types, arithmetic operators, assignment symbols, delimiters, and boolean literals.
with a placeholder for dynamic components like identifiers and numbers.
*/
LexerGraph *CreateDFA(){
    LexerGraph *DFA = createGraph();
    initIdenifierSubgraph(DFA);

    DFA->startVertex = createVertex(Intermediate, NULL, DFA);

    // Keywords
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_IF),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_ELSE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_ELSIF),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_UNTIL),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_FUNC),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_RETURN),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_BLANK),DFA);

    // Logical operators
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_AND),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_OR),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_NOT),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_EQUAL),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_NOTEQUAL),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_GTE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_LTE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_GT),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_LT),DFA);

    // Types
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_INT),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_SHORT),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_LONG),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_DOUBLE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_FLOAT),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_CHAR),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_BYTE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_BOOLEAN),DFA);

    // Arithmetic operators
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_ADD),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_SUB),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_MUL),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_DIV),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_MOD),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_POW),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_ROOT),DFA);

    // Assignment
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_ASSIGN),DFA);

    // Delimiters
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_SEMICOLON),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_COMMA),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_LBRACE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_RBRACE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_LBRACKET),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_RBRACKET),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_LPAREN),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_RPAREN),DFA);

    // Boolean literals
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_TRUE),DFA);
    AddLexerConnectedComponent(hashTableSearch(symbolTable,TOKEN_FALSE),DFA);


    return DFA;
}
