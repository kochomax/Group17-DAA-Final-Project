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
// APPLICATION SCREENS AND MODES
// =========================================

typedef enum
{
    DASHBOARD,
    RECURSION_LAB,
    DATA_STRUCTURES_LAB,
    SORTING_LAB,
    COMPLEXITY_LAB
} Screen;

typedef enum
{
    FIBONACCI,
    FACTORIAL,
    TOWER_OF_HANOI
} RecursionMode;

typedef enum
{
    STACK_MODE,
    QUEUE_MODE,
    MORE_MODULES_MENU,
    SINGLY_LIST_MODE,
    DOUBLY_LIST_MODE,
    BRACKET_CHECKER_MODE
} DataStructureMode;

// =========================================
// GLOBAL APPLICATION STATE
// =========================================

static Screen currentScreen = DASHBOARD;
static RecursionMode recursionMode = FIBONACCI;
static DataStructureMode dataStructureMode = STACK_MODE;

// Recursion input and results
static char inputText[16] = "";
static bool inputActive = false;
static bool resultReady = false;
static char resultText[256] = "";

// Stack
static Stack userStack;
static bool stackInitialized = false;
static int stackCapacityInput = 5;
static char stackInput[16] = "";
static bool stackInputActive = false;
static char stackMessage[256] =
    "Create a stack by selecting its capacity.";

// Queues
static Queue circularQueue;
static LinearQueue linearQueue;
static bool queueInitialized = false;
static bool linearQueueInitialized = false;
static int queueCapacityInput = 5;
static int queueType = 0;
static char queueInput[16] = "";
static bool queueInputActive = false;
static char queueMessage[256] =
    "Select a queue type and create a queue.";

// Linked lists
static SinglyList singlyList;
static DoublyList doublyList;
static char listValueInput[16] = "";
static char listTargetInput[16] = "";
static bool listValueActive = false;
static bool listTargetActive = false;
static char listMessage[256] =
    "Enter values to perform linked-list operations.";

// Bracket checker
static char bracketExpression[BRACKET_MAX_LENGTH] = "";
static bool bracketInputActive = false;
static bool bracketResultReady = false;
static int bracketScroll = 0;
static BracketResult bracketResult;

// Tower of Hanoi
static int hanoiDisks = 3;
static int hanoiMovesMade = 0;
static int hanoiTotal = 0;
static bool hanoiRunning = false;
static bool hanoiPaused = false;
static double hanoiMoveTimer = 0.0;
static float hanoiMoveSpeed = 1.0f;
static HanoiMove hanoiMoves[MAX_HANOI_MOVES];
static int hanoiMoveCount = 0;
static int hanoiCurrentMove = 0;
static int pegDisks[3][8];
static int pegCounts[3] = {0, 0, 0};

// =========================================
// SHARED HELPERS
// =========================================

static void DrawButton(Rectangle button, const char *label)
{
    Vector2 mouse = GetMousePosition();
    bool hovering = CheckCollisionPointRec(mouse, button);
    Color fill = hovering ? BLUE : DARKBLUE;

    DrawRectangleRounded(button, 0.2f, 10, fill);

    int textWidth = MeasureText(label, 20);

    DrawText(
        label,
        (int)(button.x + (button.width - textWidth) / 2),
        (int)(button.y + (button.height - 20) / 2),
        20,
        WHITE
    );
}

static int ParseInteger(const char *text, int *value)
{
    char *end = NULL;
    long parsed;

    if (text == NULL || text[0] == '\0' || value == NULL)
        return 0;

    parsed = strtol(text, &end, 10);

    if (end == text || *end != '\0' ||
        parsed < INT_MIN || parsed > INT_MAX)
        return 0;

    *value = (int)parsed;
    return 1;
}

