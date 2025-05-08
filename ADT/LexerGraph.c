#include "ADT_STRUCTS.h"

static int globalVertexID = 0; 

typedef struct Edge{
    char symbol; // Symbol associated with the edge
    struct Vertex *dest; // Pointer to the destination vertex
    struct Edge *left, *right; // Left and right edges for the binary tree structure
}Edge;

typedef struct Vertex{
    int id; // Vertex ID
    Token* token; // Token associated with the vertex if the vertx is in accepting state
    State state; // State of the vertex
    Edge *edge; // Pointer to the edges of the vertex
}Vertex;

typedef struct Graph{
    Vertex *vertices; // Array of vertices
    Vertex *startVertex; // Pointer to the start
    int numVertices;
}Graph;


Vertex* createVertex(State state, Token* token, Graph* graph) {
    Vertex* v = malloc(sizeof(Vertex));
    v->id = globalVertexID++;
    v->state = state;
    v->token = token;
    v->edge = NULL;

    if(graph != NULL){
        graph->vertices = realloc(graph->vertices, sizeof(Vertex) * (graph->numVertices + 1));
        graph->vertices[graph->numVertices] = *v; // Copy the vertex into the graph's vertices array
        graph->numVertices++;
    }
    
    return v;
}

Edge* createEdge(char symbol, Vertex* dest) {
    Edge* e = malloc(sizeof(Edge));
    e->symbol = symbol;
    e->dest = dest;
    e->left = NULL;
    e->right = NULL;
    return e;
}

Graph* createGraph() {
    Graph* g = malloc(sizeof(Graph));
    g->vertices = NULL;
    g->startVertex = NULL;
    g->numVertices = 0;
    return g;
}

void addEdge(Vertex* v, char symbol, Vertex* dest) {
    //TODO: Implement the function to add an edge to the vertex
}

Vertex *findNext(Edge *e, char symbol) {
    if (e == NULL) {
        return NULL;
    }else if (symbol == e->symbol) {
        return e->dest;
    }else if (symbol < e->symbol) {
        return findNext(e->left, symbol);
    }else {
        return findNext(e->right, symbol);
    }
}
