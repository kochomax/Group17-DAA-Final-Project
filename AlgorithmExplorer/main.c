
#include "raylib.h"
#include "recursion.h"
#include "hanoi.h"
#include "stack.h"
#include "queue.h"
#include "linked_list.h"
#include "bracket_checker.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// =========================================
// APPLICATION SCREENS
// =========================================

typedef enum
{
    DASHBOARD,
    RECURSION_LAB,
    DATA_STRUCTURES_LAB,
    SORTING_LAB,
    COMPLEXITY_LAB
} Screen;

// =========================================
// RECURSION LAB SECTIONS
// =========================================

typedef enum
{
    FIBONACCI,
    FACTORIAL,
    TOWER_OF_HANOI
} RecursionMode;

// =========================================
// GLOBAL VARIABLES
// =========================================

// Application navigation
Screen currentScreen = DASHBOARD;
RecursionMode recursionMode = FIBONACCI;

// Recursion input
char inputText[16] = "";
int inputValue = 0;
bool inputActive = false;
bool resultReady = false;
char resultText[256] = "";

// Stack variables
Stack userStack;
bool stackInitialized = false;

int stackCapacityInput = 5;
char stackInput[16] = "";
bool stackInputActive = false;

char stackMessage[256] =
    "Create a stack by selecting its capacity.";

// Queue variables
Queue circularQueue;
LinearQueue linearQueue;

bool queueInitialized = false;
bool linearQueueInitialized = false;

int queueCapacityInput = 5;
int queueType = 0;
int dataStructureMode = 0;

char queueInput[16] = "";
bool queueInputActive = false;

char queueMessage[256] =
    "Select a queue type and create a queue.";

    // =========================================
// LINKED LIST VARIABLES
// =========================================

SinglyList singlyList;
DoublyList doublyList;

bool singlyListInitialized = false;
bool doublyListInitialized = false;

int linkedListType = 0;

char listValueInput[16] = "";
char listTargetInput[16] = "";

bool listValueActive = false;
bool listTargetActive = false;

char listMessage[256] =
    "Select a linked list type and create a list.";

// =========================================
// BRACKET CHECKER VARIABLES
// =========================================

char bracketExpression[BRACKET_MAX_LENGTH] = "";

bool bracketInputActive = false;
bool bracketResultReady = false;

BracketResult bracketResult;

// Tower of Hanoi variables
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

// =========================================
// DRAW BUTTON
// =========================================

void DrawButton(Rectangle button, const char *text)
{
    Vector2 mouse = GetMousePosition();

    bool hovering = CheckCollisionPointRec(mouse, button);

    Color buttonColor = hovering ? BLUE : DARKBLUE;

    DrawRectangleRounded(button, 0.2f, 10, buttonColor);

    int textWidth = MeasureText(text, 20);

    DrawText(
        text,
        (int)(button.x + (button.width - textWidth) / 2),
        (int)(button.y + (button.height - 20) / 2),
        20,
        WHITE
    );
}

// =========================================
// DASHBOARD
// =========================================

void DrawDashboard(void)
{
    ClearBackground(RAYWHITE);

    DrawText("ALGORITHM EXPLORER", 280, 65, 40, DARKBLUE);

    DrawText(
        "Learn. Explore. Understand Algorithms.",
        340, 125, 20, GRAY
    );

    DrawText("SELECT A MODULE", 425, 200, 22, DARKGRAY);

    DrawButton((Rectangle){150, 260, 320, 85}, "Recursion Lab");
    DrawButton((Rectangle){530, 260, 320, 85}, "Data Structures Lab");
    DrawButton((Rectangle){150, 375, 320, 85}, "Sorting Lab");
    DrawButton((Rectangle){530, 375, 320, 85}, "Time Complexity");
}

// =========================================
// RESET RECURSION INPUT
// =========================================

void ResetRecursionInput(void)
{
    inputText[0] = '\0';
    inputValue = 0;
    resultReady = false;
    resultText[0] = '\0';
    inputActive = false;
}

