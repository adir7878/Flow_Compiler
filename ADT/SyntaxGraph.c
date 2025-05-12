#include "ADT_STRUCTS.h"

static int globalVertexID = 0; 

typedef struct Edge{
    Token token; // Symbol associated with the edge
    struct Vertex *dest; // Pointer to the destination vertex
    struct Edge *left, *right; // Left and right edges for the binary tree structure
}Edge;

typedef struct Vertex{
    int id; // Vertex ID
    State state; // State of the vertex
    // Template *template; // Template associated with the vertex if the vertex is in accepting state
    Edge *edge; // Pointer to the edges of the vertex
}Vertex;

typedef struct LexerGraph{
    Vertex *vertices; // Array of vertices
    Vertex *startVertex; // Pointer to the start
    int numVertices;
}LexerGraph;
