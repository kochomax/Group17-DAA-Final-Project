#ifndef BRACKET_CHECKER_H
#define BRACKET_CHECKER_H

#define BRACKET_MAX_LENGTH 256

// Stack used by the bracket checker
typedef struct
{
    char items[BRACKET_MAX_LENGTH];
    int top;
} BracketStack;

// Information recorded after processing each bracket
typedef struct
{
    char symbol;

    char stackSnapshot[BRACKET_MAX_LENGTH];

    int stackSize;
    int matched;
} BracketStep;

// Final result of the bracket checking process
typedef struct
{
    int balanced;

    int stepCount;

    BracketStep steps[BRACKET_MAX_LENGTH];

    char message[160];
} BracketResult;

// Main bracket checking function
void bracketCheck(
    const char *expression,
    BracketResult *result
);

#endif