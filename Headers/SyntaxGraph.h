#pragma once
#include "ADT_STRUCTS.h"
#include "Template.h"

typedef struct SyntaxEdge{
    TOKEN_CATEGORY type;
    struct SyntaxVertex *dest;
    struct SyntaxEdge *left, *right;
}SyntaxEdge;

typedef struct SyntaxVertex{
    int id;
    State state;
    Template *template;
    SyntaxEdge *edge;
}SyntaxVertex;

typedef struct SubGraph{
    SyntaxVertex *start;
    SyntaxVertex *accepting;
}SubGraph;

typedef struct SyntaxGraph{
    SyntaxVertex **vertices;
    SyntaxVertex *startVertex;
    int numVertices;
}SyntaxGraph;



SyntaxVertex *createSyntaxVertex(SyntaxGraph *graph);
SyntaxGraph* createSyntaxGraph();
SyntaxEdge* createSyntaxEdge(TOKEN_CATEGORY category, SyntaxVertex *dest);
void insertSyntaxEdge(SyntaxEdge** edges, SyntaxEdge *newEdge);
void addSyntaxEdge(SyntaxVertex *curr, TOKEN_CATEGORY category, SyntaxVertex *dest);

void freeSyntaxEdges(SyntaxEdge *e);
void freeSyntaxGraph(SyntaxGraph *g);

