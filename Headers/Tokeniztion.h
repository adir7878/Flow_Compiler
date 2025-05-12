#pragma once


#include <ctype.h>
#include "../Headers/LexerGraph.h"
#include "../Headers/LLL_Node.h"
#include "../Headers/Tokeniztion.h"

Vertex *nextState(FILE **sourceCode, LexerGraph *DFA, HashTable *ErrorTable);

LLL_List *TokenizeCode(FILE *sourceCode, LexerGraph *DFA, HashTable *symbolTable, HashTable *ErrorTable);


