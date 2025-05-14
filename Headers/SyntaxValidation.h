#pragma once

#include "ADT_STRUCTS.h"
#include "SyntaxGraph.h"
#include "LLL_Node.h"

BOOLEAN SyntaxValidation(LLL_List *tokens, SyntaxGraph *PDA);
SyntaxVertex *nextSyntaxState(LLL_List **tokens, SyntaxGraph *PDA);
