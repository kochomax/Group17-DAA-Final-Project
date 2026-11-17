
#include "raylib.h"
#include "recursion.h"
#include "hanoi.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// Application screens
typedef enum
{
    DASHBOARD,
    RECURSION_LAB,
    DATA_STRUCTURES_LAB,
    SORTING_LAB,
    COMPLEXITY_LAB
} Screen;

// Recursion Lab sections
typedef enum
{
    FIBONACCI,
    FACTORIAL,
    TOWER_OF_HANOI
} RecursionMode;

// Input buffer
char inputText[16] = "";
int inputValue = 0;
bool inputActive = false;
bool resultReady = false;
char resultText[256] = "";

Screen currentScreen = DASHBOARD;

// Stack Lab variables
Stack userStack;
bool stackInitialized = false;
char stackInput[16] = "";
bool stackInputActive = false;
char stackMessage[256] = "Create a stack by selecting its capacity.";
int stackCapacityInput = 5;
int stackOperationValue = 0;
RecursionMode recursionMode = FIBONACCI;
// Tower of Hanoi settings
int hanoiDisks = 3;
int hanoiMovesMade = 0;
int hanoiTotal = 0;
bool hanoiRunning = false;
bool hanoiPaused = false;
double hanoiMoveTimer = 0.0;
float hanoiMoveSpeed = 1.0f;
HanoiMove hanoiMoves[MAX_HANOI_MOVES];
int hanoiMoveCount = 0;
int hanoiCurrentMove = 0;
// Store disks on the three pegs
int pegDisks[3][8];
int pegCounts[3] = {0, 0, 0};

// Draw a button
void DrawButton(Rectangle button, const char *text)
{
    Vector2 mouse = GetMousePosition();

    bool hovering = CheckCollisionPointRec(mouse, button);

    Color buttonColor = hovering ? BLUE : DARKBLUE;

    DrawRectangleRounded(button, 0.2f, 10, buttonColor);

    int textWidth = MeasureText(text, 20);

    DrawText(text,
             button.x + (button.width - textWidth) / 2,
             button.y + 30,
             20,
             WHITE);
}

// Dashboard
void DrawDashboard(void)
{
    ClearBackground(RAYWHITE);

    DrawText("ALGORITHM EXPLORER", 280, 65, 40, DARKBLUE);

    DrawText("Learn. Explore. Understand Algorithms.",
             340, 125, 20, GRAY);

    DrawText("SELECT A MODULE", 425, 200, 22, DARKGRAY);

    DrawButton((Rectangle){150, 260, 320, 85}, "Recursion Lab");
    DrawButton((Rectangle){530, 260, 320, 85}, "Data Structures Lab");
    DrawButton((Rectangle){150, 375, 320, 85}, "Sorting Lab");
    DrawButton((Rectangle){530, 375, 320, 85}, "Time Complexity");
}

// Reset input and results
void ResetRecursionInput(void)
{
    inputText[0] = '\0';
    inputValue = 0;
    resultReady = false;
    resultText[0] = '\0';
}

// Draw a simple input field
void DrawInputField(void)
{
    DrawText("Enter a number:", 150, 260, 22, DARKGRAY);

    DrawRectangle(150, 300, 250, 50, WHITE);
    DrawRectangleLines(150, 300, 250, 50, BLUE);

    DrawText(inputText, 165, 312, 22, BLACK);

    DrawText("Click the box and type a number.",
             150, 365, 16, GRAY);
}

