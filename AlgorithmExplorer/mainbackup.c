
#include "raylib.h"
#include <stdio.h>

// Screen identification
typedef enum
{
    DASHBOARD,
    RECURSION_LAB,
    DATA_STRUCTURES_LAB,
    SORTING_LAB,
    COMPLEXITY_LAB
} Screen;

// Function to draw a clickable button
void DrawButton(Rectangle button, const char *text)
{
    Vector2 mouse = GetMousePosition();

    bool hovering = CheckCollisionPointRec(mouse, button);

    Color buttonColor = hovering ? BLUE : DARKBLUE;

    DrawRectangleRounded(button, 0.2f, 10, buttonColor);

    int textWidth = MeasureText(text, 22);

    DrawText(text,
             button.x + (button.width - textWidth) / 2,
             button.y + 30,
             22,
             WHITE);
}

// Main dashboard
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

// Individual module screens
void DrawModuleScreen(const char *title)
{
    ClearBackground(RAYWHITE);

    DrawText(title, 350, 100, 35, DARKBLUE);

    DrawText("Module content will be developed here.",
             300, 200, 22, DARKGRAY);

    DrawText("Click BACK to return to the dashboard.",
             300, 260, 20, GRAY);

    DrawButton((Rectangle){400, 450, 200, 65}, "BACK");
}

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 650;

    InitWindow(screenWidth, screenHeight, "Algorithm Explorer");

    SetTargetFPS(60);

    Screen currentScreen = DASHBOARD;

    Rectangle buttons[4] =
    {
        {150, 260, 320, 85},
        {530, 260, 320, 85},
        {150, 375, 320, 85},
        {530, 375, 320, 85}
    };

    const char *moduleTitles[4] =
    {
        "Recursion Lab",
        "Data Structures Lab",
        "Sorting Lab",
        "Time Complexity Analysis"
    };

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        // Handle mouse clicks
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (currentScreen == DASHBOARD)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (CheckCollisionPointRec(mouse, buttons[i]))
                    {
                        currentScreen = (Screen)(i + 1);
                    }
                }
            }
            else
            {
                Rectangle backButton = {400, 450, 200, 65};

                if (CheckCollisionPointRec(mouse, backButton))
                {
                    currentScreen = DASHBOARD;
                }
            }
        }

        // Draw the current screen
        BeginDrawing();

        if (currentScreen == DASHBOARD)
        {
            DrawDashboard();
        }
        else
        {
            DrawModuleScreen(moduleTitles[currentScreen - 1]);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
