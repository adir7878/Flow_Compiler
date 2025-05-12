#pragma once
#include "../ADT/ADT_STRUCTS.h"

typedef struct Edge {
    char symbol;
    struct Vertex *dest;
    struct Edge *left, *right;
} Edge;

typedef struct Vertex {
    int id;
    TOKEN_CODE *tokenCode;
    State state;
    struct Edge *edge;
}Vertex;

typedef struct LexerGraph {
    struct Vertex *vertices;
    struct Vertex *startVertex;
    struct Vertex *identifierVertex;
    int numVertices;
} LexerGraph;

// No definitions here, only declarations:
Vertex *createVertex(State state, TOKEN_CODE *tokenCode, LexerGraph *graph);
Edge *createEdge(char symbol, Vertex *dest);
LexerGraph *createGraph(void);
Edge *insertEdge(Edge *root, Edge *newEdge);
void addEdge(Vertex *v, char symbol, Vertex *dest);
Edge *findNext(Edge *e, char symbol);
void freeEdges(Edge *e);
void freeLexerGraph(LexerGraph *g);