// Calculate Fibonacci results
void CalculateFibonacci(void)
{
    int n = atoi(inputText);

    if (inputText[0] == '\0' || n < 0 || n > 30)
    {
        strcpy(resultText,
               "Enter a valid Fibonacci number from 0 to 30.");
        resultReady = true;
        return;
    }

    inputValue = n;

    long long naiveCalls = 0;
    long long memoCalls = 0;
    long long dpIterations = 0;

    long long memo[31];

    for (int i = 0; i <= 30; i++)
    {
        memo[i] = -1;
    }

    // Naive Fibonacci
    double start = GetTime();

    long long naiveResult =
        fibonacciNaive(n, &naiveCalls);

    double naiveTime = (GetTime() - start) * 1000000.0;

    // Memoized Fibonacci
    start = GetTime();

    long long memoResult =
        fibonacciMemo(n, memo, &memoCalls);

    double memoTime = (GetTime() - start) * 1000000.0;

    // Dynamic programming Fibonacci
    start = GetTime();

    long long dpResult =
        fibonacciDP(n, &dpIterations);

    double dpTime = (GetTime() - start) * 1000000.0;

    snprintf(resultText, sizeof(resultText),
        "Fibonacci(%d) = %lld\n"
        "Naive: %lld calls | %.2f us\n"
        "Memoized: %lld calls | %.2f us\n"
        "Bottom-up DP: %lld iterations | %.2f us",
        n, naiveResult,
        naiveCalls, naiveTime,
        memoCalls, memoTime,
        dpIterations, dpTime);

    // Confirm all three methods agree
    if (naiveResult != memoResult || memoResult != dpResult)
    {
        strcpy(resultText, "Calculation verification failed.");
    }

    resultReady = true;
}

// Calculate factorial results
void CalculateFactorial(void)
{
    int n = atoi(inputText);

    if (inputText[0] == '\0' || n < 0 || n > 20)
    {
        strcpy(resultText,
               "Enter a valid factorial number from 0 to 20.");
        resultReady = true;
        return;
    }

    inputValue = n;

    long long recursiveCalls = 0;
    long long iterativeIterations = 0;

    double start = GetTime();

    long long recursiveResult =
        factorialRecursive(n, &recursiveCalls);

    double recursiveTime = (GetTime() - start) * 1000000.0;

    start = GetTime();

    long long iterativeResult =
        factorialIterative(n, &iterativeIterations);

    double iterativeTime = (GetTime() - start) * 1000000.0;

    snprintf(resultText, sizeof(resultText),
        "%d! = %lld\n"
        "Recursive: %lld calls | %.2f us\n"
        "Iterative: %lld iterations | %.2f us",
        n, recursiveResult,
        recursiveCalls, recursiveTime,
        iterativeIterations, iterativeTime);

    if (recursiveResult != iterativeResult)
    {
        strcpy(resultText, "Calculation verification failed.");
    }

    resultReady = true;
}

