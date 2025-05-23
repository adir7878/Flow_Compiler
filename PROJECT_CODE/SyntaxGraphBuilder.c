
#include "../Headers/SyntaxGraph.h"
#include "../Headers/SyntaxGraphBuilder.h"  
#include "SyntaxGraphBuilder.h"

SubGraph *globalIfGraph = NULL;
SubGraph *globalUntilGraph = NULL;
SubGraph *globalDeclarationGraph = NULL;
SubGraph *globalCodeBlockGraph = NULL;
SubGraph *globalElseGraph = NULL;
SubGraph *globalElsifGraph = NULL;
SubGraph *globalType = NULL;

SyntaxVertex *id = NULL;


void connect(SyntaxVertex *from, SyntaxVertex *to, TOKEN_CATEGORY category, STACK_ACTION action, BuilderASTFunc builderFunc){
    addSyntaxEdge(from, category, to, action, builderFunc);
}

SubGraph *addSubGraph(SyntaxGraph *g){
    SubGraph *subGraph = (SubGraph*)malloc(sizeof(SubGraph));
    subGraph->start = createSyntaxVertex(g);
    subGraph->accepting = createSyntaxVertex(g);
    return subGraph;
}

SubGraph *arrayAssignValues(SyntaxGraph *g){
    SubGraph *arrayAssignValues = addSubGraph(g);

    SyntaxVertex *comma;
    comma = createSyntaxVertex(g);

    // { ... }
    connect(arrayAssignValues->start, arrayAssignValues->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildRegularVarNode);
    connect(arrayAssignValues->start, arrayAssignValues->accepting, TOKEN_CAT_CONSTANT, NONE, buildRegularConstNode);

    connect(arrayAssignValues->accepting, comma, TOKEN_CAT_COMMA, NONE, buildNone);
    connect(comma, arrayAssignValues->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildRegularVarNode);
    connect(comma, arrayAssignValues->accepting, TOKEN_CAT_CONSTANT, NONE, buildRegularConstNode);

    return arrayAssignValues;
}

SubGraph *functionParams(SyntaxGraph *g){
    SubGraph *functionParams = addSubGraph(g);

    SyntaxVertex *comma, *type;
    type = createSyntaxVertex(g);
    comma = createSyntaxVertex(g);

    connect(functionParams->start, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE, putParamsInFunc);
    connect(functionParams->accepting, comma, TOKEN_CAT_COMMA, NONE, buildNone);
    connect(comma, functionParams->start, TOKEN_CAT_TYPE, NONE, buildNone);

    return functionParams;
}

SubGraph *functionParamsInput(SyntaxGraph *g){
    SubGraph *functionParams = addSubGraph(g);

    SyntaxVertex *comma;
    comma = createSyntaxVertex(g);

    connect(functionParams->start, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildRegularVarNode);
    connect(functionParams->start, functionParams->accepting, TOKEN_CAT_CONSTANT, NONE, buildRegularConstNode);
    connect(functionParams->accepting, comma, TOKEN_CAT_COMMA, NONE, buildNone);
    connect(comma, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildRegularVarNode);
    connect(comma, functionParams->accepting, TOKEN_CAT_CONSTANT, NONE, buildRegularConstNode);

    return functionParams;
}

