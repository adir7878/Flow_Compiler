
#include "../Headers/LexerGraph.h"

static int globalVertexID = 0;

LexerVertex* createVertex(State state, TOKEN_CODE tokenCode, LexerGraph* graph) {
    LexerVertex* v = malloc(sizeof(LexerVertex));
    v->id = globalVertexID++;
    v->state = state;
    v->tokenCode = tokenCode;
    v->edge = NULL;

    if(graph != NULL){
        graph->vertices = realloc(graph->vertices, sizeof(LexerVertex) * (graph->numVertices + 1));
        graph->vertices[graph->numVertices] = *v; // Copy the vertex into the graph's vertices array
        graph->numVertices++;
    }
    
    return v;
}

LexerEdge* createEdge(char symbol, LexerVertex* dest) {
    LexerEdge* e = malloc(sizeof(LexerEdge));
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

void insertEdge(LexerEdge** root, LexerEdge* newEdge){
    if (!*root) {
        *root = newEdge;
    }else if (newEdge->symbol < (*root)->symbol) {
        insertEdge(&(*root)->left,  newEdge);
    }else if (newEdge->symbol > (*root)->symbol) {
        insertEdge(&(*root)->right, newEdge);    
    }else {
        // already exists, free the new edge
        free(newEdge);
    }
}

void addEdge(LexerVertex* v, char symbol, LexerVertex* dest) {
    LexerEdge *newEdge = createEdge(symbol, dest);
    insertEdge(&(v->edge), newEdge);
}


LexerEdge *findNext(LexerEdge *e, char symbol) {
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

void freeEdges(LexerEdge *e) {
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
