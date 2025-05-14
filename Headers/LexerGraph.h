#pragma once
#include "ADT_STRUCTS.h"

typedef struct LexerEdge {
    char symbol;
    struct LexerVertex *dest;
    struct LexerEdge *left, *right;
} LexerEdge;

typedef struct LexerVertex {
    int id;
    TOKEN_CODE tokenCode;
    State state;
    struct LexerEdge *edge;
}LexerVertex;

typedef struct LexerGraph {
    struct LexerVertex *vertices;
    struct LexerVertex *startVertex;
    struct LexerVertex *identifierVertex;
    int numVertices;
} LexerGraph;

LexerVertex *createVertex(State state, TOKEN_CODE tokenCode, LexerGraph *graph);
LexerEdge *createEdge(char symbol, LexerVertex *dest);
LexerGraph *createGraph(void);
void insertEdge(LexerEdge **root, LexerEdge *newEdge);
void addEdge(LexerVertex *v, char symbol, LexerVertex *dest);
LexerEdge *findNext(LexerEdge *e, char symbol);
void freeEdges(LexerEdge *e);
void freeLexerGraph(LexerGraph *g);