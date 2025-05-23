#pragma once

#include "Token.h"
#include "Stack.h"
#include "LLL_Node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef enum {
    AST_NONE,
    AST_DATA_TYPE,
    AST_VAR,
    AST_CONST,
    AST_BIN_OP,
    AST_ASSIGN,
    AST_RETURN,
    AST_IF,
    AST_ELSE,
    AST_ELSIF,
    AST_UNTIL,
    AST_FUNC_CALL,
    AST_FUNC,
    AST_ARR_DECLRATION,
    AST_ARR_INIT_VALUES,
    AST_DECLRATION,
    AST_FUNC_PARAM,
    AST_SET_VAL,
    AST_SET_ARR_VAL,
    AST_BODY
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char *value;
    LLL_List *children; 
} ASTNode;

typedef ASTNode* (*BuilderASTFunc)(struct Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildNone(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildExpressionVarNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildRegularVarNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildExpressionConstNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *putParamsInFunc(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildFunctionParamsNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildFunction(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildRegularConstNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildDataTypeNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildDeclaration(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildSetValue(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildSetArrValue(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *putArrInitValues(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildArrDeclaration(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildOpNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildFuncCallNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *FinalizeExpression(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildAssignNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildReturnNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildIfNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildElsifNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildElseNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildUntilNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *buildBodyNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *popBodyNode(Token *token, Stack *astStack, LLL_List *lst);

ASTNode *attachToAST(ASTNode *root, ASTNode *child);

void printASTToFileRecursive(FILE *file, ASTNode *node, int level, int isLast, const char *prefix);

void printASTToFile(ASTNode *root, const char *filename);

void printAST(ASTNode *root);

void freeAST(ASTNode *node);
