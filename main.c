#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "map.h"
#include "player.h"
#include "raycaster.h"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting Engine - 2D Map Test");

    init_map();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_minimap();

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}