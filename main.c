// main.c
#include "raylib.h"
#include "constants.h"
#include "map.h"
#include "player.h"

int main(void)
{
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster - 2D Map View");

    // Initialize player
    init_player();

    // Set target FPS
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // ----- Input handling -----
        float move_speed = 3.0f * GetFrameTime();   // Frame rate independent
        float rot_speed  = 2.0f * GetFrameTime();   // Rotation speed

        // Toggle between play and edit mode with M key
        if (IsKeyPressed(KEY_M))
        {
            if (game_state == STATE_PLAY)
                game_state = STATE_EDIT;
            else
                game_state = STATE_PLAY;
        }

        // Only allow movement in play mode
        if (game_state == STATE_PLAY)
        {
            // Forward / Backward (W and S)
            if (IsKeyDown(KEY_W))
            {
                float new_x = player_pos[0] + player_dir[0] * move_speed;
                float new_y = player_pos[1] + player_dir[1] * move_speed;

                // Simple collision: check X and Y separately (wall sliding)
                if (world_map[(int)new_x][(int)player_pos[1]] == 0)
                    player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0)
                    player_pos[1] = new_y;
            }
            if (IsKeyDown(KEY_S))
            {
                float new_x = player_pos[0] - player_dir[0] * move_speed;
                float new_y = player_pos[1] - player_dir[1] * move_speed;

                if (world_map[(int)new_x][(int)player_pos[1]] == 0)
                    player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0)
                    player_pos[1] = new_y;
            }

            // Rotation with left/right arrow keys
            if (IsKeyDown(KEY_RIGHT))
            {
                // Rotate direction (clockwise)
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * Cos(-rot_speed) - player_dir[1] * Sin(-rot_speed);
                player_dir[1] = old_dir_x * Sin(-rot_speed) + player_dir[1] * Cos(-rot_speed);

                // Rotate plane too
                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * Cos(-rot_speed) - player_plane[1] * Sin(-rot_speed);
                player_plane[1] = old_plane_x * Sin(-rot_speed) + player_plane[1] * Cos(-rot_speed);
            }
            if (IsKeyDown(KEY_LEFT))
            {
                // Rotate direction (counter-clockwise)
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * Cos(rot_speed) - player_dir[1] * Sin(rot_speed);
                player_dir[1] = old_dir_x * Sin(rot_speed) + player_dir[1] * Cos(rot_speed);

                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * Cos(rot_speed) - player_plane[1] * Sin(rot_speed);
                player_plane[1] = old_plane_x * Sin(rot_speed) + player_plane[1] * Cos(rot_speed);
            }
        }

        // ----- Drawing -----
        BeginDrawing();
        ClearBackground(BLACK);

        if (game_state == STATE_EDIT)
        {
            // Draw full 2D map (top-down view)
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++)
                {
                    int screen_x = x * TILE_SIZE;
                    int screen_y = y * TILE_SIZE;

                    if (world_map[x][y] > 0)
                    {
                        DrawRectangle(screen_x, screen_y, TILE_SIZE, TILE_SIZE, WHITE);
                    }
                    else
                    {
                        DrawRectangle(screen_x, screen_y, TILE_SIZE, TILE_SIZE, DARKGRAY);
                    }

                    // Draw grid lines
                    DrawRectangleLines(screen_x, screen_y, TILE_SIZE, TILE_SIZE, GRAY);
                }
            }

            // Draw player as a red circle in edit mode
            int player_screen_x = (int)(player_pos[0] * TILE_SIZE);
            int player_screen_y = (int)(player_pos[1] * TILE_SIZE);
            DrawCircle(player_screen_x, player_screen_y, 8, RED);
        }
        else
        {
            // In play mode, just show a message for now
            DrawText("PLAY MODE - 3D view will come in next step", 10, 10, 20, GREEN);
            DrawText("Press M to switch to EDIT mode", 10, 40, 20, YELLOW);
        }

        // Show current mode
        const char* mode_text = (game_state == STATE_PLAY) ? "PLAY (3D coming)" : "EDIT (2D Map)";
        DrawText(mode_text, 10, SCREEN_HEIGHT - 30, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}