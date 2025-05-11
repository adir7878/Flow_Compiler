
#include "LexerGraphBuilder.c"


int main(){
    Graph *DFA = CreateDFA();
    printf("DFA created with %d vertices.\n", DFA->numVertices);
    free(DFA->vertices);
    free(DFA);
    return 0;
}