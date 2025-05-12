
#include "../Headers/LexerGraph.h"

static int globalVertexID = 0;

Vertex* createVertex(State state, TOKEN_CODE* tokenCode, LexerGraph* graph) {
    Vertex* v = malloc(sizeof(Vertex));
    v->id = globalVertexID++;
    v->state = state;
    v->tokenCode = tokenCode;
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

LexerGraph* createGraph() {
    LexerGraph* g = malloc(sizeof(LexerGraph));
    g->vertices = NULL;
    g->startVertex = NULL;
    g->numVertices = 0;
    g->identifierVertex = NULL;
    return g;
}

Edge* insertEdge(Edge* root, Edge* newEdge){
    if (!root) {
        return newEdge;
    }
    if (newEdge->symbol < root->symbol) {
        root->left  = insertEdge(root->left,  newEdge);
    }else if (newEdge->symbol > root->symbol) {
        root->right = insertEdge(root->right, newEdge);
    }else {
        // already exists, free the new edge
        free(newEdge);
    }
    return root;
}

void addEdge(Vertex* v, char symbol, Vertex* dest) {
    Edge *newEdge = createEdge(symbol, dest);
    insertEdge(v->edge, newEdge);
}


Edge *findNext(Edge *e, char symbol) {
    if (e == NULL) {
        return NULL;
    }else if (symbol == e->symbol) {
        return e;
    }else if (symbol < e->symbol) {
        return findNext(e->left, symbol);
    }else {
        return findNext(e->right, symbol);
    }
}

void freeEdges(Edge *e) {
    if (!e) return;
    freeEdges(e->left);
    freeEdges(e->right);
    free(e);
}

void freeLexerGraph(LexerGraph *g) {
    for (int i = 0; i < g->numVertices; i++) {
        freeEdges(g->vertices[i].edge);
    }
    free(g->vertices);
    free(g);
}
