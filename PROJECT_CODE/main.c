
#include "../Headers/ADT_STRUCTS.h"
#include "../Headers/LexerGraphBuilder.h"
#include "../Headers/LLL_Node.h"
#include "../Headers/Tokeniztion.h"

static void initializeSymbolTable(){

    printf("Initializing symbol table...\n");
    symbolTable = createHashTable(100);

    hashTableInsert(symbolTable, TOKEN_INT, createToken(TOKEN_INT, TOKEN_CAT_TYPE, "int"));
    hashTableInsert(symbolTable, TOKEN_SHORT, createToken(TOKEN_SHORT, TOKEN_CAT_TYPE, "short"));
    hashTableInsert(symbolTable, TOKEN_LONG, createToken(TOKEN_LONG, TOKEN_CAT_TYPE, "long"));
    hashTableInsert(symbolTable, TOKEN_DOUBLE, createToken(TOKEN_DOUBLE, TOKEN_CAT_TYPE, "double"));
    hashTableInsert(symbolTable, TOKEN_FLOAT, createToken(TOKEN_FLOAT, TOKEN_CAT_TYPE, "float"));
    hashTableInsert(symbolTable, TOKEN_CHAR, createToken(TOKEN_CHAR, TOKEN_CAT_TYPE, "char"));
    hashTableInsert(symbolTable, TOKEN_BYTE, createToken(TOKEN_BYTE, TOKEN_CAT_TYPE, "byte"));
    hashTableInsert(symbolTable, TOKEN_BOOLEAN, createToken(TOKEN_BOOLEAN, TOKEN_CAT_TYPE, "boolean"));
    hashTableInsert(symbolTable, TOKEN_IF, createToken(TOKEN_IF, TOKEN_CAT_KEYWORD, "if"));
    hashTableInsert(symbolTable, TOKEN_ELSIF, createToken(TOKEN_ELSIF, TOKEN_CAT_KEYWORD, "elsif"));
    hashTableInsert(symbolTable, TOKEN_ELSE, createToken(TOKEN_ELSE, TOKEN_CAT_KEYWORD, "else"));
    hashTableInsert(symbolTable, TOKEN_UNTIL, createToken(TOKEN_UNTIL, TOKEN_CAT_KEYWORD, "until"));
    hashTableInsert(symbolTable, TOKEN_FUNC, createToken(TOKEN_FUNC, TOKEN_CAT_KEYWORD, "func"));
    hashTableInsert(symbolTable, TOKEN_RETURN, createToken(TOKEN_RETURN, TOKEN_CAT_KEYWORD, "return"));
    hashTableInsert(symbolTable, TOKEN_BLANK, createToken(TOKEN_BLANK, TOKEN_CAT_KEYWORD, "blank"));
    hashTableInsert(symbolTable, TOKEN_AND, createToken(TOKEN_AND, TOKEN_CAT_LOGICAL_OP, "AND"));
    hashTableInsert(symbolTable, TOKEN_OR, createToken(TOKEN_OR, TOKEN_CAT_LOGICAL_OP, "OR"));
    hashTableInsert(symbolTable, TOKEN_NOT, createToken(TOKEN_NOT, TOKEN_CAT_LOGICAL_OP, "NOT"));
    hashTableInsert(symbolTable, TOKEN_EQUAL, createToken(TOKEN_EQUAL, TOKEN_CAT_LOGICAL_OP, "=="));
    hashTableInsert(symbolTable, TOKEN_NOTEQUAL, createToken(TOKEN_NOTEQUAL, TOKEN_CAT_LOGICAL_OP, "!="));
    hashTableInsert(symbolTable, TOKEN_GT, createToken(TOKEN_GT, TOKEN_CAT_LOGICAL_OP, ">"));
    hashTableInsert(symbolTable, TOKEN_LT, createToken(TOKEN_LT, TOKEN_CAT_LOGICAL_OP, "<"));
    hashTableInsert(symbolTable, TOKEN_GTE, createToken(TOKEN_GTE, TOKEN_CAT_LOGICAL_OP, ">="));
    hashTableInsert(symbolTable, TOKEN_LTE, createToken(TOKEN_LTE, TOKEN_CAT_LOGICAL_OP, "<="));
    hashTableInsert(symbolTable, TOKEN_ADD, createToken(TOKEN_ADD, TOKEN_CAT_ARITHMETIC_OP, "+"));
    hashTableInsert(symbolTable, TOKEN_SUB, createToken(TOKEN_SUB, TOKEN_CAT_ARITHMETIC_OP, "-"));
    hashTableInsert(symbolTable, TOKEN_MUL, createToken(TOKEN_MUL, TOKEN_CAT_ARITHMETIC_OP, "*"));
    hashTableInsert(symbolTable, TOKEN_DIV, createToken(TOKEN_DIV, TOKEN_CAT_ARITHMETIC_OP, "/"));
    hashTableInsert(symbolTable, TOKEN_MOD, createToken(TOKEN_MOD, TOKEN_CAT_ARITHMETIC_OP, "%"));
    hashTableInsert(symbolTable, TOKEN_POW, createToken(TOKEN_POW, TOKEN_CAT_ARITHMETIC_OP, "^"));
    hashTableInsert(symbolTable, TOKEN_ROOT, createToken(TOKEN_ROOT, TOKEN_CAT_ARITHMETIC_OP, "~"));
    hashTableInsert(symbolTable, TOKEN_SEMICOLON, createToken(TOKEN_SEMICOLON, TOKEN_CAT_SPECIAL_SYMBOL, ";"));
    hashTableInsert(symbolTable, TOKEN_LBRACE, createToken(TOKEN_LBRACE, TOKEN_CAT_SPECIAL_SYMBOL, "{"));
    hashTableInsert(symbolTable, TOKEN_RBRACE, createToken(TOKEN_RBRACE, TOKEN_CAT_SPECIAL_SYMBOL, "}"));
    hashTableInsert(symbolTable, TOKEN_LBRACKET, createToken(TOKEN_LBRACKET, TOKEN_CAT_SPECIAL_SYMBOL, "["));
    hashTableInsert(symbolTable, TOKEN_RBRACKET, createToken(TOKEN_RBRACKET, TOKEN_CAT_SPECIAL_SYMBOL, "]"));
    hashTableInsert(symbolTable, TOKEN_LPAREN, createToken(TOKEN_LPAREN, TOKEN_CAT_SPECIAL_SYMBOL, "("));
    hashTableInsert(symbolTable, TOKEN_RPAREN, createToken(TOKEN_RPAREN, TOKEN_CAT_SPECIAL_SYMBOL, ")"));
    hashTableInsert(symbolTable, TOKEN_ASSIGN, createToken(TOKEN_ASSIGN, TOKEN_CAT_SPECIAL_SYMBOL, "->"));
    hashTableInsert(symbolTable, TOKEN_COMMA, createToken(TOKEN_COMMA, TOKEN_CAT_SPECIAL_SYMBOL, ","));
    hashTableInsert(symbolTable, TOKEN_TRUE, createToken(TOKEN_TRUE, TOKEN_CAT_CONSTANT, "true"));
    hashTableInsert(symbolTable, TOKEN_FALSE, createToken(TOKEN_FALSE, TOKEN_CAT_CONSTANT, "false"));
    hashTableInsert(symbolTable, TOKEN_IDENTIFIER, createToken(TOKEN_IDENTIFIER, TOKEN_CAT_IDENTIFIER, "identifier"));
}

static void initializeErrorTable(){

    printf("Initializing error table...\n");
    ErrorTable = createHashTable(100);
}

int main(){

    LexerGraph *DFA;
    LLL_List *tokens;
    FILE *sourceCode = fopen("C:\\Users\\adir7\\Flow_Compiler\\TestFiles\\test_1.txt", "r");
    if (sourceCode == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }else{
        printf("File opened successfully\n");
    }

    // Initialize the symbol table and error table
    initializeSymbolTable();
    initializeErrorTable();

    printf("%s", ((Token*)(hashTableSearch(symbolTable, TOKEN_INT)))->lexeme);
    
    DFA = CreateDFA(symbolTable);
    tokens = TokenizeCode(sourceCode, DFA);

    while(tokens != NULL){
        Token *token = (Token*)tokens->data;
        if(token != NULL){
            printf("Token: %s\n", token->lexeme);
        }
        tokens = tokens->next;
    }
    
    freeHashTable(symbolTable);
    freeHashTable(ErrorTable);
    freeLexerGraph(DFA);
    free(DFA);
    return 0;
}