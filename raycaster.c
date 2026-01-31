#include <math.h>
#include "constants.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"

void render_raycast_scene(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, CEILING_COLOR);
    DrawRectangle(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, FLOOR_COLOR);

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        float camera_x = 2.0f * x / (float)SCREEN_WIDTH - 1.0f;
        
        float ray_dir_x = player_dir[0] + player_plane[0] * camera_x;
        float ray_dir_y = player_dir[1] + player_plane[1] * camera_x;

        int map_x = (int)player_pos[0];
        int map_y = (int)player_pos[1];

        float delta_dist_x = (ray_dir_x == 0) ? INFINITY_DIST : fabsf(1.0f / ray_dir_x);
        float delta_dist_y = (ray_dir_y == 0) ? INFINITY_DIST : fabsf(1.0f / ray_dir_y);

        float side_dist_x;
        float side_dist_y;
        int step_x;
        int step_y;

        if (ray_dir_x < 0)
        {
            step_x = -1;
            side_dist_x = (player_pos[0] - (float)map_x) * delta_dist_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = ((float)map_x + 1.0f - player_pos[0]) * delta_dist_x;
        }

        if (ray_dir_y < 0)
        {
            step_y = -1;
            side_dist_y = (player_pos[1] - (float)map_y) * delta_dist_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = ((float)map_y + 1.0f - player_pos[1]) * delta_dist_y;
        }

        int hit = 0;
        int side = 0;

        while (hit == 0)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            if (world_map[map_x][map_y] > 0)
                hit = 1;
        }

        float perp_wall_dist;
        
        if (side == 0)
            perp_wall_dist = (side_dist_x - delta_dist_x);
        else
            perp_wall_dist = (side_dist_y - delta_dist_y);

        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;

        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT)
            draw_end = SCREEN_HEIGHT - 1;

        float brightness = 1.0f / (1.0f + perp_wall_dist * BRIGHTNESS_FACTOR);
        if (brightness < MIN_BRIGHTNESS)
            brightness = MIN_BRIGHTNESS;
        if (brightness > MAX_BRIGHTNESS)
            brightness = MAX_BRIGHTNESS;
        
        Color wall_color;
        if (side == 1)
        {
            wall_color = (Color){
                (unsigned char)(WALL_COLOR_DARK_R * brightness),
                (unsigned char)(WALL_COLOR_DARK_G * brightness),
                (unsigned char)(WALL_COLOR_DARK_B * brightness),
                255
            };
        }
        else
        {
            wall_color = (Color){
                (unsigned char)(WALL_COLOR_LIGHT_R * brightness),
                (unsigned char)(WALL_COLOR_LIGHT_G * brightness),
                (unsigned char)(WALL_COLOR_LIGHT_B * brightness),
                255
            };
        }

        DrawLine(x, draw_start, x, draw_end, wall_color);
    }
}