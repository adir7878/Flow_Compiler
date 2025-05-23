
#include "../Headers/SyntaxValidation.h"
#include "../Headers/SyntaxGraph.h"
#include "../Headers/Stack.h"



Stack *stack = NULL;

StackActionFunc action[] = {action_none, action_push, action_pop};

BOOLEAN SyntaxValidation(LLL_List *tokens, SyntaxGraph *PDA) {
    SyntaxVertex *currentLocation = NULL;
    BOOLEAN isOK = TRUE;

    ASTNode *Program = malloc(sizeof(ASTNode));
    Program->type = AST_BODY;
    Program->value = strdup("Program");
    Program->children = NULL;
    ASTNode *pattern = NULL;
    Stack_Init(&stack);

    while(tokens && isOK){
        pattern = nextSyntaxState(&tokens, PDA);
        if(!isEmptyStack(stack)){
            printf("\nStack top: %d", *(TOKEN_CATEGORY*)Stack_Peek(stack));
            printf("\nStack size: %d", stack->size);
            isOK = FALSE;
        }
        if(pattern != NULL){
            attachToAST(Program, pattern);
        }else{
            isOK = FALSE;
        }
    }
    puts("");
    printAST(Program); 
    Stack_Free(stack);
    // freeAST(Program);
    return isOK;
}

ASTNode *nextSyntaxState(LLL_List **tokens, SyntaxGraph *PDA) {
    
    SyntaxVertex *currentLocation = PDA->startVertex;
    SyntaxEdge *nextEdge = NULL;
    LLL_List *prev = *tokens;
    Stack *astStack;
    Stack_Init(&astStack);

    Token *t = (Token*)(*tokens)->data;
    nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);

    if(nextEdge != NULL){
        printf("\nEDGE FOUND FOR %s", t->lexeme);
        action[nextEdge->action](stack, t->type);
        nextEdge->builderFunc(t, astStack, tokens);
    }else{
        printf("\nNO EDGE FOUND FOR %s", t->lexeme);
    }

    *tokens = (*tokens)->next;
    
    while(nextEdge != NULL && *tokens != NULL){
        t = (Token*)(*tokens)->data;

        currentLocation = nextEdge->dest;
        nextEdge = SyntaxFindNextEdge(currentLocation->edge ,t->type);

        if(nextEdge != NULL){
            printf("\nEDGE FOUND FOR %s", t->lexeme);
            action[nextEdge->action](stack, t->type);
            ASTNode *curr = Stack_Peek(astStack);
            nextEdge->builderFunc(t, astStack, *tokens);
        }else{
            printf("\nNO EDGE FOUND FOR %s", t->lexeme);
        }

        
        prev = *tokens;
        *tokens = (*tokens)->next;
    }
    ASTNode *root;
    while(!isEmptyStack(astStack)){
        root = Stack_Pop(astStack);
    }

    
    if(nextEdge != NULL){
        currentLocation = nextEdge->dest;
    }else{
        *tokens = prev;
    }
    
    if(currentLocation->state == Accepting){
        return root;
    }else{
        return NULL;
    }
}