static void HandleIntegerTyping(char *buffer, size_t capacity)
{
    int key = GetCharPressed();

    while (key > 0)
    {
        size_t length = strlen(buffer);

        if (((key >= '0' && key <= '9') ||
             (key == '-' && length == 0)) &&
            length + 1 < capacity)
        {
            buffer[length] = (char)key;
            buffer[length + 1] = '\0';
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        size_t length = strlen(buffer);

        if (length > 0)
            buffer[length - 1] = '\0';
    }
}

static void ResetRecursionInput(void)
{
    inputText[0] = '\0';
    resultReady = false;
    resultText[0] = '\0';
    inputActive = false;
}

static void ResetDataStructureInputFocus(void)
{
    stackInputActive = false;
    queueInputActive = false;
    listValueActive = false;
    listTargetActive = false;
    bracketInputActive = false;
}

// =========================================
// DASHBOARD
// =========================================

static void DrawDashboard(void)
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
// RECURSION LAB
// =========================================

static void DrawInputField(void)
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

static void CalculateFibonacci(void)
{
    int n;
    long long naiveCalls = 0;
    long long memoCalls = 0;
    long long dpIterations = 0;
    long long memo[31];

    double start;
    double naiveTime;
    double memoTime;
    double dpTime;

    long long naiveResult;
    long long memoResult;
    long long dpResult;

    if (!ParseInteger(inputText, &n) || n < 0 || n > 30)
    {
        snprintf(
            resultText,
            sizeof(resultText),
            "Enter a valid Fibonacci number from 0 to 30."
        );

        resultReady = true;
        return;
    }

    for (int i = 0; i <= 30; i++)
        memo[i] = -1;

    start = GetTime();
    naiveResult = fibonacciNaive(n, &naiveCalls);
    naiveTime = (GetTime() - start) * 1000000.0;

    start = GetTime();
    memoResult = fibonacciMemo(n, memo, &memoCalls);
    memoTime = (GetTime() - start) * 1000000.0;

    start = GetTime();
    dpResult = fibonacciDP(n, &dpIterations);
    dpTime = (GetTime() - start) * 1000000.0;

    if (naiveResult != memoResult || memoResult != dpResult)
    {
        snprintf(
            resultText,
            sizeof(resultText),
            "Calculation verification failed."
        );
    }
    else
    {
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
    }

    resultReady = true;
}

static void CalculateFactorial(void)
{
    int n;

    long long recursiveCalls = 0;
    long long iterativeIterations = 0;

    long long recursiveResult;
    long long iterativeResult;

    double start;
    double recursiveTime;
    double iterativeTime;

    if (!ParseInteger(inputText, &n) || n < 0 || n > 20)
    {
        snprintf(
            resultText,
            sizeof(resultText),
            "Enter a valid factorial number from 0 to 20."
        );

        resultReady = true;
        return;
    }

    start = GetTime();
    recursiveResult = factorialRecursive(n, &recursiveCalls);
    recursiveTime = (GetTime() - start) * 1000000.0;

    start = GetTime();
    iterativeResult = factorialIterative(n, &iterativeIterations);
    iterativeTime = (GetTime() - start) * 1000000.0;

    if (recursiveResult != iterativeResult)
    {
        snprintf(
            resultText,
            sizeof(resultText),
            "Calculation verification failed."
        );
    }
    else
    {
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
    }

    resultReady = true;
}

static void InitialiseHanoi(void)
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

static void MoveHanoiDisk(int from, int to)
{
    int disk;

    if (from < 0 || from > 2 || to < 0 || to > 2)
        return;

    if (pegCounts[from] <= 0 || pegCounts[to] >= 8)
        return;

    disk = pegDisks[from][pegCounts[from] - 1];

    pegCounts[from]--;

    pegDisks[to][pegCounts[to]] = disk;

    pegCounts[to]++;

    hanoiMovesMade++;
}

static void DrawHanoiScreen(void)
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

    DrawLineEx(
        (Vector2){120, 450},
        (Vector2){880, 450},
        8, DARKGRAY
    );

    for (int peg = 0; peg < 3; peg++)
    {
        for (int i = 0; i < pegCounts[peg]; i++)
        {
            int diskSize = pegDisks[peg][i];

            float diskWidth = 50 + diskSize * 35;
            float x = 250 + peg * 250 - diskWidth / 2;
            float y = 450 - (i + 1) * 20;

            DrawRectangle(
                (int)x, (int)y,
                (int)diskWidth, 20,
                BLUE
            );

            DrawRectangleLines(
                (int)x, (int)y,
                (int)diskWidth, 20,
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

static void DrawRecursionLab(void)
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
// STACK LAB
// =========================================

static void DrawStackLab(void)
{
    ClearBackground(RAYWHITE);

    DrawText("STACK LAB", 400, 30, 35, DARKBLUE);

    DrawButton((Rectangle){700, 30, 180, 45}, "QUEUE LAB");
    DrawButton((Rectangle){490, 30, 180, 45}, "MORE MODULES");

    DrawText("Array-Based Stack", 100, 100, 25, DARKGRAY);
    DrawText("Stack capacity:", 100, 155, 20, DARKGRAY);

    DrawText(
        TextFormat("%d", stackCapacityInput),
        280, 155, 20, DARKBLUE
    );

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
            int y = 430 - i * 35;

            DrawRectangle(100, y, 180, 30, SKYBLUE);
            DrawRectangleLines(100, y, 180, 30, DARKBLUE);

            DrawText(
                TextFormat("%d", userStack.items[i]),
                180, y + 5, 18, DARKBLUE
            );
        }
    }
    else
    {
        DrawText("No stack created yet.", 100, 390, 20, GRAY);
    }

    DrawText(stackMessage, 400, 370, 18, DARKGREEN);

    DrawButton((Rectangle){400, 550, 200, 55}, "BACK");
}

static void HandleStackClicks(Vector2 mouse)
{
    Rectangle queueButton = {700, 30, 180, 45};
    Rectangle moreButton = {490, 30, 180, 45};
    Rectangle minusButton = {330, 145, 50, 40};
    Rectangle plusButton = {390, 145, 50, 40};
    Rectangle createButton = {470, 140, 180, 50};
    Rectangle inputBox = {100, 265, 200, 45};
    Rectangle pushButton = {330, 260, 130, 50};
    Rectangle popButton = {480, 260, 130, 50};
    Rectangle peekButton = {630, 260, 130, 50};
    Rectangle backButton = {400, 550, 200, 55};

    if (CheckCollisionPointRec(mouse, queueButton))
    {
        dataStructureMode = QUEUE_MODE;
        stackInputActive = false;
        return;
    }

    if (CheckCollisionPointRec(mouse, moreButton))
    {
        dataStructureMode = MORE_MODULES_MENU;
        stackInputActive = false;
        return;
    }

    stackInputActive = CheckCollisionPointRec(mouse, inputBox);

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

            snprintf(
                stackMessage,
                sizeof(stackMessage),
                "Stack created successfully!"
            );
        }
        else
        {
            snprintf(
                stackMessage,
                sizeof(stackMessage),
                "Unable to create stack."
            );
        }
    }

    if (CheckCollisionPointRec(mouse, pushButton))
    {
        int value;

        if (!stackInitialized)
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Create a stack first.");
        }
        else if (!ParseInteger(stackInput, &value))
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Enter a valid integer.");
        }
        else if (stackPush(&userStack, value))
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Value pushed successfully!");
            stackInput[0] = '\0';
        }
        else
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Stack overflow! Stack is full.");
        }
    }

    if (CheckCollisionPointRec(mouse, popButton))
    {
        int value;

        if (!stackInitialized)
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Create a stack first.");
        }
        else if (stackPop(&userStack, &value))
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Popped value: %d", value);
        }
        else
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Stack underflow! Stack is empty.");
        }
    }

    if (CheckCollisionPointRec(mouse, peekButton))
    {
        int value;

        if (!stackInitialized)
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Create a stack first.");
        }
        else if (stackPeek(&userStack, &value))
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Top value: %d", value);
        }
        else
        {
            snprintf(stackMessage, sizeof(stackMessage),
                     "Stack is empty.");
        }
    }

    if (CheckCollisionPointRec(mouse, backButton))
    {
        currentScreen = DASHBOARD;
        stackInputActive = false;
    }
}

