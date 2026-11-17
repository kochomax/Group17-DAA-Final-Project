#include <stdio.h>
#include "bracket_checker.h"

// Check whether a character is an opening bracket
static int isOpening(char c)
{
    return c == '(' || c == '[' || c == '{';
}

// Check whether a character is a closing bracket
static int isClosing(char c)
{
    return c == ')' || c == ']' || c == '}';
}

// Check whether two brackets match
static int matches(char open, char close)
{
    return
        (open == '(' && close == ')') ||
        (open == '[' && close == ']') ||
        (open == '{' && close == '}');
}

// =========================================
// BRACKET BALANCE CHECKER
// =========================================

void bracketCheck(
    const char *expression,
    BracketResult *result
)
{
    BracketStack stack;

    stack.top = -1;

    result->stepCount = 0;
    result->balanced = 0;
    result->message[0] = '\0';

    if (expression == NULL || result == NULL)
    {
        if (result != NULL)
        {
            snprintf(
                result->message,
                sizeof(result->message),
                "No expression was provided."
            );
        }

        return;
    }

    int invalid = 0;

    // Process each character in the expression
    for (int i = 0; expression[i] != '\0'; i++)
    {
        char current = expression[i];

        // Ignore characters that are not brackets
        if (!isOpening(current) && !isClosing(current))
        {
            continue;
        }

        // Prevent exceeding the available step storage
        if (result->stepCount >= BRACKET_MAX_LENGTH)
        {
            snprintf(
                result->message,
                sizeof(result->message),
                "Expression contains too many brackets."
            );

            result->balanced = 0;
            return;
        }

        BracketStep *step =
            &result->steps[result->stepCount];

        step->symbol = current;
        step->matched = 0;

        // Opening bracket: push onto stack
        if (isOpening(current))
        {
            stack.items[++stack.top] = current;
        }

        // Closing bracket: check the top of the stack
        else
        {
            if (stack.top >= 0 &&
                matches(stack.items[stack.top], current))
            {
                stack.top--;

                step->matched = 1;
            }
            else
            {
                // A closing bracket has no correct opening bracket
                invalid = 1;
            }
        }

        // Record the stack size after this bracket
        step->stackSize = stack.top + 1;

        // Save the current stack contents
        for (int j = 0; j <= stack.top; j++)
        {
            step->stackSnapshot[j] = stack.items[j];
        }

        step->stackSnapshot[stack.top + 1] = '\0';

        result->stepCount++;
    }

    // Determine the final result
    if (stack.top == -1 && !invalid)
    {
        result->balanced = 1;

        snprintf(
            result->message,
            sizeof(result->message),
            "The brackets are balanced."
        );
    }
    else
    {
        result->balanced = 0;

        snprintf(
            result->message,
            sizeof(result->message),
            "The brackets are not balanced."
        );
    }
}