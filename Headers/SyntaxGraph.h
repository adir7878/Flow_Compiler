#pragma once
#include "ADT_STRUCTS.h"
#include "Template.h"
#include "Stack.h"

typedef enum{NONE, PUSH, POP} STACK_ACTION;
typedef void (*StackActionFunc)(Stack *stack, TOKEN_CATEGORY category);


typedef struct SyntaxEdge{
    TOKEN_CATEGORY type;
    STACK_ACTION action;
    struct SyntaxVertex *dest;
    struct SyntaxEdge *left, *right;
}SyntaxEdge;

typedef struct SyntaxVertex{
    int id;
    State state;
    Template *template;
    SyntaxEdge *edge;
    // BOOLEAN isSubGraphStart;
}SyntaxVertex;

typedef struct SyntaxGraph{
    SyntaxVertex **vertices;
    SyntaxVertex *startVertex;
    int numVertices;
}SyntaxGraph;

SyntaxVertex *createSyntaxVertex(SyntaxGraph *graph);
SyntaxGraph* createSyntaxGraph();
SyntaxEdge* createSyntaxEdge(TOKEN_CATEGORY category, SyntaxVertex *dest);
void insertSyntaxEdge(SyntaxEdge** edges, SyntaxEdge *newEdge);
void addSyntaxEdge(SyntaxVertex *curr, TOKEN_CATEGORY category, SyntaxVertex *dest, STACK_ACTION action);
SyntaxEdge *SyntaxFindNextEdge(SyntaxEdge *edge, TOKEN_CATEGORY category);

void printSyntaxEdgesTypes(SyntaxEdge *e);
void freeSyntaxEdges(SyntaxEdge *e);
void freeSyntaxGraph(SyntaxGraph *g);

void action_none(Stack *stack, TOKEN_CATEGORY category);
void action_push(Stack *stack, TOKEN_CATEGORY category);
void action_pop(Stack *stack, TOKEN_CATEGORY category);