// =========================================
// QUEUE LAB
// =========================================

static void DrawQueueLab(void)
{
    ClearBackground(RAYWHITE);

    DrawText("QUEUE LAB", 400, 30, 35, DARKBLUE);

    DrawButton((Rectangle){700, 30, 180, 45}, "BACK TO STACK");

    DrawText("Select Queue Type:", 100, 110, 20, DARKGRAY);

    DrawButton((Rectangle){100, 150, 200, 50}, "LINEAR QUEUE");
    DrawButton((Rectangle){330, 150, 200, 50}, "CIRCULAR QUEUE");

    DrawText(
        TextFormat("Capacity: %d", queueCapacityInput),
        100, 225, 20, DARKGRAY
    );

    DrawButton((Rectangle){330, 215, 50, 40}, "-");
    DrawButton((Rectangle){390, 215, 50, 40}, "+");
    DrawButton((Rectangle){470, 210, 180, 50}, "CREATE QUEUE");

    DrawText("Enter value:", 100, 290, 20, DARKGRAY);

    DrawRectangle(100, 320, 200, 45, WHITE);
    DrawRectangleLines(100, 320, 200, 45, BLUE);

    DrawText(queueInput, 115, 330, 20, BLACK);

    DrawButton((Rectangle){330, 315, 150, 50}, "ENQUEUE");
    DrawButton((Rectangle){500, 315, 150, 50}, "DEQUEUE");
    DrawButton((Rectangle){670, 315, 130, 50}, "FRONT");

    DrawText("QUEUE CONTENTS", 100, 400, 22, DARKBLUE);

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

        DrawText(
            TextFormat(
                "Front index: %d   Rear index: %d",
                linearQueue.size == 0 ? -1 : linearQueue.front,
                linearQueue.size == 0 ? -1 : linearQueue.rear
            ),
            400, 440, 18, DARKGRAY
        );

        for (int i = linearQueue.front;
             i <= linearQueue.rear && linearQueue.size > 0;
             i++)
        {
            int x = 100 + (i - linearQueue.front) * 75;

            DrawRectangle(x, 490, 65, 40, SKYBLUE);
            DrawRectangleLines(x, 490, 65, 40, DARKBLUE);

            DrawText(
                TextFormat("%d", linearQueue.items[i]),
                x + 20, 500, 18, DARKBLUE
            );
        }
    }
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

        DrawText(
            TextFormat(
                "Front index: %d   Rear index: %d",
                circularQueue.size == 0 ? -1 : circularQueue.front,
                circularQueue.size == 0 ? -1 : circularQueue.rear
            ),
            400, 440, 18, DARKGRAY
        );

        for (int i = 0; i < circularQueue.size; i++)
        {
            int index =
                (circularQueue.front + i) % circularQueue.capacity;

            int x = 100 + i * 75;

            DrawRectangle(x, 490, 65, 40, SKYBLUE);
            DrawRectangleLines(x, 490, 65, 40, DARKBLUE);

            DrawText(
                TextFormat("%d", circularQueue.items[index]),
                x + 20, 500, 18, DARKBLUE
            );
        }
    }
    else
    {
        DrawText("No queue created yet.", 100, 440, 20, GRAY);
    }

    DrawText(queueMessage, 100, 565, 18, DARKGREEN);

    DrawText(
        "Linear queues can become false-full; circular queues reuse freed slots.",
        100, 595, 15, GRAY
    );
}

