#include "./raylib.h"
#include <string.h>

#define RECT_WIDTH  600
#define RECT_HEIGHT 150

void draw(const char *message, int frames_counter, bool enabled);

const char *messages[3] = {
    "Hello!",
    "It looks like you're lost...",
    "What can I do to help you?"
};

int main(void)
{
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "SYSTEMS - Dialogues");
    int frames_counter = 0;
    int message_index = 0;
    const char *message = messages[message_index];
    int len = strlen(message);
    bool enabled_to_skip = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update
        frames_counter++;
        if (IsKeyPressed(KEY_SPACE) && enabled_to_skip) {
            frames_counter = 0;
            message_index++;
            enabled_to_skip = false;
            if (message_index >= 3) {
                message_index = 2;
            }
        }
        message = messages[message_index];
        len = strlen(message);
        if (frames_counter >= len*3.5) {
            enabled_to_skip = true;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        draw(message, frames_counter, enabled_to_skip);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void draw(const char *message, int frames_counter, bool enabled)
{
    Rectangle box = { (float)(GetScreenWidth()/2 - RECT_WIDTH/2),
                      (float)(GetScreenHeight() - (RECT_HEIGHT + 50)),
                      RECT_WIDTH,
                      RECT_HEIGHT };
    DrawRectangleRec(box, DARKBLUE);          // Inner part
    DrawRectangleLinesEx(box, 5, LIGHTGRAY);  // Outer part
    DrawText(TextSubtext(message, 0, frames_counter/3), box.x + 30, box.y + 40, 40, WHITE);
    if (enabled) {
        DrawRectangle(box.x + RECT_WIDTH - 48, box.y + 105, 32, 32, WHITE);
    }
}

