#pragma once

#include "ADT_STRUCTS.h"

//Token structure
typedef enum TOKEN_CODE{
    NO_TOKEN,
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

    // Identifier and main
    TOKEN_IDENTIFIER, // identifier
    TOKEN_MAIN
} TOKEN_CODE;

typedef enum TOKEN_CATEGORY{
    TOKEN_CAT_TYPE,
    TOKEN_CAT_KEYWORD,
    TOKEN_CAT_LOGICAL_OP,
    TOKEN_CAT_ARITHMETIC_OP,
    TOKEN_CAT_SPECIAL_SYMBOL,
    TOKEN_CAT_ASSIGN,
    TOKEN_CAT_SEMICOLON,
    TOKEN_CAT_CONSTANT,
    TOKEN_CAT_EXPRESSION,
    TOKEN_CAT_IDENTIFIER,       
} TOKEN_CATEGORY;

typedef struct Token{
    char *lexeme; // The actual string of the token
    TOKEN_CODE code; // The code of the token
    TOKEN_CATEGORY type; // The type of the token
} Token;

Token *createToken(TOKEN_CODE code, TOKEN_CATEGORY category, char *lexeme);
#pragma once