static void HandleQueueClicks(Vector2 mouse)
{
    Rectangle backButton = {700, 30, 180, 45};
    Rectangle linearButton = {100, 150, 200, 50};
    Rectangle circularButton = {330, 150, 200, 50};
    Rectangle minusButton = {330, 215, 50, 40};
    Rectangle plusButton = {390, 215, 50, 40};
    Rectangle createButton = {470, 210, 180, 50};
    Rectangle inputBox = {100, 320, 200, 45};
    Rectangle enqueueButton = {330, 315, 150, 50};
    Rectangle dequeueButton = {500, 315, 150, 50};
    Rectangle frontButton = {670, 315, 130, 50};

    if (CheckCollisionPointRec(mouse, backButton))
    {
        dataStructureMode = STACK_MODE;
        queueInputActive = false;
        return;
    }

    queueInputActive = CheckCollisionPointRec(mouse, inputBox);

    if (CheckCollisionPointRec(mouse, linearButton))
        queueType = 1;
    else if (CheckCollisionPointRec(mouse, circularButton))
        queueType = 2;

    if (CheckCollisionPointRec(mouse, minusButton) &&
        queueCapacityInput > 1)
    {
        queueCapacityInput--;
    }

    if (CheckCollisionPointRec(mouse, plusButton) &&
        queueCapacityInput < 8)
    {
        queueCapacityInput++;
    }

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
            if (linearQueueInit(&linearQueue, queueCapacityInput))
            {
                linearQueueInitialized = true;

                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Linear queue created!"
                );
            }
            else
            {
                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Queue creation failed."
                );
            }
        }
        else if (queueType == 2)
        {
            if (queueInit(&circularQueue, queueCapacityInput))
            {
                queueInitialized = true;

                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Circular queue created!"
                );
            }
            else
            {
                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Queue creation failed."
                );
            }
        }
        else
        {
            snprintf(
                queueMessage,
                sizeof(queueMessage),
                "Select a queue type first."
            );
        }

        queueInput[0] = '\0';
    }

    if (CheckCollisionPointRec(mouse, enqueueButton))
    {
        int value;

        if (!ParseInteger(queueInput, &value))
        {
            snprintf(
                queueMessage,
                sizeof(queueMessage),
                "Enter a valid integer."
            );
        }
        else if (queueType == 1 && linearQueueInitialized)
        {
            if (linearEnqueue(&linearQueue, value))
            {
                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Value enqueued!"
                );

                queueInput[0] = '\0';
            }
            else
            {
                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Linear queue is full!"
                );
            }
        }
        else if (queueType == 2 && queueInitialized)
        {
            if (enqueue(&circularQueue, value))
            {
                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Value enqueued!"
                );

                queueInput[0] = '\0';
            }
            else
            {
                snprintf(
                    queueMessage,
                    sizeof(queueMessage),
                    "Circular queue is full!"
                );
            }
        }
        else
        {
            snprintf(
                queueMessage,
                sizeof(queueMessage),
                "Create a queue first."
            );
        }
    }

    if (CheckCollisionPointRec(mouse, dequeueButton))
    {
        int value;
        int success = 0;

        if (queueType == 1 && linearQueueInitialized)
            success = linearDequeue(&linearQueue, &value);
        else if (queueType == 2 && queueInitialized)
            success = dequeue(&circularQueue, &value);

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
            snprintf(
                queueMessage,
                sizeof(queueMessage),
                "Queue is empty or not created."
            );
        }
    }

    if (CheckCollisionPointRec(mouse, frontButton))
    {
        int value;
        int success = 0;

        if (queueType == 1 &&
            linearQueueInitialized &&
            linearQueue.size > 0)
        {
            value = linearQueue.items[linearQueue.front];
            success = 1;
        }
        else if (queueType == 2 && queueInitialized)
        {
            success = queueFront(&circularQueue, &value);
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
            snprintf(
                queueMessage,
                sizeof(queueMessage),
                "Queue is empty or not created."
            );
        }
    }
}

// =========================================
// MORE MODULES MENU
// =========================================

static void DrawMoreModulesMenu(void)
{
    ClearBackground(RAYWHITE);

    DrawText("MORE DATA STRUCTURES", 280, 60, 35, DARKBLUE);

    DrawText(
        "Select a module to explore:",
        350, 125, 20, DARKGRAY
    );

    DrawButton(
        (Rectangle){150, 190, 320, 80},
        "SINGLY LINKED LIST"
    );

    DrawButton(
        (Rectangle){530, 190, 320, 80},
        "DOUBLY LINKED LIST"
    );

    DrawButton(
        (Rectangle){150, 310, 700, 80},
        "BRACKET BALANCE CHECKER"
    );

    DrawButton(
        (Rectangle){400, 460, 200, 60},
        "BACK"
    );
}

static void HandleMoreModulesClicks(Vector2 mouse)
{
    Rectangle singlyButton = {150, 190, 320, 80};
    Rectangle doublyButton = {530, 190, 320, 80};
    Rectangle bracketButton = {150, 310, 700, 80};
    Rectangle backButton = {400, 460, 200, 60};

    if (CheckCollisionPointRec(mouse, singlyButton))
    {
        dataStructureMode = SINGLY_LIST_MODE;
        listValueInput[0] = '\0';
        listTargetInput[0] = '\0';
    }
    else if (CheckCollisionPointRec(mouse, doublyButton))
    {
        dataStructureMode = DOUBLY_LIST_MODE;
        listValueInput[0] = '\0';
        listTargetInput[0] = '\0';
    }
    else if (CheckCollisionPointRec(mouse, bracketButton))
    {
        dataStructureMode = BRACKET_CHECKER_MODE;
        bracketInputActive = false;
        bracketResultReady = false;
        bracketScroll = 0;
    }
    else if (CheckCollisionPointRec(mouse, backButton))
    {
        dataStructureMode = STACK_MODE;
    }
}

// =========================================
// LINKED LIST LAB
// =========================================

