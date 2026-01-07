#include "raycaster.h"
#include "constants.h"
#include "player.h"
#include "map.h"
#include <math.h>
#include <stdio.h>

void render_raycast_scene(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, DARKGRAY);
    DrawRectangle(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, GRAY);

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        float cameraX = 2.0f * x / (float)SCREEN_WIDTH - 1.0f;
        
        float rayDirX = player_dir[0] + player_plane[0] * cameraX;
        float rayDirY = player_dir[1] + player_plane[1] * cameraX;

        if (x == SCREEN_WIDTH / 2)
        {
            printf("Center ray: dirX=%.3f, dirY=%.3f\n", rayDirX, rayDirY);
            DrawLine(x, 0, x, SCREEN_HEIGHT, RED);
        }
    }

    DrawText("Task 1: Ray directions calculated", 10, 10, 20, GREEN);
}