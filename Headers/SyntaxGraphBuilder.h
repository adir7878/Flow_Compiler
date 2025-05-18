#pragma once

#include "SyntaxGraph.h"

SyntaxGraph *createPDA();
SubGraph *buildExpression(SyntaxGraph *syntaxGraph);

SubGraph *arrayInitializationValues(SyntaxGraph *syntaxGraph);