// Recursion Lab screen
void DrawRecursionLab(void)
{
    ClearBackground(RAYWHITE);

    DrawText("RECURSION LAB", 350, 35, 35, DARKBLUE);

    DrawText("Choose an algorithm:", 150, 115, 22, DARKGRAY);

    DrawButton((Rectangle){150, 155, 220, 65}, "Fibonacci");
    DrawButton((Rectangle){390, 155, 220, 65}, "Factorial");
    DrawButton((Rectangle){630, 155, 220, 65}, "Tower of Hanoi");

    DrawInputField();

    DrawButton((Rectangle){150, 420, 250, 65}, "Calculate");

    if (resultReady)
    {
        DrawText("RESULTS", 500, 270, 22, DARKBLUE);

        DrawText(resultText, 500, 310, 17, DARKGRAY);
    }

    DrawButton((Rectangle){150, 530, 180, 60}, "BACK");
}
// Draw the Tower of Hanoi screen
void DrawHanoiScreen(void)
{
    ClearBackground(RAYWHITE);

    DrawText("TOWER OF HANOI", 350, 30, 32, DARKBLUE);

    DrawText("Recursive disk movement demonstration",
             280, 80, 20, DARKGRAY);

    DrawText(TextFormat("Number of disks: %d", hanoiDisks),
             100, 140, 20, DARKGRAY);
             DrawText("Choose disks (1-8):", 100, 180, 18, DARKGRAY);
             DrawRectangle(300, 175, 100, 35, LIGHTGRAY);
             DrawRectangleLines(300, 175, 100, 35, DARKBLUE);
             DrawText(TextFormat("%d", hanoiDisks), 335, 183, 20, DARKBLUE);
             DrawButton((Rectangle){420, 175, 70, 35}, "-");
             DrawButton((Rectangle){510, 175, 70, 35}, "+");

    DrawText(TextFormat("Moves: %d / %d",
                        hanoiMovesMade, hanoiTotal),
             700, 140, 20, DARKGRAY);

    // Draw the three pegs
    DrawLineEx((Vector2){250, 450},
               (Vector2){250, 230}, 8, DARKGRAY);

    DrawLineEx((Vector2){500, 450},
               (Vector2){500, 230}, 8, DARKGRAY);

    DrawLineEx((Vector2){750, 450},
               (Vector2){750, 230}, 8, DARKGRAY);

    // Draw the bases
    DrawLineEx((Vector2){120, 450},
               (Vector2){880, 450}, 8, DARKGRAY);
               // Draw disks on the three pegs
               for (int peg = 0; peg < 3; peg++)
               {
                for (int i = 0; i < pegCounts[peg]; i++)
                {
                    int diskSize = pegDisks[peg][i];
                    float diskWidth = 50 + diskSize * 35;
                    float diskHeight = 20;
                    float x = 250 + peg * 250 - diskWidth / 2;
                    float y = 450 - (i + 1) * diskHeight;
                    DrawRectangle(
                        (int)x,
                        (int)y,
                        (int)diskWidth,
                        (int)diskHeight,
                        BLUE
                    );
                    DrawRectangleLines(
                        (int)x,
                        (int)y,
                        (int)diskWidth,
                        (int)diskHeight,
                        DARKBLUE
                    );
                }
            }

    DrawText("A", 240, 465, 25, DARKBLUE);
    DrawText("B", 490, 465, 25, DARKBLUE);
    DrawText("C", 740, 465, 25, DARKBLUE);

   DrawButton((Rectangle){150, 530, 120, 45}, "START");
   DrawButton((Rectangle){350, 530, 120, 45}, "PAUSE");
   DrawButton((Rectangle){550, 530, 120, 45}, "STEP");
   DrawButton((Rectangle){750, 530, 120, 45}, "RESET");
   DrawButton((Rectangle){150, 480, 120, 40}, "SLOW");
   DrawButton((Rectangle){350, 480, 120, 40}, "NORMAL");
   DrawButton((Rectangle){550, 480, 120, 40}, "FAST");
   DrawButton((Rectangle){400, 590, 200, 45}, "BACK");
}
// Move one disk between two pegs
void MoveHanoiDisk(int from, int to)
{
    if (pegCounts[from] <= 0)
    {
        return;
    }

    int disk = pegDisks[from][pegCounts[from] - 1];

    pegCounts[from]--;

    pegDisks[to][pegCounts[to]] = disk;

    pegCounts[to]++;

    hanoiMovesMade++;
}

