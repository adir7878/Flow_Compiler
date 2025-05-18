
#include "../Headers/SyntaxGraph.h"
#include "../Headers/SyntaxGraphBuilder.h"
#include "SyntaxGraphBuilder.h"

//all accepting vertices
SyntaxVertex *semicolon;

SubGraph* createSubGraph(SyntaxGraph *syntaxGraph){
    SubGraph* subGraph = (SubGraph*)malloc(sizeof(SubGraph));
    subGraph->start = createSyntaxVertex(syntaxGraph);
    subGraph->start->isSubGraphStart = TRUE;
    subGraph->accepting = createSyntaxVertex(syntaxGraph);
    return subGraph;
}

SubGraph* buildExpression(SyntaxGraph *syntaxGraph){
    SubGraph* expSubGraph = createSubGraph(syntaxGraph);
    SyntaxVertex *logicOperator, *arithmeticOperator, *identifier,
                *funcLPAREN, *funcRPAREN, *comma, *value;

    logicOperator = createSyntaxVertex(syntaxGraph);
    arithmeticOperator = createSyntaxVertex(syntaxGraph);
    identifier = createSyntaxVertex(syntaxGraph);
    funcLPAREN = createSyntaxVertex(syntaxGraph);
    funcRPAREN = createSyntaxVertex(syntaxGraph);
    comma = createSyntaxVertex(syntaxGraph);
    value = createSyntaxVertex(syntaxGraph);


    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_CONSTANT, expSubGraph->accepting);
    

    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_LPAREN, expSubGraph->start);

    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_ARITHMETIC_OP, arithmeticOperator);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_LOGICAL_OP, logicOperator);

    addSyntaxEdge(arithmeticOperator, TOKEN_CAT_CONSTANT, expSubGraph->accepting);
    addSyntaxEdge(arithmeticOperator, TOKEN_CAT_IDENTIFIER, expSubGraph->accepting);
    addSyntaxEdge(arithmeticOperator, TOKEN_CAT_LPAREN,expSubGraph->start);

    addSyntaxEdge(logicOperator, TOKEN_CAT_CONSTANT, expSubGraph->accepting);
    addSyntaxEdge(logicOperator, TOKEN_CAT_IDENTIFIER, expSubGraph->accepting);
    addSyntaxEdge(logicOperator, TOKEN_CAT_LPAREN, expSubGraph->start);


    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_RPAREN, expSubGraph->accepting);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_SEMICOLON, semicolon);

    
    // function call : add()
    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_IDENTIFIER, identifier);
    addSyntaxEdge(identifier, TOKEN_CAT_LPAREN, funcLPAREN);
    addSyntaxEdge(funcLPAREN, TOKEN_CAT_CONSTANT, value);
    addSyntaxEdge(funcLPAREN, TOKEN_CAT_IDENTIFIER, value);
    addSyntaxEdge(funcLPAREN, TOKEN_CAT_RPAREN, expSubGraph->accepting);

    // function call : add(5, 3)
    addSyntaxEdge(value, TOKEN_CAT_COMMA, comma);
    addSyntaxEdge(comma, TOKEN_CAT_CONSTANT, value);
    addSyntaxEdge(value, TOKEN_CAT_RPAREN, expSubGraph->accepting);

    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_IDENTIFIER, identifier);
    addSyntaxEdge(identifier, TOKEN_CAT_SEMICOLON, semicolon);
    
    



    return expSubGraph;
}

SubGraph* arrayInitializationValues(SyntaxGraph *syntaxGraph){
    SubGraph* arrValuesSubGraph = createSubGraph(syntaxGraph);
    SyntaxVertex *lBrace, *comma, *value;

    lBrace = createSyntaxVertex(syntaxGraph);
    comma = createSyntaxVertex(syntaxGraph);
    value = createSyntaxVertex(syntaxGraph);

    //array initialization : {1, 2, 3, 4, 5}
    addSyntaxEdge(arrValuesSubGraph->start, TOKEN_CAT_LBRACE, lBrace);
    addSyntaxEdge(lBrace, TOKEN_CAT_CONSTANT, value);
    addSyntaxEdge(lBrace, TOKEN_CAT_IDENTIFIER, value);
    
    addSyntaxEdge(value, TOKEN_CAT_COMMA, comma);
    addSyntaxEdge(comma, TOKEN_CAT_CONSTANT, value);
    addSyntaxEdge(comma, TOKEN_CAT_IDENTIFIER, value);

    addSyntaxEdge(value, TOKEN_CAT_RBRACE, arrValuesSubGraph->accepting);
    
    return arrValuesSubGraph;
}

