#pragma once


#include <ctype.h>
#include "../Headers/LexerGraph.h"
#include "../Headers/LLL_Node.h"
#include "../Headers/Tokeniztion.h"




LexerVertex *nextState(FILE **sourceCode, LexerGraph *DFA, HashTable *ErrorTable);

LLL_List *TokenizeCode(FILE *sourceCode, LexerGraph *DFA, HashTable *symbolTable, HashTable *ErrorTable);

STRING *str_init();

void str_append(STRING *s, char c);

void str_free(STRING *s);