static void DrawLinkedListLab(void)
{
    bool doubly = (dataStructureMode == DOUBLY_LIST_MODE);
    int size = doubly ? doublyList.size : singlyList.size;
    int displayCount = size < 10 ? size : 10;

    ClearBackground(RAYWHITE);

    DrawText(
        doubly ? "DOUBLY LINKED LIST" : "SINGLY LINKED LIST",
        300, 25, 32, DARKBLUE
    );

    DrawText("Value:", 40, 105, 18, DARKGRAY);

    DrawRectangle(110, 100, 180, 40, WHITE);
    DrawRectangleLines(110, 100, 180, 40, BLUE);
    DrawText(listValueInput, 120, 110, 20, BLACK);

    DrawText("Target:", 340, 105, 18, DARKGRAY);

    DrawRectangle(420, 100, 180, 40, WHITE);
    DrawRectangleLines(420, 100, 180, 40, BLUE);
    DrawText(listTargetInput, 430, 110, 20, BLACK);

    DrawButton((Rectangle){40, 170, 220, 45}, "INSERT BEGINNING");
    DrawButton((Rectangle){280, 170, 220, 45}, "INSERT END");
    DrawButton((Rectangle){520, 170, 220, 45}, "INSERT BEFORE");
    DrawButton((Rectangle){760, 170, 220, 45}, "INSERT AFTER");

    DrawButton((Rectangle){20, 230, 180, 45}, "DELETE BEGIN");
    DrawButton((Rectangle){212, 230, 180, 45}, "DELETE END");
    DrawButton((Rectangle){404, 230, 180, 45}, "DELETE VALUE");
    DrawButton((Rectangle){596, 230, 180, 45}, "DELETE BEFORE");
    DrawButton((Rectangle){788, 230, 180, 45}, "DELETE AFTER");

    DrawButton((Rectangle){20, 290, 180, 45}, "SEARCH");

    DrawText("LIST CONTENTS", 40, 355, 22, DARKBLUE);

    DrawText(
        TextFormat("Size: %d", size),
        300, 355, 20, DARKGRAY
    );

    DrawText(listMessage, 500, 355, 16, DARKGREEN);
    DrawText("FORWARD:", 40, 410, 17, DARKGRAY);

    for (int i = 0; i < displayCount; i++)
    {
        int value = 0;

        int valid = doubly
            ? doublyGetForwardAt(&doublyList, i, &value)
            : singlyGetAt(&singlyList, i, &value);

        if (valid)
        {
            int x = 40 + i * 90;

            DrawRectangle(x, 435, 70, 38, SKYBLUE);
            DrawRectangleLines(x, 435, 70, 38, DARKBLUE);

            DrawText(
                TextFormat("%d", value),
                x + 20, 444, 18, DARKBLUE
            );

            if (i < displayCount - 1)
            {
                DrawText(
                    doubly ? "<->" : "->",
                    x + 71, 444, 16, DARKBLUE
                );
            }
        }
    }

    if (doubly)
    {
        DrawText("BACKWARD:", 40, 495, 17, DARKGRAY);

        for (int i = 0; i < displayCount; i++)
        {
            int value = 0;

            if (doublyGetBackwardAt(&doublyList, i, &value))
            {
                int x = 40 + i * 90;

                DrawRectangle(x, 520, 70, 38, LIGHTGRAY);
                DrawRectangleLines(x, 520, 70, 38, DARKBLUE);

                DrawText(
                    TextFormat("%d", value),
                    x + 20, 529, 18, DARKBLUE
                );

                if (i < displayCount - 1)
                    DrawText("<->", x + 71, 529, 16, DARKBLUE);
            }
        }
    }

    if (size > 10)
    {
        DrawText(
            "Only the first 10 nodes are displayed.",
            650, 495, 14, GRAY
        );
    }

    DrawButton((Rectangle){400, 590, 200, 45}, "BACK");
}

