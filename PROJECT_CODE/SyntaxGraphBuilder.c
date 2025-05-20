
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


void connect(SyntaxVertex *from, SyntaxVertex *to, TOKEN_CATEGORY category, STACK_ACTION action){
    addSyntaxEdge(from, category, to, action);
}

SubGraph *addSubGraph(SyntaxGraph *g){
    SubGraph *subGraph = (SubGraph*)malloc(sizeof(SubGraph));
    subGraph->start = createSyntaxVertex(g);
    subGraph->accepting = createSyntaxVertex(g);
    // subGraph->start->isSubGraphStart = TRUE;
    return subGraph;
}

SubGraph *arrayAssignValues(SyntaxGraph *g){
    SubGraph *arrayAssignValues = addSubGraph(g);

    SyntaxVertex *comma;
    comma = createSyntaxVertex(g);

    // { ... }
    connect(arrayAssignValues->start, arrayAssignValues->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(arrayAssignValues->start, arrayAssignValues->accepting, TOKEN_CAT_CONSTANT, NONE);

    connect(arrayAssignValues->accepting, comma, TOKEN_CAT_COMMA, NONE);
    connect(comma, arrayAssignValues->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(comma, arrayAssignValues->accepting, TOKEN_CAT_CONSTANT, NONE);

    return arrayAssignValues;
}

SubGraph *functionParams(SyntaxGraph *g){
    SubGraph *functionParams = addSubGraph(g);

    SyntaxVertex *comma, *type;
    type = createSyntaxVertex(g);
    comma = createSyntaxVertex(g);

    connect(functionParams->start, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(functionParams->accepting, comma, TOKEN_CAT_COMMA, NONE);
    connect(comma, type, TOKEN_CAT_TYPE, NONE);
    connect(type, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    return functionParams;
}

SubGraph *functionParamsInput(SyntaxGraph *g){
    SubGraph *functionParams = addSubGraph(g);

    SyntaxVertex *comma;
    comma = createSyntaxVertex(g);

    connect(functionParams->start, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(functionParams->accepting, comma, TOKEN_CAT_COMMA, NONE);
    connect(comma, functionParams->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    return functionParams;
}

SubGraph *expression(SyntaxGraph *g){
    SubGraph *exp = addSubGraph(g);
    SubGraph *callParams = functionParamsInput(g);

    SyntaxVertex *arithOp = createSyntaxVertex(g);
    arithOp->id = 1000;
    SyntaxVertex *logicOp = createSyntaxVertex(g);
    SyntaxVertex *rel = createSyntaxVertex(g);

    connect(exp->start, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(exp->start, exp->accepting, TOKEN_CAT_CONSTANT, NONE);

    connect(exp->start, exp->start, TOKEN_CAT_LPAREN, PUSH);
    connect(exp->accepting, exp->accepting, TOKEN_CAT_RPAREN, POP);

    connect(exp->accepting, callParams->start, TOKEN_CAT_LPAREN, PUSH);
    connect(callParams->start, exp->accepting, TOKEN_CAT_RPAREN, POP);
    connect(callParams->accepting, exp->accepting, TOKEN_CAT_RPAREN, POP);

    connect(exp->accepting, arithOp, TOKEN_CAT_ARITHMETIC_OP, NONE);
    connect(exp->accepting, logicOp, TOKEN_CAT_LOGICAL_OP, NONE);
    connect(exp->accepting, rel, TOKEN_CAT_RELATIONAL_OP, NONE);

    connect(arithOp, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(arithOp, exp->accepting, TOKEN_CAT_CONSTANT, NONE);
    connect(arithOp, exp->start, TOKEN_CAT_LPAREN, PUSH);

    connect(logicOp, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(logicOp, exp->accepting, TOKEN_CAT_CONSTANT, NONE);
    connect(logicOp, exp->start, TOKEN_CAT_LPAREN, PUSH);

    connect(rel, exp->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(rel, exp->accepting, TOKEN_CAT_CONSTANT, NONE);
    connect(rel, exp->start, TOKEN_CAT_LPAREN, PUSH);


    return exp;
}



SubGraph *condition(SyntaxGraph *g){
    SubGraph *condition = addSubGraph(g);

    SyntaxVertex *logic_op, *relational_op;
    logic_op = createSyntaxVertex(g);
    relational_op = createSyntaxVertex(g);

    connect(condition->start, relational_op , TOKEN_CAT_RELATIONAL_OP, NONE);
    connect(relational_op, condition->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    connect(relational_op, condition->accepting, TOKEN_CAT_CONSTANT, NONE);

    connect(condition->accepting, logic_op, TOKEN_CAT_LOGICAL_OP, NONE);

    SyntaxVertex *value = createSyntaxVertex(g);
    connect(logic_op, value, TOKEN_CAT_IDENTIFIER, NONE);
    connect(logic_op, value, TOKEN_CAT_CONSTANT, NONE);
    connect(value, relational_op, TOKEN_CAT_RELATIONAL_OP, NONE);
    connect(relational_op, condition->accepting, TOKEN_CAT_CONSTANT, NONE);
    connect(relational_op, condition->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    return condition;
}  

SubGraph *ifStatement(SyntaxGraph *g){
    SubGraph *ifStatement = addSubGraph(g);
    SubGraph *conditionGraph = condition(g);

    SyntaxVertex *LPAREN, *RPAREN, *RBRACE, *LBRACE;

    LPAREN = createSyntaxVertex(g);
    RPAREN = createSyntaxVertex(g);
    LBRACE = createSyntaxVertex(g);
    RBRACE = createSyntaxVertex(g);
    ifStatement->accepting->state = Accepting;

    //start the if statement
    connect(ifStatement->start, LPAREN, TOKEN_CAT_LPAREN, PUSH);
    
    //can start at id or const
    connect(LPAREN, conditionGraph->start, TOKEN_CAT_IDENTIFIER, NONE);
    connect(LPAREN, conditionGraph->start, TOKEN_CAT_CONSTANT, NONE);

    //reach end
    connect(conditionGraph->accepting, ifStatement->accepting, TOKEN_CAT_RPAREN, POP);
    connect(conditionGraph->start, ifStatement->accepting, TOKEN_CAT_RPAREN, POP);


    return ifStatement;
}

SubGraph *elsifStatement(SyntaxGraph *g){
    SubGraph *elsifStatement = addSubGraph(g);
    SubGraph *conditionGraph = condition(g);

    SyntaxVertex *LPAREN, *RPAREN, *LBRACE;

    LPAREN = createSyntaxVertex(g);
    RPAREN = createSyntaxVertex(g);
    LBRACE = createSyntaxVertex(g);
    elsifStatement->accepting->state = Accepting;

    //start the elsif statement
    connect(elsifStatement->start, LPAREN, TOKEN_CAT_LPAREN, PUSH);
    
    //can start at id or const
    connect(LPAREN, conditionGraph->start, TOKEN_CAT_IDENTIFIER, NONE);
    connect(LPAREN, conditionGraph->start, TOKEN_CAT_CONSTANT, NONE);

    //reach end
    connect(conditionGraph->accepting, RPAREN, TOKEN_CAT_RPAREN, POP);
    connect(conditionGraph->start, RPAREN, TOKEN_CAT_RPAREN, POP);

    //open brace
    connect(RPAREN, elsifStatement->accepting, TOKEN_CAT_LBRACE, PUSH);

    return elsifStatement;
}

SubGraph *elseStatement(SyntaxGraph *g){
    SubGraph *elseStatement = addSubGraph(g);

    SyntaxVertex *LBRACE;

    LBRACE = createSyntaxVertex(g);
    elseStatement->start = elseStatement->accepting;
    // from here all we need is in the main graph connect the braces to code block    

    return elseStatement;
}

SubGraph *untilStatement(SyntaxGraph *g){
    SubGraph *untilStatement = addSubGraph(g);
    SubGraph *conditionGraph = condition(g);
    // SubGraph *codeBlockGraph = codeBlock(g);
    // untilStatement->accepting = codeBlockGraph->accepting;

    SyntaxVertex *LPAREN, *RPAREN, *LBRACE;

    LPAREN = createSyntaxVertex(g);
    RPAREN = createSyntaxVertex(g);
    LBRACE = createSyntaxVertex(g);
    untilStatement->accepting->state = Accepting;

    //start the until statement
    connect(untilStatement->start, LPAREN, TOKEN_CAT_LPAREN, PUSH);
    
    //can start at id or const
    connect(LPAREN, conditionGraph->start, TOKEN_CAT_IDENTIFIER, NONE);
    connect(LPAREN, conditionGraph->start, TOKEN_CAT_CONSTANT, NONE);

    //reach end
    connect(conditionGraph->accepting, untilStatement->accepting, TOKEN_CAT_RPAREN, POP);
    connect(conditionGraph->start, untilStatement->accepting, TOKEN_CAT_RPAREN, POP);

    //open brace
    // connect(RPAREN, codeBlockGraph->start, TOKEN_CAT_LBRACE, PUSH);
    

    return untilStatement;
}

SubGraph *simpleTypeID(SyntaxGraph *g){
    SubGraph *simpleTypeID = addSubGraph(g);

    // type id
    connect(simpleTypeID->start, simpleTypeID->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    return simpleTypeID;
}

SubGraph *simpleDeclaration(SyntaxGraph *g){
    SubGraph *simpleDecl = addSubGraph(g);
    simpleDecl->start = simpleDecl->accepting;
    // type id --> ;(accepting)
    return simpleDecl;
}

SubGraph *simpleAssignment(SyntaxGraph *g){
    SubGraph *simpleAssignment = addSubGraph(g);

    //assign is start, end is semicolon
    connect(simpleAssignment->start, simpleAssignment->accepting, TOKEN_CAT_SEMICOLON, NONE);
    connect(simpleAssignment->start, simpleAssignment->accepting, TOKEN_CAT_SEMICOLON, NONE);

    return simpleAssignment;
}

SubGraph *codeBlock(SyntaxGraph *g){
    SubGraph *codeBlock = addSubGraph(g);
    codeBlock->accepting->state = Accepting;

    SubGraph *ifGraph = globalIfGraph;
    SubGraph *untilGraph = globalUntilGraph;

    SubGraph *typeTOidentifier = globalType;
    SubGraph *simpleDecl = simpleDeclaration(g);
    SyntaxVertex *assign = createSyntaxVertex(g);

    // from identifier to assign or semicolon.
    connect(typeTOidentifier->accepting, simpleDecl->accepting, TOKEN_CAT_SEMICOLON, NONE);
    connect(typeTOidentifier->accepting, assign, TOKEN_CAT_ASSIGN, NONE);
    
    SyntaxVertex *expSemi = createSyntaxVertex(g);
    SubGraph *exp = expression(g);

    connect(assign, exp->start, TOKEN_CAT_LPAREN, PUSH);
    connect(assign, exp->accepting,TOKEN_CAT_IDENTIFIER, NONE);
    connect(assign, exp->accepting, TOKEN_CAT_CONSTANT,   NONE);

    connect(exp->accepting, expSemi, TOKEN_CAT_SEMICOLON, NONE);
    connect(exp->accepting, expSemi, TOKEN_CAT_SEMICOLON,   NONE);

    // empty code block
    connect(codeBlock->start, codeBlock->accepting, TOKEN_CAT_RBRACE, POP);

    //from block to anywhere
    connect(codeBlock->start, ifGraph->start, TOKEN_CAT_IF, NONE);
    connect(codeBlock->start, untilGraph->start, TOKEN_CAT_UNTIL, NONE);
    connect(codeBlock->start, typeTOidentifier->start, TOKEN_CAT_TYPE, NONE);
    connect(codeBlock->start, typeTOidentifier->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    
    //from if to anywhere
    connect(ifGraph->accepting, ifGraph->start, TOKEN_CAT_IF, NONE);
    connect(ifGraph->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE);
    connect(ifGraph->accepting, globalType->start, TOKEN_CAT_TYPE, NONE);
    connect(ifGraph->accepting, globalType->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    // from until to anywhere
    connect(untilGraph->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE);
    connect(untilGraph->accepting, ifGraph->start, TOKEN_CAT_IF, NONE);
    connect(untilGraph->accepting, globalType->start, TOKEN_CAT_TYPE, NONE);
    connect(untilGraph->accepting, globalType->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    // from simpleDecl to anywhere
    connect(simpleDecl->accepting, untilGraph->start, TOKEN_CAT_UNTIL, NONE);
    connect(simpleDecl->accepting, ifGraph->start, TOKEN_CAT_IF, NONE);
    connect(simpleDecl->accepting, globalType->start, TOKEN_CAT_TYPE, NONE);
    connect(simpleDecl->accepting, globalType->accepting, TOKEN_CAT_IDENTIFIER, NONE);

    // from simpleAssign to anywhere
    connect(expSemi, untilGraph->start, TOKEN_CAT_UNTIL, NONE);
    connect(expSemi, ifGraph->start, TOKEN_CAT_IF, NONE);
    connect(expSemi, globalType->start, TOKEN_CAT_TYPE, NONE);
    connect(expSemi, globalType->accepting, TOKEN_CAT_IDENTIFIER, NONE);
    
    // from anywhere to block accepting
    connect(ifGraph->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP);
    connect(untilGraph->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP);
    connect(simpleDecl->accepting, codeBlock->accepting, TOKEN_CAT_RBRACE, POP);
    connect(expSemi, codeBlock->accepting, TOKEN_CAT_RBRACE, POP);

    return codeBlock;
}

SubGraph *functionStatement(SyntaxGraph *g){
    SubGraph *functionStatement = addSubGraph(g);
    SubGraph *functionParamsGraph = functionParams(g);
    SubGraph *codeBlockGraph = codeBlock(g);

    functionStatement->accepting = codeBlockGraph->accepting;

    SyntaxVertex *LPAREN, *RPAREN, *LBRACE, *type, *identifier;

    LPAREN = createSyntaxVertex(g);
    RPAREN = createSyntaxVertex(g);
    LBRACE = createSyntaxVertex(g);
    type = createSyntaxVertex(g);
    identifier = createSyntaxVertex(g);
    functionStatement->accepting->state = Accepting;

    connect(functionStatement->start, type, TOKEN_CAT_TYPE, NONE);
    connect(type, identifier, TOKEN_CAT_IDENTIFIER, NONE);
    connect(identifier, LPAREN, TOKEN_CAT_LPAREN, PUSH);

    connect(LPAREN, functionParamsGraph->start, TOKEN_CAT_TYPE, NONE); // params
    connect(functionParamsGraph->accepting, RPAREN, TOKEN_CAT_RPAREN, POP);

    connect(LPAREN, RPAREN, TOKEN_CAT_RPAREN, POP); // no params
    connect(RPAREN, codeBlockGraph->start, TOKEN_CAT_LBRACE, PUSH);
    connect(codeBlockGraph->accepting, functionStatement->accepting, TOKEN_CAT_RBRACE, POP);

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
    globalType->start->id = 10000;


    SubGraph *codeIF = codeBlock(g);
    SubGraph *codeELSE = codeBlock(g);
    SubGraph *codeELSIF = codeBlock(g);
    SubGraph *codeUNTIL = codeBlock(g);


    connect(globalUntilGraph->accepting, codeUNTIL->start, TOKEN_CAT_LBRACE, PUSH);
    globalUntilGraph->accepting = codeUNTIL->accepting;

    connect(globalIfGraph->accepting, codeIF->start, TOKEN_CAT_LBRACE, PUSH);
    connect(codeIF->accepting, globalElseGraph->start, TOKEN_CAT_ELSE, NONE);
    globalIfGraph->accepting = codeIF->accepting;

    connect(globalElsifGraph->accepting, codeELSIF->start, TOKEN_CAT_LBRACE, PUSH);
    connect(codeELSIF->accepting, globalElsifGraph->start, TOKEN_CAT_ELSIF, NONE);
    connect(codeELSIF->accepting, globalElseGraph->start, TOKEN_CAT_ELSE, NONE);
    globalElsifGraph->accepting = codeELSIF->accepting;

    connect(globalElseGraph->accepting, codeELSE->start, TOKEN_CAT_LBRACE, PUSH);
    globalElseGraph->accepting = codeELSE->accepting;


    SubGraph *functionStatementGraph = functionStatement(g);

    connect(g->startVertex, functionStatementGraph->start, TOKEN_CAT_FUNC, NONE);
    
    return g;
}

