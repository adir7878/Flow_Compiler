#pragma once


#include "LexerGraph.h"
#include "Token.h"

Vertex *createVertexWithIdenifierEdges(LexerGraph *DFA, char *lexeme);

void AddLexerConnectedComponent(Token *token, LexerGraph *DFA);

void initIdenifierSubgraph(LexerGraph *DFA);

LexerGraph *CreateDFA(HashTable *symbolTable);