static void HandleLinkedListClicks(Vector2 mouse)
{
    bool doubly = (dataStructureMode == DOUBLY_LIST_MODE);

    Rectangle valueBox = {110, 100, 180, 40};
    Rectangle targetBox = {420, 100, 180, 40};

    Rectangle buttons[10] =
    {
        {40, 170, 220, 45},
        {280, 170, 220, 45},
        {520, 170, 220, 45},
        {760, 170, 220, 45},
        {20, 230, 180, 45},
        {212, 230, 180, 45},
        {404, 230, 180, 45},
        {596, 230, 180, 45},
        {788, 230, 180, 45},
        {20, 290, 180, 45}
    };

    Rectangle backButton = {400, 590, 200, 45};

    if (CheckCollisionPointRec(mouse, backButton))
    {
        dataStructureMode = MORE_MODULES_MENU;
        listValueActive = false;
        listTargetActive = false;
        return;
    }

    if (CheckCollisionPointRec(mouse, valueBox))
    {
        listValueActive = true;
        listTargetActive = false;
        return;
    }

    if (CheckCollisionPointRec(mouse, targetBox))
    {
        listValueActive = false;
        listTargetActive = true;
        return;
    }

    listValueActive = false;
    listTargetActive = false;

    int action = -1;

    for (int i = 0; i < 10; i++)
    {
        if (CheckCollisionPointRec(mouse, buttons[i]))
        {
            action = i;
            break;
        }
    }

    if (action < 0)
        return;

    int value = 0;
    int target = 0;

    int valueValid = ParseInteger(listValueInput, &value);
    int targetValid = ParseInteger(listTargetInput, &target);

    int needsValue =
        action == 0 || action == 1 ||
        action == 2 || action == 3 ||
        action == 6 || action == 9;

    int needsTarget =
        action == 2 || action == 3 ||
        action == 7 || action == 8;

    if (needsValue && !valueValid)
    {
        snprintf(
            listMessage,
            sizeof(listMessage),
            "Enter a valid value."
        );
        return;
    }

    if (needsTarget && !targetValid)
    {
        snprintf(
            listMessage,
            sizeof(listMessage),
            "Enter a valid target value."
        );
        return;
    }

    int success = 0;
    int removed = 0;
    int searchIndex = -1;

    if (!doubly)
    {
        switch (action)
        {
            case 0:
                success = singlyInsertBeginning(&singlyList, value);
                break;

            case 1:
                success = singlyInsertEnd(&singlyList, value);
                break;

            case 2:
                success = singlyInsertBefore(&singlyList, target, value);
                break;

            case 3:
                success = singlyInsertAfter(&singlyList, target, value);
                break;

            case 4:
                success = singlyDeleteBeginning(&singlyList, &removed);
                break;

            case 5:
                success = singlyDeleteEnd(&singlyList, &removed);
                break;

            case 6:
                success = singlyDeleteValue(&singlyList, value);
                break;

            case 7:
                success = singlyDeleteBefore(&singlyList, target, &removed);
                break;

            case 8:
                success = singlyDeleteAfter(&singlyList, target, &removed);
                break;

            case 9:
                searchIndex = singlySearch(&singlyList, value);
                break;

            default:
                break;
        }
    }
    else
    {
        switch (action)
        {
            case 0:
                success = doublyInsertBeginning(&doublyList, value);
                break;

            case 1:
                success = doublyInsertEnd(&doublyList, value);
                break;

            case 2:
                success = doublyInsertBefore(&doublyList, target, value);
                break;

            case 3:
                success = doublyInsertAfter(&doublyList, target, value);
                break;

            case 4:
                success = doublyDeleteBeginning(&doublyList, &removed);
                break;

            case 5:
                success = doublyDeleteEnd(&doublyList, &removed);
                break;

            case 6:
                success = doublyDeleteValue(&doublyList, value);
                break;

            case 7:
                success = doublyDeleteBefore(&doublyList, target, &removed);
                break;

            case 8:
                success = doublyDeleteAfter(&doublyList, target, &removed);
                break;

            case 9:
                searchIndex = doublySearch(&doublyList, value);
                break;

            default:
                break;
        }
    }

    if (action == 9)
    {
        if (searchIndex >= 0)
        {
            snprintf(
                listMessage,
                sizeof(listMessage),
                "Value found at position %d.",
                searchIndex
            );
        }
        else
        {
            snprintf(
                listMessage,
                sizeof(listMessage),
                "Value not found."
            );
        }
    }
    else if (success)
    {
        if (action >= 4 && action <= 8)
        {
            if (action == 6)
            {
                snprintf(
                    listMessage,
                    sizeof(listMessage),
                    "Value deleted successfully."
                );
            }
            else
            {
                snprintf(
                    listMessage,
                    sizeof(listMessage),
                    "Deleted value: %d",
                    removed
                );
            }
        }
        else
        {
            snprintf(
                listMessage,
                sizeof(listMessage),
                "Value inserted successfully."
            );

            listValueInput[0] = '\0';
        }
    }
    else
    {
        snprintf(
            listMessage,
            sizeof(listMessage),
            "Operation failed. Check that the list and target exist."
        );
    }
}

// =========================================
// BRACKET BALANCE CHECKER
// =========================================

static void DrawBracketCheckerLab(void)
{
    int visibleSteps = 8;

    int maxScroll =
        bracketResult.stepCount > visibleSteps
        ? bracketResult.stepCount - visibleSteps : 0;

    ClearBackground(RAYWHITE);

    DrawText(
        "BRACKET BALANCE CHECKER",
        265, 35, 32, DARKBLUE
    );

    DrawText(
        "Enter an expression containing (), [] or {}:",
        120, 125, 20, DARKGRAY
    );

    DrawRectangle(120, 165, 760, 50, WHITE);
    DrawRectangleLines(120, 165, 760, 50, BLUE);

    DrawText(bracketExpression, 135, 178, 22, BLACK);

    DrawButton((Rectangle){420, 235, 160, 50}, "CHECK");

    if (bracketResultReady)
    {
        DrawText(
            bracketResult.message,
            120, 305, 22,
            bracketResult.balanced ? DARKGREEN : RED
        );

        DrawText(
            "STACK PROCESSING STEPS",
            120, 345, 20, DARKBLUE
        );

        if (bracketResult.stepCount == 0)
        {
            DrawText(
                "No brackets were found in the expression.",
                120, 385, 17, DARKGRAY
            );
        }
        else
        {
            int first = bracketScroll;
            int last = first + visibleSteps;

            if (last > bracketResult.stepCount)
                last = bracketResult.stepCount;

            DrawText(
                TextFormat(
                    "Showing steps %d-%d of %d (use mouse wheel to scroll)",
                    first + 1,
                    last,
                    bracketResult.stepCount
                ),
                120, 368, 14, GRAY
            );

            for (int i = first; i < last; i++)
            {
                BracketStep *step = &bracketResult.steps[i];

                DrawText(
                    TextFormat(
                        "%d. Bracket: %c | Stack: %s",
                        i + 1,
                        step->symbol,
                        step->stackSize > 0
                            ? step->stackSnapshot : "EMPTY"
                    ),
                    120,
                    393 + (i - first) * 25,
                    17,
                    DARKGRAY
                );
            }
        }

        if (maxScroll > 0)
        {
            DrawText(
                "Scroll over this screen to view the remaining steps.",
                120, 600, 14, GRAY
            );
        }
    }

    DrawButton((Rectangle){400, 610, 200, 35}, "BACK");
}

