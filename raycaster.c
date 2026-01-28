// raycaster.c
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

        int mapX = (int)player_pos[0];
        int mapY = (int)player_pos[1];

        float deltaDistX = (rayDirX == 0) ? 1e30f : fabsf(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : fabsf(1.0f / rayDirY);

        float sideDistX;
        float sideDistY;
        int stepX;
        int stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (player_pos[0] - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0f - player_pos[0]) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (player_pos[1] - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0f - player_pos[1]) * deltaDistY;
        }

        int hit = 0;
        int side;

        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (world_map[mapX][mapY] > 0)
            {
                hit = 1;
            }
        }

        if (x == SCREEN_WIDTH / 2)
        {
            printf("Wall hit: [%d,%d] side=%d\n", mapX, mapY, side);
            DrawLine(x, 0, x, SCREEN_HEIGHT, RED);
        }
    }

    DrawText("Task 3: DDA loop - wall detection", 10, 10, 20, GREEN);
}