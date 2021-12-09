#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Array {
    Rectangle *values;
    size_t used;
    size_t size;
} Array;

void arr_init(Array *a, size_t init_size);
size_t arr_getsize(Array *a);
void arr_insert(Array *a, Rectangle element);
void arr_deinit(Array *a);

const int REC_SIZE = 64;

int main(int argc, char **argv)
{
    InitWindow(800, 450, "Dynamic Array example");

    Array a;
    arr_init(&a, 1);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            arr_insert(&a, (Rectangle){ (float)GetMouseX(), (float)GetMouseY(), REC_SIZE, REC_SIZE });
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < (int)arr_getsize(&a); i++) {
            DrawRectanglePro(a.values[i], (Vector2){ REC_SIZE/2, REC_SIZE/2 }, 0.0f, RED);
        }

        DrawText(TextFormat("Rectangles: %zu", arr_getsize(&a)), 10, 10, 20, BLACK);
        EndDrawing();
    }

    arr_deinit(&a);
    CloseWindow();
    return 0;
}

void arr_init(Array *a, size_t init_size)
{
    a->values = malloc(sizeof(Rectangle)*init_size);
    a->used = 0;
    a->size = init_size;

    for (int i = 0; i < (int)init_size; i++) {
        a->values[i] = (Rectangle){ i*REC_SIZE + 10, 40, REC_SIZE, REC_SIZE };
    }
    TraceLog(LOG_INFO, "ARRAY: Init array with %zu elements", init_size);
}

size_t arr_getsize(Array *a)
{
    return a->used;
}

void arr_insert(Array *a, Rectangle element)
{
    if (a->used == a->size) {
        a->size *= 2;
        a->values = realloc(a->values, sizeof(Rectangle)*a->size);
    }

    a->values[a->used++] = element;
    TraceLog(LOG_INFO, "ARRAY: Increment array size");
}

void arr_deinit(Array *a)
{
    TraceLog(LOG_INFO, "ARRAY: Deinitialize array of %zu elements", arr_getsize(a));
    free(a->values);
    a->values = NULL;
    a->used = 0;
    a->size = 0;
}