static void HandleBracketCheckerClicks(Vector2 mouse)
{
    Rectangle inputBox = {120, 165, 760, 50};
    Rectangle checkButton = {420, 235, 160, 50};
    Rectangle backButton = {400, 610, 200, 35};

    if (CheckCollisionPointRec(mouse, backButton))
    {
        dataStructureMode = MORE_MODULES_MENU;
        bracketInputActive = false;
        return;
    }

    if (CheckCollisionPointRec(mouse, inputBox))
    {
        bracketInputActive = true;
        return;
    }

    bracketInputActive = false;

    if (CheckCollisionPointRec(mouse, checkButton))
    {
        bracketCheck(bracketExpression, &bracketResult);
        bracketResultReady = true;
        bracketScroll = 0;
    }
}

// =========================================
// PLACEHOLDER SCREENS
// =========================================

static void DrawModuleScreen(const char *title)
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
// MOUSE CONTROL HELPERS
// =========================================

static void HandleHanoiClicks(Vector2 mouse)
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

    if (CheckCollisionPointRec(mouse, minusButton) && hanoiDisks > 1)
    {
        hanoiDisks--;
        InitialiseHanoi();
    }
    else if (CheckCollisionPointRec(mouse, plusButton) && hanoiDisks < 8)
    {
        hanoiDisks++;
        InitialiseHanoi();
    }

    if (CheckCollisionPointRec(mouse, slowButton))
        hanoiMoveSpeed = 1.5f;

    if (CheckCollisionPointRec(mouse, normalButton))
        hanoiMoveSpeed = 1.0f;

    if (CheckCollisionPointRec(mouse, fastButton))
        hanoiMoveSpeed = 0.3f;

    if (CheckCollisionPointRec(mouse, startButton) &&
        hanoiCurrentMove < hanoiMoveCount)
    {
        hanoiRunning = true;
        hanoiPaused = false;
    }

    if (CheckCollisionPointRec(mouse, pauseButton))
    {
        hanoiPaused = true;
        hanoiRunning = false;
    }

    if (CheckCollisionPointRec(mouse, stepButton) &&
        hanoiCurrentMove < hanoiMoveCount)
    {
        HanoiMove move = hanoiMoves[hanoiCurrentMove++];

        MoveHanoiDisk(move.from, move.to);

        if (hanoiCurrentMove >= hanoiMoveCount)
            hanoiRunning = false;
    }

    if (CheckCollisionPointRec(mouse, resetButton))
        InitialiseHanoi();

    if (CheckCollisionPointRec(mouse, backButton))
    {
        recursionMode = FIBONACCI;
        ResetRecursionInput();
    }
}

static void HandleRecursionClicks(Vector2 mouse)
{
    Rectangle inputBox = {150, 300, 250, 50};

    Rectangle fibButton = {150, 155, 220, 65};
    Rectangle factButton = {390, 155, 220, 65};
    Rectangle hanoiButton = {630, 155, 220, 65};

    Rectangle calculateButton = {150, 420, 250, 65};
    Rectangle backButton = {150, 530, 180, 60};

    inputActive = CheckCollisionPointRec(mouse, inputBox);

    if (CheckCollisionPointRec(mouse, fibButton))
    {
        recursionMode = FIBONACCI;
        ResetRecursionInput();
    }
    else if (CheckCollisionPointRec(mouse, factButton))
    {
        recursionMode = FACTORIAL;
        ResetRecursionInput();
    }
    else if (CheckCollisionPointRec(mouse, hanoiButton))
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
            CalculateFibonacci();
        else if (recursionMode == FACTORIAL)
            CalculateFactorial();
    }

    if (CheckCollisionPointRec(mouse, backButton))
    {
        currentScreen = DASHBOARD;
        ResetRecursionInput();
    }
}

// =========================================
// MAIN FUNCTION
// =========================================