// Draw the Stack Lab screen
void DrawStackLab(void)
{
    ClearBackground(RAYWHITE);

    DrawText("STACK LAB", 400, 30, 35, DARKBLUE);

    DrawText("Array-Based Stack", 100, 100, 25, DARKGRAY);

    DrawText("Stack capacity:", 100, 155, 20, DARKGRAY);

    DrawText(TextFormat("%d", stackCapacityInput),
             280, 155, 20, DARKBLUE);

    DrawButton((Rectangle){330, 145, 50, 40}, "-");
    DrawButton((Rectangle){390, 145, 50, 40}, "+");

    DrawButton((Rectangle){470, 140, 180, 50}, "CREATE STACK");

    DrawText("Enter value:", 100, 230, 20, DARKGRAY);

    DrawRectangle(100, 265, 200, 45, WHITE);
    DrawRectangleLines(100, 265, 200, 45, BLUE);

    DrawText(stackInput, 115, 275, 20, BLACK);

    DrawButton((Rectangle){330, 260, 130, 50}, "PUSH");
    DrawButton((Rectangle){480, 260, 130, 50}, "POP");
    DrawButton((Rectangle){630, 260, 130, 50}, "PEEK");

    DrawText("STACK CONTENTS", 100, 350, 22, DARKBLUE);

    if (stackInitialized)
    {
        DrawText(TextFormat("Size: %d / %d",
                            stackSize(&userStack),
                            userStack.capacity),
                 100, 390, 20, DARKGRAY);

                 DrawText(
    TextFormat("Empty: %s", stackIsEmpty(&userStack) ? "YES" : "NO"),
    350, 390, 20, DARKGRAY
);

DrawText(
    TextFormat("Full: %s", stackIsFull(&userStack) ? "YES" : "NO"),
    550, 390, 20, DARKGRAY
);

                 

        for (int i = 0; i <= userStack.top; i++)
        {
            DrawRectangle(100, 430 - i * 35, 180, 30, SKYBLUE);
            DrawRectangleLines(100, 430 - i * 35,
                               180, 30, DARKBLUE);

            DrawText(TextFormat("%d", userStack.items[i]),
                     180, 435 - i * 35, 18, DARKBLUE);
        }
    }
    else
    {
        DrawText("No stack created yet.",
                 100, 390, 20, GRAY);
    }

    DrawText(stackMessage, 400, 370, 18, DARKGREEN);

    DrawButton((Rectangle){400, 550, 200, 55}, "BACK");
}


// Placeholder screens for remaining modules
void DrawModuleScreen(const char *title)
{
    ClearBackground(RAYWHITE);

    DrawText(title, 300, 100, 35, DARKBLUE);

    DrawText("Module content will be developed here.",
             300, 200, 22, DARKGRAY);

    DrawButton((Rectangle){400, 450, 200, 65}, "BACK");
}