// =========================================
// RECURSION INPUT FIELD
// =========================================

void DrawInputField(void)
{
    DrawText("Enter a number:", 150, 260, 22, DARKGRAY);

    DrawRectangle(150, 300, 250, 50, WHITE);
    DrawRectangleLines(150, 300, 250, 50, BLUE);

    DrawText(inputText, 165, 312, 22, BLACK);

    DrawText(
        "Click the box and type a number.",
        150, 365, 16, GRAY
    );
}

// =========================================
// FIBONACCI CALCULATION
// =========================================

void CalculateFibonacci(void)
{
    int n = atoi(inputText);

    if (inputText[0] == '\0' || n < 0 || n > 30)
    {
        strcpy(
            resultText,
            "Enter a valid Fibonacci number from 0 to 30."
        );

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

    double start = GetTime();

    long long naiveResult =
        fibonacciNaive(n, &naiveCalls);

    double naiveTime =
        (GetTime() - start) * 1000000.0;

    start = GetTime();

    long long memoResult =
        fibonacciMemo(n, memo, &memoCalls);

    double memoTime =
        (GetTime() - start) * 1000000.0;

    start = GetTime();

    long long dpResult =
        fibonacciDP(n, &dpIterations);

    double dpTime =
        (GetTime() - start) * 1000000.0;

    snprintf(
        resultText,
        sizeof(resultText),
        "Fibonacci(%d) = %lld\n"
        "Naive: %lld calls | %.2f us\n"
        "Memoized: %lld calls | %.2f us\n"
        "Bottom-up DP: %lld iterations | %.2f us",
        n,
        naiveResult,
        naiveCalls,
        naiveTime,
        memoCalls,
        memoTime,
        dpIterations,
        dpTime
    );

    if (naiveResult != memoResult ||
        memoResult != dpResult)
    {
        strcpy(
            resultText,
            "Calculation verification failed."
        );
    }

    resultReady = true;
}

// =========================================
// FACTORIAL CALCULATION
// =========================================

void CalculateFactorial(void)
{
    int n = atoi(inputText);

    if (inputText[0] == '\0' || n < 0 || n > 20)
    {
        strcpy(
            resultText,
            "Enter a valid factorial number from 0 to 20."
        );

        resultReady = true;
        return;
    }

    inputValue = n;

    long long recursiveCalls = 0;
    long long iterativeIterations = 0;

    double start = GetTime();

    long long recursiveResult =
        factorialRecursive(n, &recursiveCalls);

    double recursiveTime =
        (GetTime() - start) * 1000000.0;

    start = GetTime();

    long long iterativeResult =
        factorialIterative(n, &iterativeIterations);

    double iterativeTime =
        (GetTime() - start) * 1000000.0;

    snprintf(
        resultText,
        sizeof(resultText),
        "%d! = %lld\n"
        "Recursive: %lld calls | %.2f us\n"
        "Iterative: %lld iterations | %.2f us",
        n,
        recursiveResult,
        recursiveCalls,
        recursiveTime,
        iterativeIterations,
        iterativeTime
    );

    if (recursiveResult != iterativeResult)
    {
        strcpy(
            resultText,
            "Calculation verification failed."
        );
    }

    resultReady = true;
}

// =========================================
// RECURSION LAB SCREEN
// =========================================

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

// =========================================
// TOWER OF HANOI SCREEN
// =========================================

void DrawHanoiScreen(void)
{
    ClearBackground(RAYWHITE);

    DrawText("TOWER OF HANOI", 350, 30, 32, DARKBLUE);

    DrawText(
        "Recursive disk movement demonstration",
        280, 80, 20, DARKGRAY
    );

    DrawText(
        TextFormat("Number of disks: %d", hanoiDisks),
        100, 140, 20, DARKGRAY
    );

    DrawText("Choose disks (1-8):", 100, 180, 18, DARKGRAY);

    DrawRectangle(300, 175, 100, 35, LIGHTGRAY);
    DrawRectangleLines(300, 175, 100, 35, DARKBLUE);

    DrawText(
        TextFormat("%d", hanoiDisks),
        335, 183, 20, DARKBLUE
    );

    DrawButton((Rectangle){420, 175, 70, 35}, "-");
    DrawButton((Rectangle){510, 175, 70, 35}, "+");

    DrawText(
        TextFormat("Moves: %d / %d", hanoiMovesMade, hanoiTotal),
        700, 140, 20, DARKGRAY
    );

    // Draw pegs
    DrawLineEx(
        (Vector2){250, 450},
        (Vector2){250, 230},
        8, DARKGRAY
    );

    DrawLineEx(
        (Vector2){500, 450},
        (Vector2){500, 230},
        8, DARKGRAY
    );

    DrawLineEx(
        (Vector2){750, 450},
        (Vector2){750, 230},
        8, DARKGRAY
    );

    // Draw base
    DrawLineEx(
        (Vector2){120, 450},
        (Vector2){880, 450},
        8, DARKGRAY
    );

    // Draw disks
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

// =========================================
// MOVE HANOI DISK
// =========================================

void MoveHanoiDisk(int from, int to)
{
    if (from < 0 || from > 2 || to < 0 || to > 2)
        return;

    if (pegCounts[from] <= 0 || pegCounts[to] >= 8)
        return;

    int disk = pegDisks[from][pegCounts[from] - 1];

    pegCounts[from]--;

    pegDisks[to][pegCounts[to]] = disk;

    pegCounts[to]++;

    hanoiMovesMade++;
}

// =========================================
// DRAW STACK LAB
// =========================================

void DrawStackLab(void)
{
    ClearBackground(RAYWHITE);

    DrawText("STACK LAB", 400, 30, 35, DARKBLUE);

    DrawButton(
        (Rectangle){700, 30, 180, 45},
        "QUEUE LAB"
    );

    DrawText("Array-Based Stack", 100, 100, 25, DARKGRAY);

    DrawText("Stack capacity:", 100, 155, 20, DARKGRAY);

    DrawText(
        TextFormat("%d", stackCapacityInput),
        280, 155, 20, DARKBLUE
    );

    DrawButton((Rectangle){330, 145, 50, 40}, "-");
    DrawButton((Rectangle){390, 145, 50, 40}, "+");

    DrawButton(
        (Rectangle){470, 140, 180, 50},
        "CREATE STACK"
    );

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
        DrawText(
            TextFormat(
                "Size: %d / %d",
                stackSize(&userStack),
                userStack.capacity
            ),
            100, 390, 20, DARKGRAY
        );

        DrawText(
            TextFormat(
                "Empty: %s",
                stackIsEmpty(&userStack) ? "YES" : "NO"
            ),
            350, 390, 20, DARKGRAY
        );

        DrawText(
            TextFormat(
                "Full: %s",
                stackIsFull(&userStack) ? "YES" : "NO"
            ),
            550, 390, 20, DARKGRAY
        );

        for (int i = 0; i <= userStack.top; i++)
        {
            DrawRectangle(
                100,
                430 - i * 35,
                180,
                30,
                SKYBLUE
            );

            DrawRectangleLines(
                100,
                430 - i * 35,
                180,
                30,
                DARKBLUE
            );

            DrawText(
                TextFormat("%d", userStack.items[i]),
                180,
                435 - i * 35,
                18,
                DARKBLUE
            );
        }
    }
    else
    {
        DrawText(
            "No stack created yet.",
            100, 390, 20, GRAY
        );
    }

    DrawText(stackMessage, 400, 370, 18, DARKGREEN);

    DrawButton((Rectangle){400, 550, 200, 55}, "BACK");
}

// =========================================
// DRAW QUEUE LAB
// =========================================

void DrawQueueLab(void)
{
    ClearBackground(RAYWHITE);

    DrawText("QUEUE LAB", 400, 30, 35, DARKBLUE);

    DrawButton(
        (Rectangle){700, 30, 180, 45},
        "BACK TO STACK"
    );

    DrawText("Select Queue Type:", 100, 110, 20, DARKGRAY);

    DrawButton(
        (Rectangle){100, 150, 200, 50},
        "LINEAR QUEUE"
    );

    DrawButton(
        (Rectangle){330, 150, 200, 50},
        "CIRCULAR QUEUE"
    );

    DrawText(
        TextFormat("Capacity: %d", queueCapacityInput),
        100, 225, 20, DARKGRAY
    );

    DrawButton((Rectangle){330, 215, 50, 40}, "-");
    DrawButton((Rectangle){390, 215, 50, 40}, "+");

    DrawButton(
        (Rectangle){470, 210, 180, 50},
        "CREATE QUEUE"
    );

    DrawText("Enter value:", 100, 290, 20, DARKGRAY);

    DrawRectangle(100, 320, 200, 45, WHITE);
    DrawRectangleLines(100, 320, 200, 45, BLUE);

    DrawText(queueInput, 115, 330, 20, BLACK);

    DrawButton((Rectangle){330, 315, 150, 50}, "ENQUEUE");
    DrawButton((Rectangle){500, 315, 150, 50}, "DEQUEUE");
    DrawButton((Rectangle){670, 315, 130, 50}, "FRONT");

    DrawText("QUEUE CONTENTS", 100, 400, 22, DARKBLUE);

    // Linear queue display
    if (queueType == 1 && linearQueueInitialized)
    {
        DrawText(
            TextFormat(
                "Size: %d / %d",
                linearQueue.size,
                linearQueue.capacity
            ),
            100, 440, 20, DARKGRAY
        );

        for (int i = linearQueue.front;
             i <= linearQueue.rear;
             i++)
        {
            DrawRectangle(
                100 + (i - linearQueue.front) * 75,
                490,
                65,
                40,
                SKYBLUE
            );

            DrawRectangleLines(
                100 + (i - linearQueue.front) * 75,
                490,
                65,
                40,
                DARKBLUE
            );

            DrawText(
                TextFormat("%d", linearQueue.items[i]),
                120 + (i - linearQueue.front) * 75,
                500,
                18,
                DARKBLUE
            );
        }
    }

    // Circular queue display
    else if (queueType == 2 && queueInitialized)
    {
        DrawText(
            TextFormat(
                "Size: %d / %d",
                circularQueue.size,
                circularQueue.capacity
            ),
            100, 440, 20, DARKGRAY
        );

        for (int i = 0; i < circularQueue.size; i++)
        {
            int index =
                (circularQueue.front + i) %
                circularQueue.capacity;

            DrawRectangle(
                100 + i * 75,
                490,
                65,
                40,
                SKYBLUE
            );

            DrawRectangleLines(
                100 + i * 75,
                490,
                65,
                40,
                DARKBLUE
            );

            DrawText(
                TextFormat("%d", circularQueue.items[index]),
                120 + i * 75,
                500,
                18,
                DARKBLUE
            );
        }

        DrawText(
            TextFormat(
                "Front: %d  Rear: %d",
                circularQueue.front,
                circularQueue.rear
            ),
            100, 550, 20, DARKGRAY
        );
    }
    else
    {
        DrawText(
            "No queue created yet.",
            100, 440, 20, GRAY
        );
    }

    DrawText(queueMessage, 400, 400, 18, DARKGREEN);

    DrawButton((Rectangle){400, 620, 200, 50}, "BACK");
}

// =========================================
// PLACEHOLDER MODULE SCREENS
// =========================================

void DrawModuleScreen(const char *title)
{
    ClearBackground(RAYWHITE);

    DrawText(title, 300, 100, 35, DARKBLUE);

    DrawText(
        "Module content will be developed here.",
        300, 200, 22, DARKGRAY
    );

    DrawButton((Rectangle){400, 450, 200, 65}, "BACK");
}

// =========================================
// INITIALISE HANOI
// =========================================

void InitialiseHanoi(void)
{
    hanoiMovesMade = 0;
    hanoiCurrentMove = 0;
    hanoiMoveCount = 0;

    hanoiTotal = hanoiTotalMoves(hanoiDisks);

    generateHanoiMoves(
        hanoiDisks,
        0,
        1,
        2,
        hanoiMoves,
        &hanoiMoveCount
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

// =========================================
// MAIN FUNCTION
// =========================================

int main(void)
{
    InitWindow(1000, 650, "Algorithm Explorer");

    if (!IsWindowReady())
    {
        return 1;
    }

    SetTargetFPS(60);

    // =====================================
    // MAIN APPLICATION LOOP
    // =====================================

    while (!WindowShouldClose())
    {
        // ---------------------------------
        // AUTOMATIC TOWER OF HANOI MOVEMENT
        // ---------------------------------

        if (currentScreen == RECURSION_LAB &&
            recursionMode == TOWER_OF_HANOI &&
            hanoiRunning &&
            !hanoiPaused)
        {
            hanoiMoveTimer += GetFrameTime();

            if (hanoiMoveTimer >= hanoiMoveSpeed)
            {
                if (hanoiCurrentMove < hanoiMoveCount)
                {
                    HanoiMove move =
                        hanoiMoves[hanoiCurrentMove];

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

        // =================================
        // KEYBOARD INPUT
        // =================================

        // Recursion input
        if (currentScreen == RECURSION_LAB &&
            recursionMode != TOWER_OF_HANOI &&
            inputActive)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                if (key >= '0' &&
                    key <= '9' &&
                    strlen(inputText) < 2)
                {
                    int length = (int)strlen(inputText);

                    inputText[length] = (char)key;
                    inputText[length + 1] = '\0';

                    resultReady = false;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                int length = (int)strlen(inputText);

                if (length > 0)
                {
                    inputText[length - 1] = '\0';
                    resultReady = false;
                }
            }
        }

        // Stack input
        if (currentScreen == DATA_STRUCTURES_LAB &&
            dataStructureMode == 0 &&
            stackInputActive)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                if (((key >= '0' && key <= '9') ||
                     (key == '-' && strlen(stackInput) == 0)) &&
                    strlen(stackInput) < 14)
                {
                    int length = (int)strlen(stackInput);

                    stackInput[length] = (char)key;
                    stackInput[length + 1] = '\0';
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                int length = (int)strlen(stackInput);

                if (length > 0)
                {
                    stackInput[length - 1] = '\0';
                }
            }
        }

        // Queue input
        if (currentScreen == DATA_STRUCTURES_LAB &&
            dataStructureMode == 1 &&
            queueInputActive)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                if (((key >= '0' && key <= '9') ||
                     (key == '-' && strlen(queueInput) == 0)) &&
                    strlen(queueInput) < 14)
                {
                    int length = (int)strlen(queueInput);

                    queueInput[length] = (char)key;
                    queueInput[length + 1] = '\0';
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                int length = (int)strlen(queueInput);

                if (length > 0)
                {
                    queueInput[length - 1] = '\0';
                }
            }
        }

        // =================================
        // HANDLE MOUSE CLICKS
        // =================================

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // ---------------------------------
            // DASHBOARD CONTROLS
            // ---------------------------------

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

                        stackInputActive = false;
                        queueInputActive = false;
                    }
                }
            }

            // ---------------------------------
            // TOWER OF HANOI CONTROLS
            // ---------------------------------

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

                Rectangle backButton = {400, 590, 200, 45};

                // Change disk count
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

                    InitialiseHanoi();
                }

                // Speed controls
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

                // Start
                if (CheckCollisionPointRec(mouse, startButton))
                {
                    if (hanoiCurrentMove < hanoiMoveCount)
                    {
                        hanoiRunning = true;
                        hanoiPaused = false;
                    }
                }

                // Pause
                if (CheckCollisionPointRec(mouse, pauseButton))
                {
                    hanoiPaused = true;
                    hanoiRunning = false;
                }

                // Step
                if (CheckCollisionPointRec(mouse, stepButton))
                {
                    if (hanoiCurrentMove < hanoiMoveCount)
                    {
                        HanoiMove move =
                            hanoiMoves[hanoiCurrentMove];

                        MoveHanoiDisk(move.from, move.to);

                        hanoiCurrentMove++;
                    }
                }

                // Reset
                if (CheckCollisionPointRec(mouse, resetButton))
                {
                    InitialiseHanoi();
                }

                // Back
                if (CheckCollisionPointRec(mouse, backButton))
                {
                    currentScreen = RECURSION_LAB;
                    recursionMode = FIBONACCI;
                    ResetRecursionInput();
                }
            }

            // ---------------------------------
            // RECURSION LAB CONTROLS
            // ---------------------------------

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

                    InitialiseHanoi();

                    resultReady = false;
                    inputActive = false;
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
            // DATA STRUCTURES LAB CONTROLS
            // ---------------------------------

            else if (currentScreen == DATA_STRUCTURES_LAB)
            {
                // =================================
                // STACK CONTROLS
                // =================================

                if (dataStructureMode == 0)
                {
                    Rectangle queueLabButton = {700, 30, 180, 45};

                    Rectangle minusButton = {330, 145, 50, 40};
                    Rectangle plusButton = {390, 145, 50, 40};

                    Rectangle createButton = {470, 140, 180, 50};

                    Rectangle inputBox = {100, 265, 200, 45};

                    Rectangle pushButton = {330, 260, 130, 50};
                    Rectangle popButton = {480, 260, 130, 50};
                    Rectangle peekButton = {630, 260, 130, 50};

                    Rectangle backButton = {400, 550, 200, 55};

                    // Open Queue Lab
                    if (CheckCollisionPointRec(mouse, queueLabButton))
                    {
                        dataStructureMode = 1;
                        stackInputActive = false;
                    }
                    else
                    {
                        stackInputActive =
                            CheckCollisionPointRec(mouse, inputBox);
                    }

                    // Capacity minus
                    if (CheckCollisionPointRec(mouse, minusButton) &&
                        stackCapacityInput > 1)
                    {
                        stackCapacityInput--;
                    }

                    // Capacity plus
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

                            strcpy(
                                stackMessage,
                                "Stack created successfully!"
                            );
                        }
                        else
                        {
                            strcpy(
                                stackMessage,
                                "Unable to create stack."
                            );
                        }
                    }

                    // PUSH
                    if (CheckCollisionPointRec(mouse, pushButton))
                    {
                        char *end;
                        long value = strtol(stackInput, &end, 10);

                        if (!stackInitialized)
                        {
                            strcpy(stackMessage, "Create a stack first.");
                        }
                        else if (stackInput[0] == '\0' ||
                                 *end != '\0' ||
                                 value < INT_MIN ||
                                 value > INT_MAX)
                        {
                            strcpy(stackMessage, "Enter a valid integer.");
                        }
                        else if (stackPush(&userStack, (int)value))
                        {
                            strcpy(
                                stackMessage,
                                "Value pushed successfully!"
                            );

                            stackInput[0] = '\0';
                        }
                        else
                        {
                            strcpy(
                                stackMessage,
                                "Stack overflow! Stack is full."
                            );
                        }
                    }

                    // POP
                    if (CheckCollisionPointRec(mouse, popButton))
                    {
                        int value;

                        if (!stackInitialized)
                        {
                            strcpy(stackMessage, "Create a stack first.");
                        }
                        else if (stackPop(&userStack, &value))
                        {
                            snprintf(
                                stackMessage,
                                sizeof(stackMessage),
                                "Popped value: %d",
                                value
                            );
                        }
                        else
                        {
                            strcpy(
                                stackMessage,
                                "Stack underflow! Stack is empty."
                            );
                        }
                    }

                    // PEEK
                    if (CheckCollisionPointRec(mouse, peekButton))
                    {
                        int value;

                        if (!stackInitialized)
                        {
                            strcpy(stackMessage, "Create a stack first.");
                        }
                        else if (stackPeek(&userStack, &value))
                        {
                            snprintf(
                                stackMessage,
                                sizeof(stackMessage),
                                "Top value: %d",
                                value
                            );
                        }
                        else
                        {
                            strcpy(stackMessage, "Stack is empty.");
                        }
                    }

                    // Back to dashboard
                    if (CheckCollisionPointRec(mouse, backButton))
                    {
                        currentScreen = DASHBOARD;
                        stackInputActive = false;
                    }
                }

                // =================================
                // QUEUE CONTROLS
                // =================================

                else
                {
                    Rectangle linearButton = {100, 150, 200, 50};
                    Rectangle circularButton = {330, 150, 200, 50};

                    Rectangle minusButton = {330, 215, 50, 40};
                    Rectangle plusButton = {390, 215, 50, 40};

                    Rectangle createButton = {470, 210, 180, 50};

                    Rectangle inputBox = {100, 320, 200, 45};

                    Rectangle enqueueButton = {330, 315, 150, 50};
                    Rectangle dequeueButton = {500, 315, 150, 50};
                    Rectangle frontButton = {670, 315, 130, 50};

                    Rectangle backButton = {700, 30, 180, 45};

                    // Back to Stack Lab
                    if (CheckCollisionPointRec(mouse, backButton))
                    {
                        dataStructureMode = 0;
                        queueInputActive = false;
                    }
                    else
                    {
                        queueInputActive =
                            CheckCollisionPointRec(mouse, inputBox);
                    }

                    // Select linear queue
                    if (CheckCollisionPointRec(mouse, linearButton))
                    {
                        queueType = 1;
                    }

                    // Select circular queue
                    if (CheckCollisionPointRec(mouse, circularButton))
                    {
                        queueType = 2;
                    }

                    // Capacity minus
                    if (CheckCollisionPointRec(mouse, minusButton) &&
                        queueCapacityInput > 1)
                    {
                        queueCapacityInput--;
                    }

                    // Capacity plus
                    if (CheckCollisionPointRec(mouse, plusButton) &&
                        queueCapacityInput < 8)
                    {
                        queueCapacityInput++;
                    }

                    // Create queue
                    if (CheckCollisionPointRec(mouse, createButton))
                    {
                        if (queueInitialized)
                        {
                            queueFree(&circularQueue);
                            queueInitialized = false;
                        }

                        if (linearQueueInitialized)
                        {
                            linearQueueFree(&linearQueue);
                            linearQueueInitialized = false;
                        }

                        if (queueType == 1)
                        {
                            if (linearQueueInit(
                                &linearQueue,
                                queueCapacityInput
                            ))
                            {
                                linearQueueInitialized = true;

                                strcpy(
                                    queueMessage,
                                    "Linear queue created!"
                                );
                            }
                            else
                            {
                                strcpy(
                                    queueMessage,
                                    "Queue creation failed."
                                );
                            }
                        }
                        else if (queueType == 2)
                        {
                            if (queueInit(
                                &circularQueue,
                                queueCapacityInput
                            ))
                            {
                                queueInitialized = true;

                                strcpy(
                                    queueMessage,
                                    "Circular queue created!"
                                );
                            }
                            else
                            {
                                strcpy(
                                    queueMessage,
                                    "Queue creation failed."
                                );
                            }
                        }
                        else
                        {
                            strcpy(
                                queueMessage,
                                "Select a queue type first."
                            );
                        }

                        queueInput[0] = '\0';
                    }

                    // ENQUEUE
                    if (CheckCollisionPointRec(mouse, enqueueButton))
                    {
                        char *end;
                        long value = strtol(queueInput, &end, 10);

                        if (queueInput[0] == '\0' ||
                            *end != '\0' ||
                            value < INT_MIN ||
                            value > INT_MAX)
                        {
                            strcpy(
                                queueMessage,
                                "Enter a valid integer."
                            );
                        }
                        else if (queueType == 1 &&
                                 linearQueueInitialized)
                        {
                            if (linearEnqueue(
                                &linearQueue,
                                (int)value
                            ))
                            {
                                strcpy(
                                    queueMessage,
                                    "Value enqueued!"
                                );

                                queueInput[0] = '\0';
                            }
                            else
                            {
                                strcpy(
                                    queueMessage,
                                    "Linear queue is full!"
                                );
                            }
                        }
                        else if (queueType == 2 &&
                                 queueInitialized)
                        {
                            if (enqueue(
                                &circularQueue,
                                (int)value
                            ))
                            {
                                strcpy(
                                    queueMessage,
                                    "Value enqueued!"
                                );

                                queueInput[0] = '\0';
                            }
                            else
                            {
                                strcpy(
                                    queueMessage,
                                    "Circular queue is full!"
                                );
                            }
                        }
                        else
                        {
                            strcpy(
                                queueMessage,
                                "Create a queue first."
                            );
                        }
                    }

                    // DEQUEUE
                    if (CheckCollisionPointRec(mouse, dequeueButton))
                    {
                        int value;
                        int success = 0;

                        if (queueType == 1 &&
                            linearQueueInitialized)
                        {
                            success = linearDequeue(
                                &linearQueue,
                                &value
                            );
                        }
                        else if (queueType == 2 &&
                                 queueInitialized)
                        {
                            success = dequeue(
                                &circularQueue,
                                &value
                            );
                        }

                        if (success)
                        {
                            snprintf(
                                queueMessage,
                                sizeof(queueMessage),
                                "Dequeued: %d",
                                value
                            );
                        }
                        else
                        {
                            strcpy(
                                queueMessage,
                                "Queue is empty or not created."
                            );
                        }
                    }

                    // FRONT
                    if (CheckCollisionPointRec(mouse, frontButton))
                    {
                        int value;
                        int success = 0;

                        if (queueType == 1 &&
                            linearQueueInitialized &&
                            linearQueue.size > 0)
                        {
                            value =
                                linearQueue.items[linearQueue.front];

                            success = 1;
                        }
                        else if (queueType == 2 &&
                                 queueInitialized)
                        {
                            success = queueFront(
                                &circularQueue,
                                &value
                            );
                        }

                        if (success)
                        {
                            snprintf(
                                queueMessage,
                                sizeof(queueMessage),
                                "Front value: %d",
                                value
                            );
                        }
                        else
                        {
                            strcpy(
                                queueMessage,
                                "Queue is empty or not created."
                            );
                        }
                    }
                }
            }

            // ---------------------------------
            // OTHER MODULE BACK BUTTONS
            // ---------------------------------

            else
            {
                Rectangle backButton = {400, 450, 200, 65};

                if (CheckCollisionPointRec(mouse, backButton))
                {
                    currentScreen = DASHBOARD;
                }
            }
        }

        // =================================
        // DRAW CURRENT SCREEN
        // =================================

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
            if (dataStructureMode == 0)
            {
                DrawStackLab();
            }
            else
            {
                DrawQueueLab();
            }
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

    // =================================
    // FREE MEMORY
    // =================================

    if (stackInitialized)
    {
        stackFree(&userStack);
    }

    if (queueInitialized)
    {
        queueFree(&circularQueue);
    }

    if (linearQueueInitialized)
    {
        linearQueueFree(&linearQueue);
    }

    CloseWindow();

    return 0;
}