int main(void)
{
    InitWindow(1000, 650, "Algorithm Explorer");

    if (!IsWindowReady())
        return 1;

    SetTargetFPS(60);

    // Initialize linked lists before they are used.
    singlyInit(&singlyList);
    doublyInit(&doublyList);
    InitialiseHanoi();

    while (!WindowShouldClose())
    {
        // Automatic Tower of Hanoi animation
        if (currentScreen == RECURSION_LAB &&
            recursionMode == TOWER_OF_HANOI &&
            hanoiRunning && !hanoiPaused)
        {
            hanoiMoveTimer += GetFrameTime();

            if (hanoiMoveTimer >= hanoiMoveSpeed)
            {
                if (hanoiCurrentMove < hanoiMoveCount)
                {
                    HanoiMove move = hanoiMoves[hanoiCurrentMove++];

                    MoveHanoiDisk(move.from, move.to);
                }

                hanoiMoveTimer = 0.0;

                if (hanoiCurrentMove >= hanoiMoveCount)
                    hanoiRunning = false;
            }
        }

        // =====================================
        // KEYBOARD INPUT
        // =====================================

        if (currentScreen == RECURSION_LAB &&
            recursionMode != TOWER_OF_HANOI &&
            inputActive)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                size_t length = strlen(inputText);

                if (key >= '0' && key <= '9' && length < 2)
                {
                    inputText[length] = (char)key;
                    inputText[length + 1] = '\0';
                    resultReady = false;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                size_t length = strlen(inputText);

                if (length > 0)
                {
                    inputText[length - 1] = '\0';
                    resultReady = false;
                }
            }
        }

        if (currentScreen == DATA_STRUCTURES_LAB &&
            dataStructureMode == STACK_MODE &&
            stackInputActive)
        {
            HandleIntegerTyping(stackInput, sizeof(stackInput));
        }

        if (currentScreen == DATA_STRUCTURES_LAB &&
            dataStructureMode == QUEUE_MODE &&
            queueInputActive)
        {
            HandleIntegerTyping(queueInput, sizeof(queueInput));
        }

        if (currentScreen == DATA_STRUCTURES_LAB &&
            (dataStructureMode == SINGLY_LIST_MODE ||
             dataStructureMode == DOUBLY_LIST_MODE))
        {
            if (listValueActive)
            {
                HandleIntegerTyping(
                    listValueInput,
                    sizeof(listValueInput)
                );
            }
            else if (listTargetActive)
            {
                HandleIntegerTyping(
                    listTargetInput,
                    sizeof(listTargetInput)
                );
            }
        }

        if (currentScreen == DATA_STRUCTURES_LAB &&
            dataStructureMode == BRACKET_CHECKER_MODE &&
            bracketInputActive)
        {
            int key = GetCharPressed();

            while (key > 0)
            {
                size_t length = strlen(bracketExpression);

                if (key >= 32 && key <= 126 &&
                    length + 1 < sizeof(bracketExpression))
                {
                    bracketExpression[length] = (char)key;
                    bracketExpression[length + 1] = '\0';
                    bracketResultReady = false;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                size_t length = strlen(bracketExpression);

                if (length > 0)
                {
                    bracketExpression[length - 1] = '\0';
                    bracketResultReady = false;
                }
            }
        }

        // Scroll through bracket-processing steps
        if (currentScreen == DATA_STRUCTURES_LAB &&
            dataStructureMode == BRACKET_CHECKER_MODE &&
            bracketResultReady)
        {
            int wheel = (int)GetMouseWheelMove();

            int maxScroll =
                bracketResult.stepCount > 8
                ? bracketResult.stepCount - 8 : 0;

            bracketScroll -= wheel;

            if (bracketScroll < 0)
                bracketScroll = 0;

            if (bracketScroll > maxScroll)
                bracketScroll = maxScroll;
        }

        // =====================================
        // MOUSE CLICKS
        // =====================================

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();

            if (currentScreen == DASHBOARD)
            {
                Rectangle dashboardButtons[4] =
                {
                    {150, 260, 320, 85},
                    {530, 260, 320, 85},
                    {150, 375, 320, 85},
                    {530, 375, 320, 85}
                };

                for (int i = 0; i < 4; i++)
                {
                    if (CheckCollisionPointRec(mouse, dashboardButtons[i]))
                    {
                        currentScreen = (Screen)(i + 1);

                        ResetRecursionInput();
                        ResetDataStructureInputFocus();

                        if (currentScreen == DATA_STRUCTURES_LAB)
                            dataStructureMode = STACK_MODE;

                        break;
                    }
                }
            }
            else if (currentScreen == RECURSION_LAB)
            {
                if (recursionMode == TOWER_OF_HANOI)
                    HandleHanoiClicks(mouse);
                else
                    HandleRecursionClicks(mouse);
            }
            else if (currentScreen == DATA_STRUCTURES_LAB)
            {
                if (dataStructureMode == STACK_MODE)
                    HandleStackClicks(mouse);
                else if (dataStructureMode == QUEUE_MODE)
                    HandleQueueClicks(mouse);
                else if (dataStructureMode == MORE_MODULES_MENU)
                    HandleMoreModulesClicks(mouse);
                else if (dataStructureMode == SINGLY_LIST_MODE ||
                         dataStructureMode == DOUBLY_LIST_MODE)
                    HandleLinkedListClicks(mouse);
                else if (dataStructureMode == BRACKET_CHECKER_MODE)
                    HandleBracketCheckerClicks(mouse);
            }
            else
            {
                Rectangle backButton = {400, 450, 200, 65};

                if (CheckCollisionPointRec(mouse, backButton))
                    currentScreen = DASHBOARD;
            }
        }

        // =====================================
        // DRAW CURRENT SCREEN
        // =====================================

        BeginDrawing();

        if (currentScreen == DASHBOARD)
        {
            DrawDashboard();
        }
        else if (currentScreen == RECURSION_LAB)
        {
            if (recursionMode == TOWER_OF_HANOI)
                DrawHanoiScreen();
            else
                DrawRecursionLab();
        }
        else if (currentScreen == DATA_STRUCTURES_LAB)
        {
            if (dataStructureMode == STACK_MODE)
                DrawStackLab();
            else if (dataStructureMode == QUEUE_MODE)
                DrawQueueLab();
            else if (dataStructureMode == MORE_MODULES_MENU)
                DrawMoreModulesMenu();
            else if (dataStructureMode == SINGLY_LIST_MODE ||
                     dataStructureMode == DOUBLY_LIST_MODE)
                DrawLinkedListLab();
            else if (dataStructureMode == BRACKET_CHECKER_MODE)
                DrawBracketCheckerLab();
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
    }

    // =====================================
    // FREE MEMORY
    // =====================================

    if (stackInitialized)
        stackFree(&userStack);

    if (queueInitialized)
        queueFree(&circularQueue);

    if (linearQueueInitialized)
        linearQueueFree(&linearQueue);

    singlyClear(&singlyList);
    doublyClear(&doublyList);

    CloseWindow();

    return 0;
}