SubGraph *expression(SyntaxGraph *g){
    SubGraph *exp = addSubGraph(g);
    SubGraph *callParams = functionParamsInput(g);

    SyntaxVertex *arithOp = createSyntaxVertex(g);
    arithOp->id = 1000;
    SyntaxVertex *logicOp = createSyntaxVertex(g);
    SyntaxVertex *rel = createSyntaxVertex(g);

    connect(exp->start, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(exp->start, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);

    connect(exp->start, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);
    connect(exp->accepting, exp->accepting, TOKEN_CAT_RPAREN, POP, buildNone);

    connect(exp->accepting, callParams->start, TOKEN_CAT_LPAREN, PUSH, buildFuncCallNode);
    connect(callParams->start, exp->accepting, TOKEN_CAT_RPAREN, POP, buildNone);
    connect(callParams->accepting, exp->accepting, TOKEN_CAT_RPAREN, POP, buildNone);

    connect(exp->accepting, arithOp, TOKEN_CAT_ARITHMETIC_OP, NONE, buildOpNode);
    connect(exp->accepting, logicOp, TOKEN_CAT_LOGICAL_OP, NONE, buildOpNode);
    connect(exp->accepting, rel, TOKEN_CAT_RELATIONAL_OP, NONE, buildOpNode);

    connect(arithOp, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(arithOp, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(arithOp, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);

    connect(logicOp, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(logicOp, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(logicOp, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);

    connect(rel, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(rel, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(rel, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);


    return exp;
}

SubGraph *condition(SyntaxGraph *g){
    SubGraph *condition = addSubGraph(g);

    SyntaxVertex *logic_op, *relational_op;
    logic_op = createSyntaxVertex(g);
    relational_op = createSyntaxVertex(g);

    connect(condition->start, relational_op , TOKEN_CAT_RELATIONAL_OP, NONE, buildOpNode);
    connect(relational_op, condition->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(relational_op, condition->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);

    connect(condition->accepting, logic_op, TOKEN_CAT_LOGICAL_OP, NONE, buildOpNode);
    connect(condition->accepting, relational_op, TOKEN_CAT_RELATIONAL_OP, NONE, buildOpNode);

    connect(logic_op, condition->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(logic_op, condition->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);

    connect(condition->accepting, relational_op, TOKEN_CAT_RELATIONAL_OP, NONE, buildOpNode);
    connect(relational_op, condition->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(relational_op, condition->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);

    return condition;
}

SubGraph *ifStatement(SyntaxGraph *g){
    SubGraph *ifStatement = addSubGraph(g);
    SubGraph *conditionGraph = condition(g);

    SyntaxVertex *LPAREN;

    LPAREN = createSyntaxVertex(g);
    ifStatement->accepting->state = Accepting;

    //start the if statement
    connect(ifStatement->start, LPAREN, TOKEN_CAT_LPAREN, PUSH, buildNone);
    
    //can start at id or const
    connect(LPAREN, conditionGraph->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(LPAREN, conditionGraph->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    //reach end
    connect(conditionGraph->accepting, ifStatement->accepting, TOKEN_CAT_RPAREN, POP, FinalizeExpression);
    connect(conditionGraph->start, ifStatement->accepting, TOKEN_CAT_RPAREN, POP, FinalizeExpression);


    return ifStatement;
}

SubGraph *functionCallStatement(SyntaxGraph *g){

    SubGraph *callStmt = addSubGraph(g);

    SubGraph *params = functionParamsInput(g);

    SyntaxVertex *rparen = createSyntaxVertex(g);

    connect(callStmt->start, params->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(callStmt->start, params->accepting, TOKEN_CAT_CONSTANT, NONE, buildNone);
    connect(callStmt->start, rparen, TOKEN_CAT_RPAREN, POP, buildNone);
    connect(params->accepting, rparen, TOKEN_CAT_RPAREN, POP, buildNone);
    connect(rparen, callStmt->accepting, TOKEN_CAT_SEMICOLON, NONE, buildNone);

    return callStmt;
}

SubGraph *returnStatement(SyntaxGraph *g){
    SubGraph *ret = addSubGraph(g);
    SubGraph *exp = expression(g);
    ret->accepting->state = Accepting;

    connect(ret->start, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(ret->start, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(ret->start, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);
    connect(exp->accepting, ret->accepting, TOKEN_CAT_SEMICOLON, NONE, FinalizeExpression);

    return ret;
}


SubGraph *elsifStatement(SyntaxGraph *g){
    SubGraph *elsifStatement = addSubGraph(g);
    SubGraph *conditionGraph = condition(g);

    SyntaxVertex *LPAREN, *RPAREN;

    LPAREN = createSyntaxVertex(g);
    RPAREN = createSyntaxVertex(g);
    elsifStatement->accepting->state = Accepting;

    //start the elsif statement
    connect(elsifStatement->start, LPAREN, TOKEN_CAT_LPAREN, PUSH, buildNone);
    
    //can start at id or const
    connect(LPAREN, conditionGraph->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(LPAREN, conditionGraph->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    //reach end
    connect(conditionGraph->accepting, elsifStatement->accepting, TOKEN_CAT_RPAREN, POP, FinalizeExpression);
    connect(conditionGraph->start, elsifStatement->accepting, TOKEN_CAT_RPAREN, POP, FinalizeExpression);

    return elsifStatement;
}

SubGraph *elseStatement(SyntaxGraph *g){
    SubGraph *elseStatement = addSubGraph(g);

    elseStatement->start = elseStatement->accepting;
    // from here all we need is in the main graph connect the braces to code block    

    return elseStatement;
}

SubGraph *untilStatement(SyntaxGraph *g){
    SubGraph *untilStatement = addSubGraph(g);
    SubGraph *conditionGraph = condition(g);

    SyntaxVertex *LPAREN;

    LPAREN = createSyntaxVertex(g);
    untilStatement->accepting->state = Accepting;

    //start the until statement
    connect(untilStatement->start, LPAREN, TOKEN_CAT_LPAREN, PUSH, buildNone);
    
    //can start at id or const
    connect(LPAREN, conditionGraph->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(LPAREN, conditionGraph->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    //reach end
    connect(conditionGraph->accepting, untilStatement->accepting, TOKEN_CAT_RPAREN, POP, FinalizeExpression);
    connect(conditionGraph->start, untilStatement->accepting, TOKEN_CAT_RPAREN, POP, FinalizeExpression);

    return untilStatement;
}

SubGraph *simpleTypeID(SyntaxGraph *g){
    SubGraph *simpleTypeID = addSubGraph(g);

    // type id
    connect(simpleTypeID->start, simpleTypeID->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildNone);

    return simpleTypeID;
}

SubGraph *simpleDeclaration(SyntaxGraph *g){
    SubGraph *simpleDecl = addSubGraph(g);
    simpleDecl->start = simpleDecl->accepting;
    // type id --> ;(accepting)
    return simpleDecl;
}


SubGraph *arrDeclaration(SyntaxGraph *g){
    SubGraph *arrDecl = addSubGraph(g);
    SubGraph *exp = expression(g);

    SyntaxVertex *LBRACK, *RBRACK, *value;

    LBRACK = createSyntaxVertex(g);
    value = createSyntaxVertex(g);
    
    connect(arrDecl->start, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);
    connect(arrDecl->start, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(arrDecl->start, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(exp->accepting, arrDecl->accepting, TOKEN_CAT_RBRACKET, POP, FinalizeExpression);
    
    return arrDecl;
}

SubGraph *simpleAssignment(SyntaxGraph *g){
    SubGraph *simpleAssignment = addSubGraph(g);

    //assign is start, end is semicolon
    connect(simpleAssignment->start, simpleAssignment->accepting, TOKEN_CAT_SEMICOLON, NONE, buildNone);
    connect(simpleAssignment->start, simpleAssignment->accepting, TOKEN_CAT_SEMICOLON, NONE, buildNone);

    return simpleAssignment;
}

SubGraph *codeBlock(SyntaxGraph *g){
    SubGraph *codeBlock = addSubGraph(g);
    codeBlock->accepting->state = Accepting;

    SubGraph *ifGraph = globalIfGraph;
    SubGraph *untilGraph = globalUntilGraph;

    SubGraph *callStatement = functionCallStatement(g);
    SubGraph *retStmt = returnStatement(g);

    SubGraph *typeTOidentifier = globalType;
    SubGraph *simpleDecl = simpleDeclaration(g);
    SubGraph *arrDecl = arrDeclaration(g);
    SubGraph *arrInitValues = arrayAssignValues(g);
    SyntaxVertex *assign = createSyntaxVertex(g);
    SyntaxVertex *arrAssign = createSyntaxVertex(g);

    // from identifier to assign or semicolon.
    connect(typeTOidentifier->accepting, simpleDecl->accepting, TOKEN_CAT_SEMICOLON, NONE, buildDeclaration); // reach ; means its regular decl 
    connect(typeTOidentifier->accepting, assign, TOKEN_CAT_ASSIGN, NONE, buildDeclaration); // reach -> after ident means its regular decl
    connect(typeTOidentifier->accepting, arrDecl->start, TOKEN_CAT_LBRACKET, PUSH, buildArrDeclaration);// reach [ means its arr decl
    connect(arrDecl->accepting, arrAssign, TOKEN_CAT_ASSIGN, NONE, buildNone);
    
    SyntaxVertex *expSemi = createSyntaxVertex(g);
    SubGraph *exp = expression(g);

    SyntaxVertex *initArrSemi = createSyntaxVertex(g);
    connect(arrAssign, arrInitValues->start, TOKEN_CAT_LBRACE, PUSH, putArrInitValues);
    connect(arrInitValues->accepting, initArrSemi, TOKEN_CAT_RBRACE, POP, buildNone);
    connect(initArrSemi, expSemi, TOKEN_CAT_SEMICOLON, NONE, buildNone);

    connect(arrDecl->accepting, expSemi, TOKEN_CAT_SEMICOLON, NONE, buildNone);

    arrAssign = createSyntaxVertex(g);// for setting value and not init
    arrDecl = arrDeclaration(g); // set new just in case 
    connect(id, assign, TOKEN_CAT_ASSIGN, NONE, buildSetValue);
    connect(id, arrDecl->start, TOKEN_CAT_LBRACKET, PUSH, buildSetArrValue);
    connect(arrDecl->accepting, arrDecl->start, TOKEN_CAT_RBRACKET, POP, buildNone);
    connect(arrDecl->accepting, arrAssign, TOKEN_CAT_ASSIGN, NONE, buildNone);
    connect(id, callStatement->start, TOKEN_CAT_LPAREN, PUSH, buildFuncCallNode);

    connect(assign, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);
    connect(assign, exp->accepting,TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(assign, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);
    connect(arrAssign, exp->start, TOKEN_CAT_LPAREN, PUSH, buildNone);
    connect(arrAssign, exp->accepting,TOKEN_CAT_IDENTIFIER, NONE, buildExpressionVarNode);
    connect(arrAssign, exp->accepting, TOKEN_CAT_CONSTANT, NONE, buildExpressionConstNode);

    connect(exp->accepting, expSemi, TOKEN_CAT_SEMICOLON, NONE, FinalizeExpression);

    // empty code block
    connect(codeBlock->start, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);

    //from block to anywhere
    connect(codeBlock->start, ifGraph->start, TOKEN_CAT_IF, NONE, buildIfNode);
    connect(codeBlock->start, untilGraph->start, TOKEN_CAT_UNTIL, NONE, buildUntilNode);
    connect(codeBlock->start, typeTOidentifier->start, TOKEN_CAT_TYPE, NONE, buildNone);
    connect(codeBlock->start, id, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(codeBlock->start, retStmt->start, TOKEN_CAT_RETURN, NONE, buildReturnNode);
    
    //from if to anywhere
    connect(ifGraph->accepting, ifGraph->start, TOKEN_CAT_IF, NONE, buildIfNode);
    connect(ifGraph->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE, buildUntilNode);
    connect(ifGraph->accepting, globalType->start, TOKEN_CAT_TYPE, NONE, buildNone);
    connect(ifGraph->accepting, id, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(ifGraph->accepting, retStmt->start, TOKEN_CAT_RETURN, NONE, buildReturnNode);

    // from until to anywhere
    connect(untilGraph->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE, buildUntilNode);
    connect(untilGraph->accepting, ifGraph->start, TOKEN_CAT_IF, NONE, buildIfNode);
    connect(untilGraph->accepting, globalType->start, TOKEN_CAT_TYPE, NONE, buildNone);
    connect(untilGraph->accepting, id, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(untilGraph->accepting, retStmt->start, TOKEN_CAT_RETURN, NONE, buildReturnNode);

    // from simpleDecl to anywhere
    connect(simpleDecl->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE, buildUntilNode);
    connect(simpleDecl->accepting, ifGraph->start, TOKEN_CAT_IF, NONE, buildIfNode);
    connect(simpleDecl->accepting, globalType->start, TOKEN_CAT_TYPE, NONE, buildNone);
    connect(simpleDecl->accepting, id, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(simpleDecl->accepting, retStmt->start, TOKEN_CAT_RETURN, NONE, buildReturnNode);

    // from simpleAssign to anywhere
    connect(expSemi, untilGraph->start, TOKEN_CAT_UNTIL, NONE, buildUntilNode);
    connect(expSemi, ifGraph->start, TOKEN_CAT_IF, NONE, buildIfNode);
    connect(expSemi, globalType->start, TOKEN_CAT_TYPE, NONE, buildNone);
    connect(expSemi, id, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(expSemi, retStmt->start, TOKEN_CAT_RETURN, NONE, buildReturnNode);

    // from call statement to anywhere
    connect(callStatement->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE, buildUntilNode);
    connect(callStatement->accepting, ifGraph->start, TOKEN_CAT_IF, NONE, buildIfNode);
    connect(callStatement->accepting, globalType->start, TOKEN_CAT_TYPE, NONE, buildNone);
    connect(callStatement->accepting, id, TOKEN_CAT_IDENTIFIER, NONE, buildNone);
    connect(callStatement->accepting, retStmt->start, TOKEN_CAT_RETURN, NONE, buildReturnNode);
    
    // from anywhere to block accepting
    connect(ifGraph->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    connect(ifGraph->accepting, globalElsifGraph->start, TOKEN_CAT_ELSIF, NONE, buildElsifNode);
    connect(globalElsifGraph->accepting, globalElsifGraph->start, TOKEN_CAT_ELSIF, NONE, buildElsifNode);
    connect(ifGraph->accepting, globalElseGraph->start, TOKEN_CAT_ELSE, NONE, buildElseNode);
    connect(globalElsifGraph->accepting, globalElseGraph->start, TOKEN_CAT_ELSE, NONE, buildElseNode);
    connect(globalElsifGraph->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    connect(globalElseGraph->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    connect(untilGraph->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);

    connect(simpleDecl->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    connect(expSemi, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    connect(callStatement->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    connect(retStmt->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);

    
    return codeBlock;
}

SubGraph *functionStatement(SyntaxGraph *g){
    SubGraph *functionStatement = addSubGraph(g);
    SubGraph *functionParamsGraph = functionParams(g);
    SubGraph *codeBlockGraph = codeBlock(g);

    functionStatement->accepting = codeBlockGraph->accepting;

    SyntaxVertex *LPAREN, *RPAREN, *type, *identifier;

    LPAREN = createSyntaxVertex(g);
    RPAREN = createSyntaxVertex(g);
    type = createSyntaxVertex(g);
    identifier = createSyntaxVertex(g);

    connect(functionStatement->start, type, TOKEN_CAT_TYPE, NONE, buildDataTypeNode);
    connect(type, identifier, TOKEN_CAT_IDENTIFIER, NONE, buildRegularVarNode);
    connect(identifier, LPAREN, TOKEN_CAT_LPAREN, PUSH, buildFunctionParamsNode);

    connect(LPAREN, functionParamsGraph->start, TOKEN_CAT_TYPE, NONE, buildNone); // params
    connect(functionParamsGraph->accepting, RPAREN, TOKEN_CAT_RPAREN, POP, buildNone);

    connect(LPAREN, RPAREN, TOKEN_CAT_RPAREN, POP, buildNone); // no params
    connect(RPAREN, codeBlockGraph->start, TOKEN_CAT_LBRACE, PUSH, buildBodyNode);
    
    return functionStatement;
}


SyntaxGraph *createPDA(){

    //init graph
    SyntaxGraph *g = createSyntaxGraph();
    g->startVertex = createSyntaxVertex(g);

    globalIfGraph = ifStatement(g);
    globalElsifGraph = elsifStatement(g);
    globalElseGraph = elseStatement(g);
    globalUntilGraph = untilStatement(g);
    globalType = simpleTypeID(g);
    id = createSyntaxVertex(g);
    globalType->start->id = 10000;


    SubGraph *codeIF = codeBlock(g);
    SubGraph *codeELSE = codeBlock(g);
    SubGraph *codeELSIF = codeBlock(g);
    SubGraph *codeUNTIL = codeBlock(g);


    connect(globalUntilGraph->accepting, codeUNTIL->start, TOKEN_CAT_LBRACE, PUSH, buildBodyNode);
    globalUntilGraph->accepting = codeUNTIL->accepting;

    connect(globalIfGraph->accepting, codeIF->start, TOKEN_CAT_LBRACE, PUSH, buildBodyNode);
    connect(codeIF->accepting, globalElseGraph->start, TOKEN_CAT_ELSE, NONE, buildElseNode);
    connect(codeIF->accepting, globalElsifGraph->start, TOKEN_CAT_ELSIF, NONE, buildElsifNode);
    connect(codeIF->accepting, globalIfGraph->accepting, TOKEN_CAT_RBRACE, POP, popBodyNode);
    globalIfGraph->accepting = codeIF->accepting;

    connect(globalElsifGraph->accepting, codeELSIF->start, TOKEN_CAT_LBRACE, PUSH, buildBodyNode);
    connect(codeELSIF->accepting, globalElsifGraph->start, TOKEN_CAT_ELSIF, NONE, buildElsifNode);
    connect(codeELSIF->accepting, globalElseGraph->start, TOKEN_CAT_ELSE, NONE, buildElseNode);
    globalElsifGraph->accepting = codeELSIF->accepting;

    connect(globalElseGraph->accepting, codeELSE->start, TOKEN_CAT_LBRACE, PUSH, buildBodyNode);
    globalElseGraph->accepting = codeELSE->accepting;

    


    SubGraph *functionStatementGraph = functionStatement(g);

    connect(g->startVertex, functionStatementGraph->start, TOKEN_CAT_FUNC, NONE, buildFunction);
    
    return g;
}

