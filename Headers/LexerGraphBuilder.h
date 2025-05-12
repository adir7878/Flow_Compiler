#pragma once

#include "../Headers/LexerGraph.h"
#include "../Headers/Token.h"

Vertex *createVertexWithIdenifierEdges(LexerGraph *DFA, char *lexeme);

void AddLexerConnectedComponent(Token *token, LexerGraph *DFA);

void initIdenifierSubgraph(LexerGraph *DFA);

LexerGraph *CreateDFA();
