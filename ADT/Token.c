
#include "../Headers/Token.h"

Token* createToken(TOKEN_CODE code, TOKEN_CATEGORY category, char *lexeme) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->code = code;
    token->lexeme = strdup(lexeme); // Duplicate the string to avoid memory issues
    return token;
}