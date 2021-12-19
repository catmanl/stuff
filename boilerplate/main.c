#include "raylib.h"

int main()
{
    InitWindow(800, 450, "X EXAMPLE");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
