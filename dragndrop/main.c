#include "raylib.h"

#define REC_SIZE        64.0f
#define MID_REC_SIZE    REC_SIZE/2

void centered_text(const char *text, int y, int font_size, Color color);

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(800, 450, "DRAG N' DROP EXAMPLE");

    Rectangle rec = { (float)GetScreenWidth()/2 - MID_REC_SIZE, (float)GetScreenHeight()/2 - MID_REC_SIZE, REC_SIZE, REC_SIZE };
    Rectangle hitbox_rec = { rec.x - MID_REC_SIZE/2, rec.y - MID_REC_SIZE/2, REC_SIZE, REC_SIZE };
    int font_size = 30;
    const char *text = "Hello there!";
    int width = MeasureText(text, font_size);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        hitbox_rec = (Rectangle){ rec.x - MID_REC_SIZE/2, rec.y - MID_REC_SIZE/2, REC_SIZE*1.5f, REC_SIZE*1.5f };
        bool over_hitbox = CheckCollisionPointRec(mouse, hitbox_rec);

        // Drag logic
        bool cond = (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && over_hitbox);
        float mousex = GetMouseDelta().x;
        float mousey = GetMouseDelta().y;

        if (cond) {
            rec.x += mousex;
            rec.y += mousey;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleLinesEx(hitbox_rec, 2, over_hitbox? GRAY : BLANK);
        DrawRectangleRec(rec, RED);
        centered_text(text, (int)(rec.y + (REC_SIZE + MID_REC_SIZE)), font_size, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void centered_text(const char *text, int y, int font_size, Color color)
{
    int width = MeasureText(text, font_size);
    DrawText(text, GetScreenWidth()/2 - width/2, y, font_size, BLACK);
}

