
#include "../Headers/AST.h"
#include "AST.h"


ASTNode *addASTNode(ASTNodeType type, const char *name){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = strdup(name);
    node->children = NULL;
    return node;
}
ASTNode *buildNone(Token *token, Stack *astStack, LLL_List *lst) {
    return NULL;
}

ASTNode *buildExpressionVarNode(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_VAR;
    node->value = strdup(token->lexeme);
    node->children = NULL;

    Stack_Push(astStack, node);

    return node;
}

ASTNode *buildRegularVarNode(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = addASTNode(AST_VAR, token->lexeme);

      if (!isEmptyStack(astStack)) {
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, node);
    }

    return node;
}

ASTNode *buildExpressionConstNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_CONST;
    node->value = strdup(token->lexeme);
    node->children = NULL;
    
    Stack_Push(astStack, node);

    return node;
}

ASTNode *putParamsInFunc(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FUNC_PARAM;
    node->value = strdup("FUNCTION PARAMS");
    node->children = NULL;

    buildDataTypeNode((Token*)(lst->prev->data), astStack, lst);
    buildRegularVarNode((Token*)(lst->data), astStack, lst);

    return node;
}
ASTNode *buildFunctionParamsNode(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FUNC_PARAM;
    node->value = strdup("FUNCTION PARAMS");
    node->children = NULL;

    if(!isEmptyStack(astStack)){
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, node);
    }

    Stack_Push(astStack, node);
    return node;
}
ASTNode *buildFunction(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FUNC;
    node->value = strdup(token->lexeme);
    node->children = NULL;
 
    if(!isEmptyStack(astStack)){
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, node);
    }

    Stack_Push(astStack, node);
    return node;
}

ASTNode *buildRegularConstNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_CONST;
    node->value = strdup(token->lexeme);
    node->children = NULL;

    
    if (!isEmptyStack(astStack)) {
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, node);
    }

    return node;
}
ASTNode *buildDataTypeNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DATA_TYPE;
    node->value = strdup(token->lexeme);
    node->children = NULL;

    
    if (!isEmptyStack(astStack)) {
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, node);
    }

    return node;
}


ASTNode *putArrInitValues(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = addASTNode(AST_ARR_INIT_VALUES, "ARRAY INIT VALUES");
    
    ASTNode *parent = NULL;
    parent = Stack_Peek(astStack);
    
    attachToAST(parent, node);
    Stack_Push(astStack, node);
    
    return node;
}

ASTNode *buildArrDeclaration(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = addASTNode(AST_ARR_DECLRATION, "ARRAY DECLARATION");
    
    // Get the declaration parent
    ASTNode *parent = NULL;
    parent = Stack_Peek(astStack);

    Token *identifier, *type;
    identifier = (Token*)(lst->prev->data);
    type = (Token*)(lst->prev->prev->data);
    
    attachToAST(parent, node);
    Stack_Push(astStack, node);

    buildDataTypeNode(type, astStack, lst);
    buildRegularVarNode(identifier, astStack, lst);
    
    return node;
}
ASTNode *buildDeclaration(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = addASTNode(AST_DECLRATION, "DECLARATION");

    ASTNode *parent = NULL;
    while ((parent = Stack_Peek(astStack))->type != AST_BODY) {
        Stack_Pop(astStack);
    }

    Token *identifier, *type;
    identifier = (Token*)(lst->prev->data);
    type = (Token*)(lst->prev->prev->data);

    attachToAST(parent, node);
    Stack_Push(astStack, node);

    buildDataTypeNode(type, astStack, lst);
    buildRegularVarNode(identifier, astStack, lst);

    return node;
}
ASTNode *buildSetValue(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = addASTNode(AST_SET_VAL, "ASSIGNMENT");

    ASTNode *parent = NULL;
    while ((parent = Stack_Peek(astStack))->type != AST_BODY) {
        Stack_Pop(astStack);
    }

    Token *identifier;
    identifier = (Token*)(lst->prev->data);

    attachToAST(parent, node);
    Stack_Push(astStack, node);

    buildRegularVarNode(identifier, astStack, lst);

    return node;
}

ASTNode *buildSetArrValue(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = addASTNode(AST_SET_ARR_VAL, "ARR ASSIGNMENT");

    ASTNode *parent;
    while ((parent = Stack_Peek(astStack))->type != AST_BODY) {
        Stack_Pop(astStack);
    }

    Token *identifier;
    identifier = (Token*)(lst->prev->data);

    attachToAST(parent, node);
    Stack_Push(astStack, node);

    buildRegularVarNode(identifier, astStack, lst);

    return node;
}

ASTNode *buildOpNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BIN_OP;
    node->value = strdup(token->lexeme);
    node->children = NULL;

    Stack_Push(astStack, node);
    return node;
}

