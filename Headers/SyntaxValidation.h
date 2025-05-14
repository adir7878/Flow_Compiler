#pragma once

#include "ADT_STRUCTS.h"
#include "../ADT/SyntaxGraph.c"
#include "../ADT/LLL_Node.c"

BOOLEAN SyntaxValidation(LLL_List *tokens, SyntaxGraph *PDA);
SyntaxVertex *nextSyntaxState(LLL_List **tokens, SyntaxGraph *PDA);
