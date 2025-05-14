
#include "../Headers/SyntaxGraph.h"
#include "../Headers/SyntaxGraphBuilder.h"

SubGraph* buildExpressionSubGraph(SyntaxGraph *syntaxGraph){
    SubGraph* expSubGraph = (SubGraph*)malloc(sizeof(SubGraph));
    SyntaxVertex *constV, *identifierV, *logicOperator, *arithmeticOpeartor;

    //init subgraph
    expSubGraph->start = createSyntaxVertex(syntaxGraph);
    expSubGraph->accepting = createSyntaxVertex(syntaxGraph);

    constV = createSyntaxVertex(syntaxGraph);
    identifierV = createSyntaxVertex(syntaxGraph);
    logicOperator = createSyntaxVertex(syntaxGraph);
    arithmeticOpeartor = createSyntaxVertex(syntaxGraph);

    constV->state = Trap;
    identifierV->state = Trap;
    logicOperator->state = Trap;
    arithmeticOpeartor->state = Trap;

    //add edges

    // in case of value and nothing after.
    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_CONSTANT, expSubGraph->accepting);
    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_IDENTIFIER, expSubGraph->accepting);

    //in case of expression.
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_ARITHMETIC_OP, arithmeticOpeartor);
    addSyntaxEdge(expSubGraph->start, TOKEN_CAT_LOGICAL_OP, expSubGraph->accepting);
    
    addSyntaxEdge(arithmeticOpeartor, TOKEN_CAT_CONSTANT, expSubGraph->accepting);
    addSyntaxEdge(arithmeticOpeartor, TOKEN_CAT_IDENTIFIER, expSubGraph->accepting);

    addSyntaxEdge(logicOperator, TOKEN_CAT_CONSTANT, expSubGraph->accepting);
    addSyntaxEdge(logicOperator, TOKEN_CAT_IDENTIFIER, expSubGraph->accepting);
    
    return expSubGraph;
}

SyntaxGraph *createPDA(){
    SyntaxGraph *syntaxGraph = createSyntaxGraph();
    SubGraph* expSubGraph = buildExpressionSubGraph(syntaxGraph);

    SyntaxVertex *start = syntaxGraph->startVertex;

    SyntaxVertex *type, *identifier, *assign, *semicolon;
    type = createSyntaxVertex(syntaxGraph);
    identifier = createSyntaxVertex(syntaxGraph);
    assign = createSyntaxVertex(syntaxGraph);
    semicolon = createSyntaxVertex(syntaxGraph);

    addSyntaxEdge(start, TOKEN_CAT_TYPE, type);
    addSyntaxEdge(type, TOKEN_CAT_IDENTIFIER, identifier);
    addSyntaxEdge(identifier, TOKEN_CAT_ASSIGN, assign);
    addSyntaxEdge(assign, TOKEN_CAT_EXPRESSION, expSubGraph->start);
    addSyntaxEdge(expSubGraph->accepting, TOKEN_CAT_SEMICOLON, semicolon);

    return syntaxGraph;
}