int main(void)
{
    InitWindow(1000, 650, "Algorithm Explorer");

    if (!IsWindowReady())
    {
        return 1;
    }

    SetTargetFPS(60);

    // Main application loop
    while (!WindowShouldClose())
    {
        // ---------------------------------
        // AUTOMATIC TOWER OF HANOI MOVEMENT
        // ---------------------------------
        if (currentScreen == RECURSION_LAB &&
            recursionMode == TOWER_OF_HANOI &&
            hanoiRunning && !hanoiPaused)
        {
            hanoiMoveTimer += GetFrameTime();

            if (hanoiMoveTimer >= hanoiMoveSpeed)
            {
                if (hanoiCurrentMove < hanoiMoveCount)
                {
                    HanoiMove move = hanoiMoves[hanoiCurrentMove];

                    MoveHanoiDisk(move.from, move.to);

                    hanoiCurrentMove++;
                }

                hanoiMoveTimer = 0.0;

                if (hanoiCurrentMove >= hanoiMoveCount)
                {
                    hanoiRunning = false;
                }
            }
        }

        Vector2 mouse = GetMousePosition();

        // ---------------------------------
        // HANDLE KEYBOARD INPUT
        // ---------------------------------
        if (currentScreen == RECURSION_LAB && inputActive)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                if (key >= '0' && key <= '9' &&
                    strlen(inputText) < 2)
                {
                    int length = strlen(inputText);

                    inputText[length] = (char)key;
                    inputText[length + 1] = '\0';

                    resultReady = false;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                int length = strlen(inputText);

                if (length > 0)
                {
                    inputText[length - 1] = '\0';
                    resultReady = false;
                }
            }
        }

        // Handle keyboard input for Stack Lab
if (currentScreen == DATA_STRUCTURES_LAB && stackInputActive)
{
    int key = GetCharPressed();

    while (key > 0)
    {
        if (((key >= '0' && key <= '9') ||
             (key == '-' && strlen(stackInput) == 0)) &&
            strlen(stackInput) < 14)
        {
            int length = strlen(stackInput);

            stackInput[length] = (char)key;
            stackInput[length + 1] = '\0';
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        int length = strlen(stackInput);

        if (length > 0)
        {
            stackInput[length - 1] = '\0';
        }
    }
}

        // ---------------------------------
        // HANDLE MOUSE CLICKS
        // ---------------------------------
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Dashboard buttons
            if (currentScreen == DASHBOARD)
            {
                Rectangle buttons[4] =
                {
                    {150, 260, 320, 85},
                    {530, 260, 320, 85},
                    {150, 375, 320, 85},
                    {530, 375, 320, 85}
                };

                for (int i = 0; i < 4; i++)
                {
                    if (CheckCollisionPointRec(mouse, buttons[i]))
                    {
                        currentScreen = (Screen)(i + 1);
                        ResetRecursionInput();
                    }
                }
            }

            // Tower of Hanoi controls
            else if (currentScreen == RECURSION_LAB &&
                     recursionMode == TOWER_OF_HANOI)
            {
                Rectangle startButton = {150, 530, 120, 45};
                Rectangle pauseButton = {350, 530, 120, 45};
                Rectangle stepButton = {550, 530, 120, 45};
                Rectangle resetButton = {750, 530, 120, 45};

                Rectangle slowButton = {150, 480, 120, 40};
                Rectangle normalButton = {350, 480, 120, 40};
                Rectangle fastButton = {550, 480, 120, 40};

                Rectangle minusButton = {420, 175, 70, 35};
                Rectangle plusButton = {510, 175, 70, 35};

                Rectangle backHanoiButton = {400, 590, 200, 45};

                // Change number of disks
                if (CheckCollisionPointRec(mouse, minusButton) ||
                    CheckCollisionPointRec(mouse, plusButton))
                {
                    if (CheckCollisionPointRec(mouse, minusButton) &&
                        hanoiDisks > 1)
                    {
                        hanoiDisks--;
                    }
                    else if (CheckCollisionPointRec(mouse, plusButton) &&
                             hanoiDisks < 8)
                    {
                        hanoiDisks++;
                    }

                    hanoiMovesMade = 0;
                    hanoiCurrentMove = 0;
                    hanoiMoveCount = 0;

                    hanoiTotal = hanoiTotalMoves(hanoiDisks);

                    generateHanoiMoves(
                        hanoiDisks, 0, 1, 2,
                        hanoiMoves, &hanoiMoveCount
                    );

                    pegCounts[0] = 0;
                    pegCounts[1] = 0;
                    pegCounts[2] = 0;

                    for (int i = 0; i < hanoiDisks; i++)
                    {
                        pegDisks[0][i] = hanoiDisks - i;
                        pegCounts[0]++;
                    }

                    hanoiRunning = false;
                    hanoiPaused = false;
                    hanoiMoveTimer = 0.0;
                }

                // Change movement speed
                if (CheckCollisionPointRec(mouse, slowButton))
                {
                    hanoiMoveSpeed = 1.5f;
                }

                if (CheckCollisionPointRec(mouse, normalButton))
                {
                    hanoiMoveSpeed = 1.0f;
                }

                if (CheckCollisionPointRec(mouse, fastButton))
                {
                    hanoiMoveSpeed = 0.3f;
                }

                // Start automatic movement
                if (CheckCollisionPointRec(mouse, startButton))
                {
                    if (hanoiCurrentMove < hanoiMoveCount)
                    {
                        hanoiRunning = true;
                        hanoiPaused = false;
                    }
                }

                // Pause movement
                if (CheckCollisionPointRec(mouse, pauseButton))
                {
                    hanoiPaused = true;
                    hanoiRunning = false;
                }

                // Move one disk
                if (CheckCollisionPointRec(mouse, stepButton))
                {
                    if (hanoiCurrentMove < hanoiMoveCount)
                    {
                        HanoiMove move = hanoiMoves[hanoiCurrentMove];

                        MoveHanoiDisk(move.from, move.to);

                        hanoiCurrentMove++;
                    }
                }

                // Reset demonstration
                if (CheckCollisionPointRec(mouse, resetButton))
                {
                    hanoiMovesMade = 0;
                    hanoiCurrentMove = 0;

                    pegCounts[0] = 0;
                    pegCounts[1] = 0;
                    pegCounts[2] = 0;

                    for (int i = 0; i < hanoiDisks; i++)
                    {
                        pegDisks[0][i] = hanoiDisks - i;
                        pegCounts[0]++;
                    }

                    hanoiRunning = false;
                    hanoiPaused = false;
                    hanoiMoveTimer = 0.0;
                }

                // Return to recursion menu
                if (CheckCollisionPointRec(mouse, backHanoiButton))
                {
                    currentScreen = RECURSION_LAB;
                    recursionMode = FIBONACCI;
                }
            }

            // Other Recursion Lab controls
            else if (currentScreen == RECURSION_LAB)
            {
                Rectangle inputBox = {150, 300, 250, 50};
                Rectangle fibButton = {150, 155, 220, 65};
                Rectangle factButton = {390, 155, 220, 65};
                Rectangle hanoiButton = {630, 155, 220, 65};
                Rectangle calculateButton = {150, 420, 250, 65};
                Rectangle backButton = {150, 530, 180, 60};

                inputActive =
                    CheckCollisionPointRec(mouse, inputBox);

                if (CheckCollisionPointRec(mouse, fibButton))
                {
                    recursionMode = FIBONACCI;
                    ResetRecursionInput();
                }

                if (CheckCollisionPointRec(mouse, factButton))
                {
                    recursionMode = FACTORIAL;
                    ResetRecursionInput();
                }

                if (CheckCollisionPointRec(mouse, hanoiButton))
                {
                    recursionMode = TOWER_OF_HANOI;

                    hanoiDisks = 3;
                    hanoiMovesMade = 0;
                    hanoiCurrentMove = 0;
                    hanoiMoveCount = 0;

                    hanoiTotal = hanoiTotalMoves(hanoiDisks);

                    generateHanoiMoves(
                        hanoiDisks, 0, 1, 2,
                        hanoiMoves, &hanoiMoveCount
                    );

                    pegCounts[0] = 0;
                    pegCounts[1] = 0;
                    pegCounts[2] = 0;

                    for (int i = 0; i < hanoiDisks; i++)
                    {
                        pegDisks[0][i] = hanoiDisks - i;
                        pegCounts[0]++;
                    }

                    hanoiRunning = false;
                    hanoiPaused = false;
                    hanoiMoveTimer = 0.0;
                    resultReady = false;
                }

                if (CheckCollisionPointRec(mouse, calculateButton))
                {
                    if (recursionMode == FIBONACCI)
                    {
                        CalculateFibonacci();
                    }
                    else if (recursionMode == FACTORIAL)
                    {
                        CalculateFactorial();
                    }
                }

                if (CheckCollisionPointRec(mouse, backButton))
                {
                    currentScreen = DASHBOARD;
                    ResetRecursionInput();
                }
            }

            // ---------------------------------
// DATA STRUCTURES LAB - STACK CONTROLS
// ---------------------------------
else if (currentScreen == DATA_STRUCTURES_LAB)
{
    Rectangle minusButton = {330, 145, 50, 40};
    Rectangle plusButton = {390, 145, 50, 40};
    Rectangle createButton = {470, 140, 180, 50};

    Rectangle inputBox = {100, 265, 200, 45};
    Rectangle pushButton = {330, 260, 130, 50};
    Rectangle popButton = {480, 260, 130, 50};
    Rectangle peekButton = {630, 260, 130, 50};
    Rectangle backButton = {400, 550, 200, 55};

    stackInputActive =
        CheckCollisionPointRec(mouse, inputBox);

    // Change stack capacity
    if (CheckCollisionPointRec(mouse, minusButton) &&
        stackCapacityInput > 1)
    {
        stackCapacityInput--;
    }

    if (CheckCollisionPointRec(mouse, plusButton) &&
        stackCapacityInput < 20)
    {
        stackCapacityInput++;
    }

    // Create stack
    if (CheckCollisionPointRec(mouse, createButton))
    {
        if (stackInitialized)
        {
            stackFree(&userStack);
            stackInitialized = false;
        }

        if (stackInit(&userStack, stackCapacityInput))
        {
            stackInitialized = true;
            strcpy(stackMessage, "Stack created successfully!");
        }
        else
        {
            strcpy(stackMessage, "Unable to create stack.");
        }
    }

    // Push operation
    if (CheckCollisionPointRec(mouse, pushButton))
    {
        if (!stackInitialized)
        {
            strcpy(stackMessage, "Create a stack first.");
        }
        else if (stackInput[0] == '\0')
        {
            strcpy(stackMessage, "Enter a value first.");
        }
        else
        {
            char *end;
            long value = strtol(stackInput, &end, 10);

            if (*end != '\0' ||
                value < INT_MIN || value > INT_MAX)
            {
                strcpy(stackMessage, "Enter a valid integer.");
            }
            else if (stackPush(&userStack, (int)value))
            {
                strcpy(stackMessage, "Value pushed successfully!");
                stackInput[0] = '\0';
            }
            else
            {
                strcpy(stackMessage, "Stack overflow! Stack is full.");
            }
        }
    }

    // Pop operation
    if (CheckCollisionPointRec(mouse, popButton))
    {
        int value;

        if (!stackInitialized)
        {
            strcpy(stackMessage, "Create a stack first.");
        }
        else if (stackPop(&userStack, &value))
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Popped value: %d", value);
        }
        else
        {
            strcpy(stackMessage, "Stack underflow! Stack is empty.");
        }
    }

    // Peek operation
    if (CheckCollisionPointRec(mouse, peekButton))
    {
        int value;

        if (!stackInitialized)
        {
            strcpy(stackMessage, "Create a stack first.");
        }
        else if (stackPeek(&userStack, &value))
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Top value: %d", value);
        }
        else
        {
            strcpy(stackMessage, "Stack is empty.");
        }
    }

    // Return to dashboard
    if (CheckCollisionPointRec(mouse, backButton))
    {
        stackInputActive = false;
        currentScreen = DASHBOARD;
    }
}

            // Back buttons on other module screens
            else
            {
                Rectangle backButton = {400, 450, 200, 65};

                if (CheckCollisionPointRec(mouse, backButton))
                {
                    currentScreen = DASHBOARD;
                }
            }
        }

        // ---------------------------------
        // DRAW THE CURRENT SCREEN
        // ---------------------------------
        BeginDrawing();

        if (currentScreen == DASHBOARD)
        {
            DrawDashboard();
        }
        else if (currentScreen == RECURSION_LAB)
        {
            if (recursionMode == TOWER_OF_HANOI)
            {
                DrawHanoiScreen();
            }
            else
            {
                DrawRecursionLab();
            }
        }
        
        
        else if (currentScreen == DATA_STRUCTURES_LAB)
        {
            DrawStackLab();
        }

        else if (currentScreen == SORTING_LAB)
        {
            DrawModuleScreen("SORTING LAB");
        }
        else if (currentScreen == COMPLEXITY_LAB)
        {
            DrawModuleScreen("TIME COMPLEXITY");
        }

        EndDrawing();

    } // End of main while loop

    CloseWindow();

    return 0;
}