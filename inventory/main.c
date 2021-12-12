// Small inventory system
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define string      const char *
#define NUM_SLOTS   9
#define SEPARATE_X  2.95
#define SEPARATE_Y  1.1
#define OFFSET      10
#define REC_SIZE    80
#define WIDTH       NUM_SLOTS/3
#define HEIGHT      WIDTH

typedef struct Item {
    string name;
    int id;
    bool selected;
    Texture2D texture;
} Item;

typedef struct Inventory {
    Item *items;
    size_t used;
    size_t size;
} Inventory;

Item new_item(string name, int id, string texture_path);
void new_inventory(Inventory *inventory, size_t size);
Inventory copy_inventory(Inventory *src);
void add_item_to_inventory(Inventory *inventory, Item item);
void rearrange_inventory(Inventory *inventory, int previdx, int newidx);
void deinit_inventory(Inventory *inventory);

void draw_stuff(Inventory Inventory);

bool is_num_pressed(void);

int slots[NUM_SLOTS] = {
    0, 1, 2,
    3, 4, 5,
    6, 7, 8
};

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(800, 450, "INVENTORY");

    Item items[NUM_SLOTS];
    Inventory inventory;
    new_inventory(&inventory, NUM_SLOTS);

    for (int i = 0; i < NUM_SLOTS; i++) {
        items[i] = new_item("", i + 1, TextFormat("assets/texture%d.png", i));
    }

    // We do this so we can have names rather than numbers...
    items[0].name = "Orb";
    items[1].name = "Sword";
    items[2].name = "Friend rock";
    items[3].name = "Poop";
    items[4].name = "Strange chips";
    items[5].name = "Sample 1";
    items[6].name = "Sample 2";
    items[7].name = "Sample 3";
    items[8].name = "Sample 4";
    for (int i = 0; i < NUM_SLOTS; i++) {
        add_item_to_inventory(&inventory, items[i]);
    }

    string instruction_text = "Use numerical keys to move items from slots to others";
    string instruction_text2 = "(Similar to Minecraft)";

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_stuff(inventory);
        DrawText(instruction_text, (GetScreenWidth()/2 - MeasureText(instruction_text, 20)/2), 10, 20, BLACK);
        DrawText(instruction_text2, (GetScreenWidth()/2 - MeasureText(instruction_text2, 20)/2), 40, 20, BLACK);
        EndDrawing();
    }

    for (int i = 0; i < NUM_SLOTS; i++) {
        UnloadTexture(items[i].texture);
    }
    deinit_inventory(&inventory);
    CloseWindow();
    return 0;
}

Item new_item(string name, int id, string texture_path)
{
    Texture2D texture = LoadTexture(texture_path);
    Item item = {
        .name = name,
        .id = id,
        .selected = false,
        .texture = texture
    };

    return item;
}

void new_inventory(Inventory *inventory, size_t size)
{
    inventory->items = malloc(sizeof(Item)*size);
    inventory->used = 0;
    inventory->size = size;
}

Inventory copy_inventory(Inventory *src)
{
    Inventory dst;
    new_inventory(&dst, src->size);     // Allocate enough bytes for the copy to work
    dst.size = src->size;
    dst.used = src->used;
    for (int i = 0; i < src->used; i++) {
        dst.items[i] = src->items[i];
    }

    return dst;
}

void add_item_to_inventory(Inventory *inventory, Item item)
{
    if (inventory->used == inventory->size) {
        inventory->size *= 2;
        inventory->items = realloc(inventory->items, sizeof(Item)*inventory->size);
    }
    inventory->items[inventory->used++] = item;
}

void rearrange_inventory(Inventory *inventory, int previdx, int newidx)
{
    // Swapping the elements
    Inventory tmp = copy_inventory(inventory);
    inventory->items[previdx] = inventory->items[newidx];
    inventory->items[newidx] = tmp.items[previdx];

    // We don't need the inventory anymore
    deinit_inventory(&tmp);
}

void deinit_inventory(Inventory *inventory)
{
    if (inventory->items) {
        free(inventory->items);
    }
    inventory->items = NULL;
    inventory->used = 0;
    inventory->size = 0;
}

void draw_stuff(Inventory inventory)
{
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // Grid
            int rec_id = slots[y*WIDTH + x];
            // Get spacings
            Rectangle rec = { 
                .x = (float)(x + SEPARATE_X)*(REC_SIZE + OFFSET),
                .y = (float)(y + SEPARATE_Y)*(REC_SIZE + OFFSET),
                .width = REC_SIZE,
                .height = REC_SIZE
            };

            // Switching items logic
            bool cond = CheckCollisionPointRec(GetMousePosition(), rec);
            int item_id = inventory.items[y*WIDTH + x].id;
            if (cond) {
                DrawRectangleRec(rec, LIGHTGRAY);   // Indicates where the user's cursor is at
                // We are going to follow a keyboad & mouse-based switching logic,
                // similar to Minecraft's
                int switch_to = 0;
                if (IsKeyPressed(KEY_ONE)) switch_to = 0;
                if (IsKeyPressed(KEY_TWO)) switch_to = 1;
                if (IsKeyPressed(KEY_THREE)) switch_to = 2;
                if (IsKeyPressed(KEY_FOUR)) switch_to = 3;
                if (IsKeyPressed(KEY_FIVE)) switch_to = 4;
                if (IsKeyPressed(KEY_SIX)) switch_to = 5;
                if (IsKeyPressed(KEY_SEVEN)) switch_to = 6;
                if (IsKeyPressed(KEY_EIGHT)) switch_to = 7;
                if (IsKeyPressed(KEY_NINE)) switch_to = 8;
                if (is_num_pressed()) {
                    rearrange_inventory(&inventory, rec_id, switch_to);
                }
            }
            DrawRectangleLinesEx(rec, 2.0f, BLACK);

            // Items
            Texture2D texture = inventory.items[y*WIDTH + x].texture;
            int separation_x = ((x + SEPARATE_X)*(REC_SIZE + OFFSET)) + texture.width/8;
            int separation_y = (y + SEPARATE_Y)*(REC_SIZE + OFFSET) + texture.height/8;
            string item_name = inventory.items[y*WIDTH + x].name;
            DrawTexture(texture, separation_x, separation_y, WHITE);
            if (cond) {
                DrawText(TextFormat("Item ID: %d", item_id), 10, 40, 30, BLACK);
                DrawText(TextFormat("Item name: \n%s", item_name), 10, 90, 30, BLACK);
            }
        }
    }
}

bool is_num_pressed(void)
{
    int key = GetKeyPressed();
    if (key >= KEY_ONE && key <= KEY_NINE) {
        return true;
    }

    return false;
}
