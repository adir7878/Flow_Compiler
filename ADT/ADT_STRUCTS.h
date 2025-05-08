#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum{
    Trap,
    Accepting,
    Intermediate
}State;

//Token structure
typedef enum TOKEN_CODE{
    // Data types
    TOKEN_INT,
    TOKEN_SHORT,
    TOKEN_LONG,
    TOKEN_DOUBLE,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_BYTE,
    TOKEN_BOOLEAN,

    // Keywords
    TOKEN_IF,
    TOKEN_ELSIF,
    TOKEN_ELSE,
    TOKEN_UNTIL,
    TOKEN_FUNC,
    TOKEN_RETURN,
    TOKEN_BLANK,

    // Logical operators
    TOKEN_AND,        // AND
    TOKEN_OR,         // OR
    TOKEN_NOT,        // NOT
    TOKEN_EQUAL,      // ==
    TOKEN_NOTEQUAL,   // !=
    TOKEN_GT,         // >
    TOKEN_LT,         // <
    TOKEN_GTE,        // >=
    TOKEN_LTE,        // <=

    // Arithmetic operators
    TOKEN_ADD,        // +
    TOKEN_SUB,        // -
    TOKEN_MUL,        // *
    TOKEN_DIV,        // /
    TOKEN_MOD,        // %
    TOKEN_POW,        // ^
    TOKEN_ROOT,       // ~

    // Special symbols
    TOKEN_SEMICOLON,  // ;
    TOKEN_LBRACE,     // {
    TOKEN_RBRACE,     // }
    TOKEN_LBRACKET,   // [
    TOKEN_RBRACKET,   // ]
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_ASSIGN,      // ->
    TOKEN_COMMA,      // ,

    // Constants
    TOKEN_NUMBER,     // numeric literal
    TOKEN_TRUE,       // true
    TOKEN_FALSE,      // false

    // Identifier and EOF
    TOKEN_IDENTIFIER // identifier
} TOKEN_CODE;

typedef enum TOKEN_CATEGORY{
    TOKEN_CAT_TYPE,             // data types: int, short, long, double, float, char, byte, boolean
    TOKEN_CAT_KEYWORD,          // language keywords: if, elsif, else, until, func, return, blank
    TOKEN_CAT_LOGICAL_OP,       // logical operators: AND, OR, NOT, ==, !=, >, <, >=, <=
    TOKEN_CAT_ARITHMETIC_OP,    // arithmetic operators: +, -, *, /, %, ^, ~
    TOKEN_CAT_SPECIAL_SYMBOL,   // punctuation & symbols: ; { } [ ] -> ,
    TOKEN_CAT_CONSTANT,         // literals: numeric (TOKEN_NUMBER), true, false
    TOKEN_CAT_IDENTIFIER,       // identifiers
    TOKEN_CAT_EOF               // end of input
} TOKEN_CATEGORY;

typedef struct Token{
    char *lexeme; // The actual string of the token
    TOKEN_CODE code; // The code of the token
    TOKEN_CATEGORY type; // The type of the token
} Token;


// Template structure
