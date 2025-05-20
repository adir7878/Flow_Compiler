#pragma once

#include "SyntaxGraph.h"

typedef struct SubGraph{
    SyntaxVertex *start;
    SyntaxVertex *accepting;
}SubGraph;


void connect(SyntaxVertex *from, SyntaxVertex *to, TOKEN_CATEGORY category, STACK_ACTION action);

SubGraph *addSubGraph(SyntaxGraph *syntaxGraph);

SubGraph *functionParams(SyntaxGraph *g);

SubGraph *functionParamsInput(SyntaxGraph *g);

SubGraph *condition(SyntaxGraph *g);

SubGraph *codeBlock(SyntaxGraph *g);

SubGraph *ifStatement(SyntaxGraph *g);

SubGraph *elsifStatement(SyntaxGraph *g);

SubGraph *elseStatement(SyntaxGraph *g);

SubGraph *untilStatement(SyntaxGraph *g);

SubGraph *functionStatement(SyntaxGraph *g);

SyntaxGraph *createPDA();
