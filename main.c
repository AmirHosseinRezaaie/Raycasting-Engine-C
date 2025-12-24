// main.c
#include "raylib.h"
#include "constants.h"
#include "map.h"
#include "player.h"
#include <math.h>

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster - 2D Map Editor");

    init_player();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Input handling
        float move_speed = 3.0f * GetFrameTime();
        float rot_speed  = 2.0f * GetFrameTime();

        // Toggle mode with M key
        if (IsKeyPressed(KEY_M))
            game_state = (game_state == STATE_PLAY) ? STATE_EDIT : STATE_PLAY;

        // Movement and rotation in play mode
        if (game_state == STATE_PLAY)
        {
            // Forward (W)
            if (IsKeyDown(KEY_W))
            {
                float new_x = player_pos[0] + player_dir[0] * move_speed;
                float new_y = player_pos[1] + player_dir[1] * move_speed;
                if (world_map[(int)new_x][(int)player_pos[1]] == 0) player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0) player_pos[1] = new_y;
            }
            // Backward (S)
            if (IsKeyDown(KEY_S))
            {
                float new_x = player_pos[0] - player_dir[0] * move_speed;
                float new_y = player_pos[1] - player_dir[1] * move_speed;
                if (world_map[(int)new_x][(int)player_pos[1]] == 0) player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0) player_pos[1] = new_y;
            }

            // Rotation right
            if (IsKeyDown(KEY_RIGHT))
            {
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * cos(-rot_speed) - player_dir[1] * sin(-rot_speed);
                player_dir[1] = old_dir_x * sin(-rot_speed) + player_dir[1] * cos(-rot_speed);

                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * cos(-rot_speed) - player_plane[1] * sin(-rot_speed);
                player_plane[1] = old_plane_x * sin(-rot_speed) + player_plane[1] * cos(-rot_speed);
            }
            // Rotation left
            if (IsKeyDown(KEY_LEFT))
            {
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * cos(rot_speed) - player_dir[1] * sin(rot_speed);
                player_dir[1] = old_dir_x * sin(rot_speed) + player_dir[1] * cos(rot_speed);

                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * cos(rot_speed) - player_plane[1] * sin(rot_speed);
                player_plane[1] = old_plane_x * sin(rot_speed) + player_plane[1] * cos(rot_speed);
            }
            // Strafe Right (D)
            if (IsKeyDown(KEY_D))
            {
                float perp_x = -player_dir[1];
                float perp_y = player_dir[0];
                float new_x = player_pos[0] + perp_x * move_speed;
                float new_y = player_pos[1] + perp_y * move_speed;

                if (world_map[(int)new_x][(int)player_pos[1]] == 0) player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0) player_pos[1] = new_y;
            }

            // Strafe Left (A)
            if (IsKeyDown(KEY_A))
            {
                float perp_x = player_dir[1];
                float perp_y = -player_dir[0];
                float new_x = player_pos[0] + perp_x * move_speed;
                float new_y = player_pos[1] + perp_y * move_speed;

                if (world_map[(int)new_x][(int)player_pos[1]] == 0) player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0) player_pos[1] = new_y;
            }
        }

        // Mouse editing in edit mode
        if (game_state == STATE_EDIT)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                int mouse_x = GetMouseX();
                int mouse_y = GetMouseY();
                int map_x = mouse_x / TILE_SIZE;
                int map_y = mouse_y / TILE_SIZE;

                if (map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT)
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) world_map[map_x][map_y] = 1;   // Place wall
                    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) world_map[map_x][map_y] = 0;   // Remove wall
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        if (game_state == STATE_EDIT)
        {
            // Draw map grid
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++)
                {
                    int sx = x * TILE_SIZE;
                    int sy = y * TILE_SIZE;
                    Color color = (world_map[x][y] > 0) ? WHITE : DARKGRAY;
                    DrawRectangle(sx, sy, TILE_SIZE, TILE_SIZE, color);
                    DrawRectangleLines(sx, sy, TILE_SIZE, TILE_SIZE, GRAY);
                }
            }

            // Draw player
            DrawCircle((int)(player_pos[0] * TILE_SIZE), (int)(player_pos[1] * TILE_SIZE), 8, RED);

            // Highlight cell under mouse
            int mx = GetMouseX() / TILE_SIZE;
            int my = GetMouseY() / TILE_SIZE;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT)
            {
                DrawRectangleLines(mx * TILE_SIZE, my * TILE_SIZE, TILE_SIZE, TILE_SIZE, YELLOW);
            }

            // Instructions
            DrawText("EDIT MODE", 10, 10, 30, YELLOW);
            DrawText("Left Click: Place Wall", 10, 50, 20, GREEN);
            DrawText("Right Click: Remove Wall", 10, 80, 20, RED);
            DrawText("M: PLAY mode", 10, 110, 20, WHITE);
        }
        else
        {
            DrawText("PLAY MODE - Ready for 3D Raycasting", 10, 10, 20, GREEN);
            DrawText("Press M for EDIT mode", 10, 40, 20, YELLOW);
        }

        // Current mode indicator
        const char* mode = (game_state == STATE_PLAY) ? "PLAY MODE" : "EDIT MODE";
        DrawText(mode, 10, SCREEN_HEIGHT - 30, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}