ASTNode *buildFuncCallNode(Token *token, Stack *astStack, LLL_List *lst){
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FUNC_CALL;
    node->value = strdup("FUNC_CALL");
    node->children = NULL;

    ASTNode *parent = Stack_Peek(astStack);
    while (parent->type != AST_ASSIGN && parent->type != AST_BIN_OP && parent->type != AST_BODY) {
        parent = Stack_Pop(astStack);
    }
    
    attachToAST(parent, node);
    // because the only way of find out that this is a function call is from the LPAREN so we need the one before
    Stack_Push(astStack, node);
    buildRegularVarNode((Token*)(lst->prev->data), astStack, lst);
    return node;
}

static int getPrecedence(ASTNode *n){
    if (!n || n->type != AST_BIN_OP)
        return 0;
    if (strcmp(n->value, "OR") == 0)
        return 1;
    if (strcmp(n->value, "AND") == 0)
        return 2;
    if (strcmp(n->value, "EQUAL") == 0 ||
        strcmp(n->value, "NOTEQUAL") == 0 ||
        strcmp(n->value, ">") == 0 ||
        strcmp(n->value, "<") == 0 ||
        strcmp(n->value, ">=") == 0 ||
        strcmp(n->value, "<=") == 0)
        return 3;
    if (strcmp(n->value, "+") == 0 || strcmp(n->value, "-") == 0)
        return 4;
    if (strcmp(n->value, "*") == 0 || strcmp(n->value, "/") == 0 ||
        strcmp(n->value, "%") == 0)
        return 5;
    if (strcmp(n->value, "~") == 0 || strcmp(n->value, "^") == 0)
        return 6;
    return 0;
}

ASTNode* FinalizeExpression(Token *token, Stack *astStack, LLL_List *lst) {

    LLL_List *exprList, *temp;
    exprList = LLL_createNode(NULL); // dummy head
    temp = exprList;

    ASTNode *top = Stack_Peek(astStack);
    while (!isEmptyStack(astStack) && top != NULL && (top->type == AST_BIN_OP || top->type == AST_VAR || top->type == AST_CONST)){
        top = Stack_Pop(astStack);
        LLL_Add_Next(temp, top);
        temp = temp->next;
        top = Stack_Peek(astStack);
    }

    temp = exprList;
    exprList = exprList->next; //remove dummy head
    free(temp);

    Stack *valStack = NULL;
    Stack *opStack  = NULL;
    Stack_Init(&valStack);
    Stack_Init(&opStack);

    for (LLL_List *it = exprList; it; it = it->next) {
        ASTNode *n = (ASTNode*)it->data;
        if (n->type == AST_CONST || n->type == AST_VAR) {
            Stack_Push(valStack, n);
        } else { // AST_BIN_OP
            while (!isEmptyStack(opStack) && getPrecedence((ASTNode*)Stack_Peek(opStack)) >= getPrecedence(n)){
                ASTNode *op = Stack_Pop(opStack);
                ASTNode *r  = Stack_Pop(valStack);
                ASTNode *l  = Stack_Pop(valStack);
                attachToAST(op, l);
                attachToAST(op, r);

                Stack_Push(valStack, op);
            }
            Stack_Push(opStack, n);
        }
    }

    while (!isEmptyStack(opStack)) {
        ASTNode *op = Stack_Pop(opStack);
        ASTNode *r  = Stack_Pop(valStack);
        ASTNode *l  = Stack_Pop(valStack);
        attachToAST(op, l);
        attachToAST(op, r);
        Stack_Push(valStack, op);
    }


    ASTNode *exprRoot = isEmptyStack(valStack) ? NULL : Stack_Pop(valStack);

    if (exprRoot){
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, exprRoot);
    }

    freeLLL(exprList);

    return exprRoot;
}

ASTNode *buildAssignNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGN;
    node->value = strdup(token->lexeme);
    node->children = NULL;

    if (!isEmptyStack(astStack)) {
        ASTNode *parent = Stack_Peek(astStack);
        attachToAST(parent, node);
    }

    Token *iden = NULL;
    while((iden = (Token*)(lst->prev->data))->type != TOKEN_CAT_IDENTIFIER){
        lst = lst->prev;
    }

    Stack_Push(astStack, node);
    buildRegularVarNode(iden, astStack, lst);
    

    return node;
}

ASTNode *buildReturnNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = addASTNode(AST_RETURN, "RETURN");
    
    // Find the body node
    ASTNode *parent = Stack_Pop(astStack);
    while (parent->type != AST_BODY) {
        parent = Stack_Pop(astStack);
    }
    
    attachToAST(parent, node);
    Stack_Push(astStack, node);
    
    return node;
}

ASTNode *buildIfNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_IF;
    node->value = strdup("IF");
    node->children = NULL;
    
    ASTNode *parent = NULL;
    while ((parent = Stack_Peek(astStack))->type != AST_BODY) {
        parent = Stack_Pop(astStack);
    }
    attachToAST(parent, node);
    Stack_Push(astStack, parent);
    Stack_Push(astStack, node);

    return node;
}

