#include "../Headers/SyntaxGraph.h"
#include "SyntaxGraph.h"


static int globalVertexID = 0;


void action_none(Stack *stack, TOKEN_CATEGORY category) {
    // do nothing
}
void action_push(Stack *stack, TOKEN_CATEGORY category) {
    Stack_Push(stack, &category);
}
void action_pop(Stack *stack, TOKEN_CATEGORY category) {
    TOKEN_CATEGORY *top = (TOKEN_CATEGORY*)Stack_Peek(stack);
    if(isEmptyStack(stack)){
        puts("\ntoo much close parenthesis\n");
        exit(1);
    }else if(*(TOKEN_CATEGORY*)Stack_Peek(stack) != (category)){
        puts("\nclose parenthesis unmatch error\n");
        exit(1);
    }else{
        Stack_Pop(stack);
    }
}


SyntaxVertex* createSyntaxVertex(SyntaxGraph *graph){
    SyntaxVertex *newVer = NULL;
    newVer = (SyntaxVertex*)malloc(sizeof(SyntaxVertex));
    newVer->edge = NULL;
    newVer->template = NULL;
    newVer->state = Trap;
    // newVer->isSubGraphStart = FALSE;
    newVer->id = globalVertexID++;

    // printf("\nvertex id add: %d\n", newVer->id);

    if(graph != NULL){
        SyntaxVertex **tmp = realloc(graph->vertices, sizeof (*graph->vertices) * (graph->numVertices + 1));

        graph->vertices = tmp;
        graph->vertices[graph->numVertices++] = newVer;
    }else{
        free(newVer);
    }

    return newVer;
}
SyntaxEdge* createSyntaxEdge(TOKEN_CATEGORY category, SyntaxVertex *dest){
    SyntaxEdge *newEdge = (SyntaxEdge*)malloc(sizeof(SyntaxEdge));
    newEdge->dest = dest;
    newEdge->type = category;
    newEdge->action = NONE;
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

void addSyntaxEdge(SyntaxVertex *curr, TOKEN_CATEGORY category, SyntaxVertex *dest, STACK_ACTION action){
    SyntaxEdge *newEdge = createSyntaxEdge(category, dest);
    newEdge->action = action;
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
        SyntaxVertex* vertex = vertcies[i]; 
        // printf("%d", vertex->edge->type);
        freeSyntaxEdges(vertex->edge);
    }
    free(g->vertices);
    free(g);
}

