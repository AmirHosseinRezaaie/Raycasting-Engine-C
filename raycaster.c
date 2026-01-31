// raycaster.c
#include "raycaster.h"
#include "constants.h"
#include "player.h"
#include "map.h"
#include <math.h>
#include <stdio.h>

void render_raycast_scene(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, (Color){135, 206, 235, 255});
    DrawRectangle(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, (Color){34, 139, 34, 255});

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
            sideDistX = (player_pos[0] - (float)mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = ((float)mapX + 1.0f - player_pos[0]) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (player_pos[1] - (float)mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = ((float)mapY + 1.0f - player_pos[1]) * deltaDistY;
        }

        int hit = 0;
        int side = 0;

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

        float perpWallDist;
        
        if (side == 0)
        {
            perpWallDist = (sideDistX - deltaDistX);
        }
        else
        {
            perpWallDist = (sideDistY - deltaDistY);
        }

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0)
        {
            drawStart = 0;
        }

        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT)
        {
            drawEnd = SCREEN_HEIGHT - 1;
        }

        // Calculate brightness based on distance (0.3 to 1.0)
        float brightness = 1.0f / (1.0f + perpWallDist * 0.05f);
        if (brightness < 0.3f) brightness = 0.3f;
        if (brightness > 1.0f) brightness = 1.0f;
        
        Color wallColor;
        if (side == 1)
        {
            wallColor = (Color){
                (unsigned char)(101 * brightness),
                (unsigned char)(67 * brightness),
                (unsigned char)(33 * brightness),
                255
            };
        }
        else
        {
            wallColor = (Color){
                (unsigned char)(160 * brightness),
                (unsigned char)(82 * brightness),
                (unsigned char)(45 * brightness),
                255
            };
        }

        DrawLine(x, drawStart, x, drawEnd, wallColor);

    }

}