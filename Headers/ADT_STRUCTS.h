#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/HashTable.h"
#include "../Headers/Token.h"

typedef enum {FALSE, TRUE} BOOLEAN;

typedef enum{
    Trap,
    Accepting,
    Intermediate
}State;


typedef enum{
    NOT_FISNISHED_TOKEN, // The token is not finished yet

} ErrorID;

typedef struct{
    ErrorID id; // Error ID
    char *errorMessage; // Error message
    int lineNumber; // Line number where the error occurred
} Error;