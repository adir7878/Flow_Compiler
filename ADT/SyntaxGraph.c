#include "../Headers/SyntaxGraph.h"
#include "SyntaxGraph.h"

static int globalVertexID = 0; 


SyntaxVertex* createSyntaxVertex(SyntaxGraph *graph){
    SyntaxVertex *newVer = (SyntaxVertex*)malloc(sizeof(SyntaxVertex));
    newVer->edge = NULL;
    newVer->id = globalVertexID++;
    newVer->state = Trap;
    newVer->template = NULL;

    if(graph != NULL){
        if(graph->vertices == NULL){
            graph->vertices = (SyntaxVertex**)realloc(graph->vertices, sizeof(SyntaxVertex*) * (graph->numVertices + 1));
        }else{
            graph->vertices =(SyntaxVertex**)malloc(sizeof(SyntaxVertex*));
        }
        
        graph->vertices[graph->numVertices] = newVer;
        graph->numVertices++;
    }

    return newVer;
}
SyntaxEdge* createSyntaxEdge(TOKEN_CATEGORY category, SyntaxVertex *dest){
    SyntaxEdge *newEdge = (SyntaxEdge*)malloc(sizeof(SyntaxEdge));
    newEdge->dest = dest;
    newEdge->left = NULL;
    newEdge->right = NULL;
    return newEdge;
}
SyntaxGraph* createSyntaxGraph(){
    SyntaxGraph *g = (SyntaxGraph*)malloc(sizeof(SyntaxGraph));
    g->numVertices = 0;
    g->vertices = NULL;
    g->startVertex = NULL;
    return g;
}

void insertSyntaxEdge(SyntaxEdge** edges, SyntaxEdge *newEdge){
    if(!*edges){
        *edges = newEdge;
    }else if((*edges)->type < newEdge->type){
        insertSyntaxEdge(&(*edges)->left, newEdge);
    }else if((*edges)->type > newEdge->type){
        insertSyntaxEdge(&(*edges)->right, newEdge);
    }else{
        free(newEdge);
    }
}

void addSyntaxEdge(SyntaxVertex *curr, TOKEN_CATEGORY category, SyntaxVertex *dest){
    SyntaxEdge *newEdge = createSyntaxEdge(category, dest);
    printf("created edge with: %d weight\n", category);
    insertSyntaxEdge(&(curr->edge), newEdge);
}

SyntaxEdge* SyntaxFindNextEdge(SyntaxEdge *edge, TOKEN_CATEGORY category){
    if(edge == NULL){
        return NULL;
    }else if(edge->type < category){
        SyntaxFindNextEdge(edge->right, category);
    }else if(edge->type > category){
        SyntaxFindNextEdge(edge->left, category);
    }
    return edge;
}

void freeSyntaxEdges(SyntaxEdge *e) {
    if (!e) return;
    freeSyntaxEdges(e->left);
    freeSyntaxEdges(e->right);
    free(e);
}
void freeSyntaxGraph(SyntaxGraph *g){
    for (int i = 0; i < g->numVertices; i++) {
        freeSyntaxEdges(g->vertices[i]->edge);
    }
    free(g->vertices);
    free(g);
}

