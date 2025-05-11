#pragma once

#include "../ADT/LexerGraph.c"
#include "../ADT/Token.c"

void AddLexerConnectedComponent(Token *token, Graph *DFA){
    Vertex *current = DFA->startVertex,
           *newVertex = NULL;
    Edge *nextPath = NULL;
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
    current->token = token;
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
    AddLexerConnectedComponent(createToken(TOKEN_IF, TOKEN_CAT_KEYWORD, "if"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_ELSE, TOKEN_CAT_KEYWORD, "else"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_ELSIF, TOKEN_CAT_KEYWORD, "elsif"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_UNTIL, TOKEN_CAT_KEYWORD, "until"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_FUNC, TOKEN_CAT_KEYWORD, "func"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_RETURN, TOKEN_CAT_KEYWORD, "return"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_BLANK, TOKEN_CAT_KEYWORD, "blank"), DFA);

    // Logical operators
    AddLexerConnectedComponent(createToken(TOKEN_AND, TOKEN_CAT_LOGICAL_OP, "AND"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_OR, TOKEN_CAT_LOGICAL_OP, "OR"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_NOT, TOKEN_CAT_LOGICAL_OP, "NOT"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_EQUAL, TOKEN_CAT_LOGICAL_OP, "=="), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_NOTEQUAL, TOKEN_CAT_LOGICAL_OP, "!="), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_GTE, TOKEN_CAT_LOGICAL_OP, ">="), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_LTE, TOKEN_CAT_LOGICAL_OP, "<="), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_GT, TOKEN_CAT_LOGICAL_OP, ">"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_LT, TOKEN_CAT_LOGICAL_OP, "<"), DFA);

    // Types
    AddLexerConnectedComponent(createToken(TOKEN_INT, TOKEN_CAT_TYPE, "int"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_SHORT, TOKEN_CAT_TYPE, "short"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_LONG, TOKEN_CAT_TYPE, "long"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_DOUBLE, TOKEN_CAT_TYPE, "double"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_FLOAT, TOKEN_CAT_TYPE, "float"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_CHAR, TOKEN_CAT_TYPE, "char"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_BYTE, TOKEN_CAT_TYPE, "byte"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_BOOLEAN, TOKEN_CAT_TYPE, "boolean"), DFA);

    // Arithmetic operators
    AddLexerConnectedComponent(createToken(TOKEN_ADD, TOKEN_CAT_ARITHMETIC_OP, "+"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_SUB, TOKEN_CAT_ARITHMETIC_OP, "-"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_MUL, TOKEN_CAT_ARITHMETIC_OP, "*"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_DIV, TOKEN_CAT_ARITHMETIC_OP, "/"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_MOD, TOKEN_CAT_ARITHMETIC_OP, "%"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_POW, TOKEN_CAT_ARITHMETIC_OP, "^"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_ROOT, TOKEN_CAT_ARITHMETIC_OP, "~"), DFA);

    // Assignment
    AddLexerConnectedComponent(createToken(TOKEN_ASSIGN, TOKEN_CAT_SPECIAL_SYMBOL, "->"), DFA);

    // Delimiters
    AddLexerConnectedComponent(createToken(TOKEN_SEMICOLON, TOKEN_CAT_SPECIAL_SYMBOL, ";"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_COMMA, TOKEN_CAT_SPECIAL_SYMBOL, ","), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_LBRACE, TOKEN_CAT_SPECIAL_SYMBOL, "{"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_RBRACE, TOKEN_CAT_SPECIAL_SYMBOL, "}"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_LBRACKET, TOKEN_CAT_SPECIAL_SYMBOL, "["), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_RBRACKET, TOKEN_CAT_SPECIAL_SYMBOL, "]"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_LPAREN, TOKEN_CAT_SPECIAL_SYMBOL, "("), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_RPAREN, TOKEN_CAT_SPECIAL_SYMBOL, ")"), DFA);

    // Boolean literals
    AddLexerConnectedComponent(createToken(TOKEN_TRUE, TOKEN_CAT_CONSTANT, "true"), DFA);
    AddLexerConnectedComponent(createToken(TOKEN_FALSE, TOKEN_CAT_CONSTANT, "false"), DFA);

    return DFA;
}