SubGraph* allDeclarations(SyntaxGraph *syntaxGraph, SubGraph* expSubGraph, SubGraph* arrValuesSubGraph){
    SubGraph* declarationSubGraph = createSubGraph(syntaxGraph);
    SyntaxVertex *type, *identifier, *assignment, *lBraket, *rBraket,
    *funcName,*lParenFunc,*rParenFunc;

    type = createSyntaxVertex(syntaxGraph);
    identifier = createSyntaxVertex(syntaxGraph);
    assignment = createSyntaxVertex(syntaxGraph);
    lBraket = createSyntaxVertex(syntaxGraph);
    rBraket = createSyntaxVertex(syntaxGraph);
    funcName = createSyntaxVertex(syntaxGraph);
    lParenFunc = createSyntaxVertex(syntaxGraph);
    rParenFunc = createSyntaxVertex(syntaxGraph);


    //declaration no value : int x;
    addSyntaxEdge(declarationSubGraph->start, TOKEN_CAT_TYPE, type);
    addSyntaxEdge(type, TOKEN_CAT_IDENTIFIER, identifier);
    addSyntaxEdge(identifier, TOKEN_CAT_SEMICOLON, semicolon); 

    //declaration with value : int x -> 5;, int x -> 5 + 3;, int x -> (5 + 3);

    addSyntaxEdge(identifier, TOKEN_CAT_ASSIGN, assignment);
    addSyntaxEdge(assignment, TOKEN_CAT_CONSTANT, expSubGraph->start);
    addSyntaxEdge(assignment, TOKEN_CAT_IDENTIFIER, expSubGraph->start);
    addSyntaxEdge(assignment, TOKEN_CAT_CONSTANT, expSubGraph->start);
    addSyntaxEdge(assignment, TOKEN_CAT_LPAREN, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_SEMICOLON, semicolon);

    //declaration with function : int x -> add(5,3);

    addSyntaxEdge(assignment, TOKEN_CAT_IDENTIFIER, funcName);
    addSyntaxEdge(funcName, TOKEN_CAT_LPAREN, lParenFunc);
    addSyntaxEdge(lParenFunc, TOKEN_CAT_CONSTANT, expSubGraph->start);
    addSyntaxEdge(lParenFunc, TOKEN_CAT_IDENTIFIER, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_COMMA, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_RPAREN, rParenFunc);
    addSyntaxEdge(rParenFunc, TOKEN_CAT_SEMICOLON, semicolon);

    //declaration with array : int x[5];
    addSyntaxEdge(identifier, TOKEN_CAT_LBRACKET, lBraket);

    //expression inside the brackets
    addSyntaxEdge(lBraket, TOKEN_CAT_IDENTIFIER, expSubGraph->start);
    addSyntaxEdge(lBraket, TOKEN_CAT_CONSTANT, expSubGraph->start);
    addSyntaxEdge(lBraket, TOKEN_CAT_LPAREN, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_RBRACKET, rBraket);
    addSyntaxEdge(rBraket, TOKEN_CAT_SEMICOLON, semicolon);

    //declaration with array and values : int x[5] -> {1, 2, 3, 4, 5};
    assignment = createSyntaxVertex(syntaxGraph); // new vertex for array assignment
    addSyntaxEdge(rBraket, TOKEN_CAT_ASSIGN, assignment);
    addSyntaxEdge(assignment, TOKEN_CAT_LBRACE, arrValuesSubGraph->start);
    addSyntaxEdge(arrValuesSubGraph->accepting, TOKEN_CAT_SEMICOLON, semicolon);
    

    return declarationSubGraph;
}