ASTNode *buildElsifNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ELSIF; 
    node->value = strdup("ELSIF");
    node->children = NULL;
    
    ASTNode *parent = NULL;
    while ((parent = Stack_Peek(astStack))->type != AST_IF) {
        parent = Stack_Pop(astStack);
    }
    attachToAST(parent, node);
    Stack_Push(astStack, parent);
    Stack_Push(astStack, node);

    return node;
}

ASTNode *buildElseNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ELSE; 
    node->value = strdup("ELSE");
    node->children = NULL;
    
    ASTNode *parent = Stack_Pop(astStack);
    if(parent->type == AST_ELSE){
        printf("\ncannot put else after else.");
        exit(20);
    }
    while (parent->type != AST_IF) {
        parent = Stack_Pop(astStack);
        if(parent->type == AST_ELSE){
            exit(20);
        }
    }
    attachToAST(parent, node);
    Stack_Push(astStack, parent);
    Stack_Push(astStack, node);

    return node;
}

ASTNode *buildUntilNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_UNTIL;
    node->value = strdup("UNTIL");
    node->children = NULL;

    ASTNode *parent = NULL;
    while ((parent = Stack_Peek(astStack))->type != AST_BODY) {
        parent = Stack_Pop(astStack);
    }
    attachToAST(parent, node);
    Stack_Push(astStack, parent);
    Stack_Push(astStack, node);

    return node;
}

ASTNode *buildBodyNode(Token *token, Stack *astStack, LLL_List *lst) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BODY;
    node->value = strdup("BODY");
    node->children = NULL;

    ASTNode *parent = NULL;
    while ((parent = Stack_Peek(astStack))->type != AST_UNTIL && parent->type != AST_IF && parent->type != AST_ELSIF && parent->type != AST_ELSE && parent->type != AST_FUNC) {
        Stack_Pop(astStack);
    }
    attachToAST(parent, node);
    Stack_Push(astStack, node);

    return node;
}
ASTNode *popBodyNode(Token *token, Stack *astStack, LLL_List *lst) {   
    ASTNode *top = Stack_Pop(astStack); //could be Assigment or anything
    if(!isEmptyStack(astStack) && (top = Stack_Peek(astStack))->type != AST_FUNC){ // if the function is empty we shouldnt pop the head of stack.
        top = Stack_Pop(astStack); // pop the body casue we end with this;
    }
    return top;
}

ASTNode* attachToAST(ASTNode *root, ASTNode *child){
    if(root == NULL){
        return child;
    }
     if(root->children == NULL){
        root->children = LLL_createNode(child);
    }else{
        LLL_List *temp = root->children;
        while(temp->next != NULL){
            temp = temp->next;
        }
        LLL_Add_Next(temp, child);
    }
    return child;
}

void printASTToFileRecursive(FILE *file, ASTNode *node, int level, int isLast, const char *prefix) {
    if (node == NULL || file == NULL) return;

    const char *val = node->value ? node->value : "NULL";
    const char *typeLabel = "";

    // הוספת תיוג לפי סוג ה-node
    switch (node->type) {
        case AST_VAR:       typeLabel = "VAR: "; break;
        case AST_CONST:   typeLabel = "LITERAL: "; break;
        case AST_BIN_OP:  typeLabel = "OPERATOR: "; break;
        case AST_FUNC_CALL: typeLabel = "FUNC_CALL: "; break;
        case AST_IF:        typeLabel = "IF: "; break;
        case AST_UNTIL:      typeLabel = "LOOP: "; break;
        case AST_RETURN:    typeLabel = "RETURN: "; break;
        // הוסף כאן עוד סוגי nodes לפי הצורך
        default:             typeLabel = ""; break;
    }

    fprintf(file, "%s", prefix);
    if (level > 0) {
        fprintf(file, "%s", isLast ? "└── " : "├── ");
    }

    fprintf(file, "[%d] %s%s\n", node->type, typeLabel, val);

    char newPrefix[256];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, (level > 0 ? (isLast ? "    " : "│   ") : ""));

    LLL_List *child = node->children;
    while (child != NULL) {
        ASTNode *childNode = (ASTNode *)child->data;
        int last = (child->next == NULL);
        printASTToFileRecursive(file, childNode, level + 1, last, newPrefix);
        child = child->next;
    }
}

void printASTToFile(ASTNode *root, const char *filename) {
    if (root == NULL || filename == NULL) return;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    printASTToFileRecursive(file, root, 0, 1, "");
    fclose(file);
}

void printAST(ASTNode *root) {
    printASTToFile(root, "C:\\Users\\adir7\\Flow_Compiler\\TestFiles\\AST_print,txt");
}

void freeAST(ASTNode *node) {
    if (!node) return;

    LLL_List *curr = node->children;
    while (curr) {
        LLL_List *next = curr->next;
        ASTNode  *child = (ASTNode*)curr->data;
        freeAST(child);  
        free(curr);
        curr = next;
    }

    if(node->value){
        free(node->value);
    }

    free(node);
}
