// main.c
#include "raylib.h"
#include "constants.h"
#include "map.h"
#include "player.h"
#include <math.h>

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
                player_dir[0] = player_dir[0] * cos(-rot_speed) - player_dir[1] * sin(-rot_speed);
                player_dir[1] = old_dir_x * sin(-rot_speed) + player_dir[1] * cos(-rot_speed);

                // Rotate plane too
                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * cos(-rot_speed) - player_plane[1] * sin(-rot_speed);
                player_plane[1] = old_plane_x * sin(-rot_speed) + player_plane[1] * cos(-rot_speed);
            }
            if (IsKeyDown(KEY_LEFT))
            {
                // Rotate direction (counter-clockwise)
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * cos(rot_speed) - player_dir[1] * sin(rot_speed);
                player_dir[1] = old_dir_x * sin(rot_speed) + player_dir[1] * cos(rot_speed);

                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * cos(rot_speed) - player_plane[1] * sin(rot_speed);
                player_plane[1] = old_plane_x * sin(rot_speed) + player_plane[1] * cos(rot_speed);
            }
        }
        // Mouse editing in EDIT mode
        // We only edit the map when in edit mode
        if (game_state == STATE_EDIT)
        {
            // Check if left or right mouse button is pressed (only once per click)
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                // Get mouse position on screen
                int mouse_x = GetMouseX();
                int mouse_y = GetMouseY();

                // Convert pixel position to map grid index
                // We use integer division to find which cell the mouse is over
                int map_x = mouse_x / TILE_SIZE;
                int map_y = mouse_y / TILE_SIZE;

                // Check if the map index is valid (inside the map bounds)
                // This prevents crash if mouse is outside the map
                if (map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT)
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        // Left click: place wall (set to 1)
                        world_map[map_x][map_y] = 1;
                    }
                    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                    {
                        // Right click: remove wall (set to 0)
                        world_map[map_x][map_y] = 0;
                    }
                }
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

            // Highlight cell under mouse
            // This helps user see which cell they are clicking
            int mouse_x = GetMouseX();
            int mouse_y = GetMouseY();
            int highlight_map_x = mouse_x / TILE_SIZE;
            int highlight_map_y = mouse_y / TILE_SIZE;

            // Only highlight if mouse is over a valid cell
            if (highlight_map_x >= 0 && highlight_map_x < MAP_WIDTH &&
                highlight_map_y >= 0 && highlight_map_y < MAP_HEIGHT)
            {
                int highlight_screen_x = highlight_map_x * TILE_SIZE;
                int highlight_screen_y = highlight_map_y * TILE_SIZE;
                // Draw yellow border around the cell
                DrawRectangleLines(highlight_screen_x, highlight_screen_y, TILE_SIZE, TILE_SIZE, YELLOW);
            }

            // ----- Show instructions in edit mode -----
            DrawText("EDIT MODE", 10, 10, 30, YELLOW);
            DrawText("Left Click: Place Wall", 10, 50, 20, GREEN);
            DrawText("Right Click: Remove Wall", 10, 80, 20, RED);
            DrawText("Press M to return to PLAY mode", 10, 110, 20, WHITE);
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