SubGraph *ifStatement(SyntaxGraph *syntaxGraph, SubGraph *expSubGraph){
    SubGraph *ifSubGraph = createSubGraph(syntaxGraph);

    SyntaxVertex *if_keyword, *elsif_keyword, *else_keyword, *lParen,
        *lBraceIf, *rBraceIf, *lBraceElsif, *rBraceElsif, *lBraceElse, *rBraceElse;

    if_keyword = createSyntaxVertex(syntaxGraph);
    elsif_keyword = createSyntaxVertex(syntaxGraph);
    else_keyword = createSyntaxVertex(syntaxGraph);

    lParen = createSyntaxVertex(syntaxGraph);
    lBraceIf = createSyntaxVertex(syntaxGraph);
    rBraceIf = createSyntaxVertex(syntaxGraph);
    rBraceIf->state = Accepting;

    lBraceElsif = createSyntaxVertex(syntaxGraph);
    rBraceElsif = createSyntaxVertex(syntaxGraph);
    rBraceElsif->state = Accepting;

    lBraceElse = createSyntaxVertex(syntaxGraph);
    rBraceElse = createSyntaxVertex(syntaxGraph);
    rBraceElse->state = Accepting;


    // if (expression) { ... }
    addSyntaxEdge(ifSubGraph->start, TOKEN_CAT_IF, if_keyword);
    addSyntaxEdge(if_keyword, TOKEN_CAT_LPAREN, lParen);
    addSyntaxEdge(lParen, TOKEN_CAT_CONSTANT, expSubGraph->start);
    addSyntaxEdge(lParen, TOKEN_CAT_IDENTIFIER, expSubGraph->start);
    addSyntaxEdge(lParen, TOKEN_CAT_LPAREN, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_LBRACE, lBraceIf);
    addSyntaxEdge(lBraceIf, TOKEN_CAT_RBRACE, rBraceIf);

    // elsif (expression) { ... }
    addSyntaxEdge(rBraceIf, TOKEN_CAT_ELSIF, elsif_keyword);
    addSyntaxEdge(elsif_keyword, TOKEN_CAT_LPAREN, lParen);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_LBRACE, lBraceElsif);
    addSyntaxEdge(lBraceElsif, TOKEN_CAT_RBRACE, rBraceElsif);

    // else { ... }
    addSyntaxEdge(rBraceIf, TOKEN_CAT_ELSE, else_keyword);
    addSyntaxEdge(rBraceElsif, TOKEN_CAT_ELSE, else_keyword);
    addSyntaxEdge(else_keyword, TOKEN_CAT_LBRACE, lBraceElse);
    addSyntaxEdge(lBraceElse, TOKEN_CAT_RBRACE, rBraceElse);

    return ifSubGraph;
}

SubGraph *UntilStatement(SyntaxGraph *syntaxGraph, SubGraph *expSubGraph){
    SubGraph *untilSubGraph = createSubGraph(syntaxGraph);
    SyntaxVertex *until_keyword, *lParen, *lBrace, *rBrace;

    until_keyword = createSyntaxVertex(syntaxGraph);
    lParen = createSyntaxVertex(syntaxGraph);
    lBrace = createSyntaxVertex(syntaxGraph);
    rBrace = createSyntaxVertex(syntaxGraph);
    rBrace->state = Accepting;

    // until (expression) { ... }
    addSyntaxEdge(untilSubGraph->start, TOKEN_CAT_UNTIL, until_keyword);
    addSyntaxEdge(until_keyword, TOKEN_CAT_LPAREN, lParen);
    addSyntaxEdge(lParen, TOKEN_CAT_CONSTANT, expSubGraph->start);
    addSyntaxEdge(lParen, TOKEN_CAT_IDENTIFIER, expSubGraph->start);
    addSyntaxEdge(lParen, TOKEN_CAT_LPAREN, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_LBRACE, lBrace);
    // TODO: add until body
    addSyntaxEdge(lBrace, TOKEN_CAT_RBRACE, rBrace);

    return untilSubGraph;
}
SubGraph *funcParams(SyntaxGraph *syntaxGraph){
    SubGraph *funcParamsSubGraph = createSubGraph(syntaxGraph);
    SyntaxVertex *comma, *type;

    comma = createSyntaxVertex(syntaxGraph);
    type = createSyntaxVertex(syntaxGraph);

    addSyntaxEdge(funcParamsSubGraph->start, TOKEN_CAT_TYPE, type);
    addSyntaxEdge(type, TOKEN_CAT_IDENTIFIER, funcParamsSubGraph->accepting);
    addSyntaxEdge(funcParamsSubGraph->accepting, TOKEN_CAT_COMMA, comma);
    addSyntaxEdge(comma, TOKEN_CAT_TYPE, type);

    //start -> type -> identifier(accepting) -> comma -> type;

    return funcParamsSubGraph;
}

