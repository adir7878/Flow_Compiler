#include "../Headers/SyntaxGraph.h"


static int globalVertexID = 0; 


SyntaxVertex* createSyntaxVertex(SyntaxGraph *graph){
    SyntaxVertex *newVer = (SyntaxVertex*)malloc(sizeof(SyntaxVertex));
    newVer->edge = NULL;
    newVer->id = globalVertexID++;
    newVer->state = Trap;
    newVer->template = NULL;
    newVer->isSubGraphStart = FALSE;

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
    newEdge->type = category;
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
        insertSyntaxEdge(&(*edges)->right, newEdge);
    }else if((*edges)->type > newEdge->type){
        insertSyntaxEdge(&(*edges)->left, newEdge);
    }else{
        free(newEdge);
    }
}

void addSyntaxEdge(SyntaxVertex *curr, TOKEN_CATEGORY category, SyntaxVertex *dest){
    SyntaxEdge *newEdge = createSyntaxEdge(category, dest);
    insertSyntaxEdge(&(curr->edge), newEdge);
}

SyntaxEdge* SyntaxFindNextEdge(SyntaxEdge *edge, TOKEN_CATEGORY category){
    if(edge == NULL){
        return NULL;
    }else if(edge->type < category){
        return SyntaxFindNextEdge(edge->right, category);
    }else if(edge->type > category){
        return SyntaxFindNextEdge(edge->left, category);
    }else{
        return edge;
    }
}

void printSyntaxEdgesTypes(SyntaxEdge *e){
    if(e == NULL){
        //do nothing
    }else{
        printSyntaxEdgesTypes(e->left);
        printSyntaxEdgesTypes(e->right);
        printf("%d, ", e->type);
    }
}

void freeSyntaxEdges(SyntaxEdge *e) {
    if (!e) return;
    freeSyntaxEdges(e->left);
    freeSyntaxEdges(e->right);
    free(e);
}
void freeSyntaxGraph(SyntaxGraph *g){
    SyntaxVertex** vertcies = g->vertices;
    for (int i = 0; i < g->numVertices; i++) {
        printf("%d", (*vertcies)->edge->type);
        freeSyntaxEdges((*vertcies)->edge);
    }
    free(g->vertices);
    free(g);
}

