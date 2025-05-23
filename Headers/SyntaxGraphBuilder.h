#pragma once

#include "SyntaxGraph.h"
#include "AST.h"

typedef struct SubGraph{
    SyntaxVertex *start;
    SyntaxVertex *accepting;
}SubGraph;


void connect(SyntaxVertex *from, SyntaxVertex *to, TOKEN_CATEGORY category, STACK_ACTION action, BuilderASTFunc builderFunc);

SubGraph *addSubGraph(SyntaxGraph *syntaxGraph);

SubGraph *arrayAssignValues(SyntaxGraph *g);

SubGraph *functionParams(SyntaxGraph *g);

SubGraph *functionParamsInput(SyntaxGraph *g);

SubGraph *expression(SyntaxGraph *g);

SubGraph *condition(SyntaxGraph *g);

SubGraph *simpleDeclaration(SyntaxGraph *g);

SubGraph *simpleAssignment(SyntaxGraph *g);

SubGraph *codeBlock(SyntaxGraph *g);

SubGraph *ifStatement(SyntaxGraph *g);

SubGraph *functionCallStatement(SyntaxGraph *g);

SubGraph *elsifStatement(SyntaxGraph *g);

SubGraph *elseStatement(SyntaxGraph *g);

SubGraph *untilStatement(SyntaxGraph *g);

SubGraph *simpleTypeID(SyntaxGraph *g);

SubGraph *functionCodeBlock(SyntaxGraph *g);

SubGraph *functionStatement(SyntaxGraph *g);

SyntaxGraph *createPDA();