SubGraph *funcStatement(SyntaxGraph *syntaxGraph, SubGraph *funcParamsSubGraph){
    SubGraph *funcSubGraph = createSubGraph(syntaxGraph);
    SyntaxVertex *func_keyword, *type, *name, *lParen, *rParen, *lBrace, *rBrace;

    func_keyword = createSyntaxVertex(syntaxGraph);
    type = createSyntaxVertex(syntaxGraph);
    name = createSyntaxVertex(syntaxGraph);
    lParen = createSyntaxVertex(syntaxGraph);
    rParen = createSyntaxVertex(syntaxGraph);
    lBrace = createSyntaxVertex(syntaxGraph);
    rBrace = createSyntaxVertex(syntaxGraph);
    rBrace->state = Accepting;

    // func () 
    addSyntaxEdge(funcSubGraph->start, TOKEN_CAT_FUNC, func_keyword);
    addSyntaxEdge(func_keyword, TOKEN_CAT_TYPE, type);
    addSyntaxEdge(type, TOKEN_CAT_IDENTIFIER, name);
    addSyntaxEdge(name, TOKEN_CAT_LPAREN, lParen);
    addSyntaxEdge(lParen, TOKEN_CAT_RPAREN, rParen);

    // func (params) 
    addSyntaxEdge(lParen, TOKEN_CAT_TYPE, funcParamsSubGraph->start);
    addSyntaxEdge(funcParamsSubGraph->accepting, TOKEN_CAT_RPAREN, rParen);

    // { ... }
    addSyntaxEdge(rParen, TOKEN_CAT_LBRACE, lBrace);
    // TODO: add function body
    addSyntaxEdge(lBrace, TOKEN_CAT_RBRACE, rBrace);

    return funcSubGraph;
}

SyntaxVertex *createAcceptingVertex(SyntaxGraph *syntaxGraph, State state){
    SyntaxVertex *accepting = createSyntaxVertex(syntaxGraph);
    accepting->state = state;
    return accepting;
}

SyntaxGraph *createPDA(){

    //init graph
    SyntaxGraph *syntaxGraph = createSyntaxGraph();
    syntaxGraph->startVertex = createSyntaxVertex(syntaxGraph);

    semicolon = createAcceptingVertex(syntaxGraph, Accepting); // ;
    
    SubGraph* declarationSubGraph = allDeclarations(syntaxGraph, buildExpression(syntaxGraph), arrayInitializationValues(syntaxGraph));
    SubGraph* if_elsif_elseSubGraph = ifStatement(syntaxGraph, buildExpression(syntaxGraph));
    SubGraph* funcSubGraph = funcStatement(syntaxGraph, funcParams(syntaxGraph));

    addSyntaxEdge(syntaxGraph->startVertex, TOKEN_CAT_FUNC, funcSubGraph->start);
    addSyntaxEdge(syntaxGraph->startVertex, TOKEN_CAT_TYPE, declarationSubGraph->start);
    addSyntaxEdge(syntaxGraph->startVertex, TOKEN_CAT_IF, if_elsif_elseSubGraph->start);

    return syntaxGraph;
}

