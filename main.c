#include "raylib.h"
#include "constants.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "map_io.h"
#include <math.h>

// Draw help screen overlay
void draw_help_screen(void)
{
    // Semi-transparent dark background
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 200});
    
    int start_y = 80;
    int line_height = 30;
    
    // Title
    DrawText("=== RAYCASTER CONTROLS ===", SCREEN_WIDTH/2 - 200, start_y, 30, GOLD);
    start_y += 60;
    
    // PLAY MODE section
    DrawText("PLAY MODE (3D View):", 100, start_y, 24, SKYBLUE);
    start_y += line_height + 10;
    
    DrawText("W / S", 120, start_y, 20, GREEN);
    DrawText("- Move forward / backward", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("A / D", 120, start_y, 20, GREEN);
    DrawText("- Strafe left / right", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("ARROW KEYS", 120, start_y, 20, GREEN);
    DrawText("- Rotate camera left / right", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("M", 120, start_y, 20, GREEN);
    DrawText("- Switch to EDIT mode", 220, start_y, 20, WHITE);
    start_y += line_height + 20;
    
    // EDIT MODE section
    DrawText("EDIT MODE (2D Map Editor):", 100, start_y, 24, SKYBLUE);
    start_y += line_height + 10;
    
    DrawText("LEFT CLICK", 120, start_y, 20, GREEN);
    DrawText("- Place wall", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("RIGHT CLICK", 120, start_y, 20, GREEN);
    DrawText("- Remove wall", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("M", 120, start_y, 20, GREEN);
    DrawText("- Switch to PLAY mode", 220, start_y, 20, WHITE);
    start_y += line_height + 20;
    
    // ANYTIME section
    DrawText("ANYTIME:", 100, start_y, 24, SKYBLUE);
    start_y += line_height + 10;
    
    DrawText("F5", 120, start_y, 20, GREEN);
    DrawText("- Save map to file", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("F9", 120, start_y, 20, GREEN);
    DrawText("- Load map from file", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("H", 120, start_y, 20, GREEN);
    DrawText("- Toggle this help screen", 220, start_y, 20, WHITE);
    start_y += line_height;
    
    DrawText("ESC", 120, start_y, 20, GREEN);
    DrawText("- Quit game", 220, start_y, 20, WHITE);
    start_y += line_height + 30;
    
    // Footer
    DrawText("Press H to close", SCREEN_WIDTH/2 - 100, start_y, 20, YELLOW);
}

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

        // Toggle help screen with H key
        if (IsKeyPressed(KEY_H))
            show_help = !show_help;

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
                float perp_x = player_dir[1];
                float perp_y = -player_dir[0];
                float new_x = player_pos[0] + perp_x * move_speed;
                float new_y = player_pos[1] + perp_y * move_speed;

                if (world_map[(int)new_x][(int)player_pos[1]] == 0) player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0) player_pos[1] = new_y;
            }

            // Strafe Left (A)
            if (IsKeyDown(KEY_A))
            {
                float perp_x = -player_dir[1];
                float perp_y = player_dir[0];
                float new_x = player_pos[0] + perp_x * move_speed;
                float new_y = player_pos[1] + perp_y * move_speed;

                if (world_map[(int)new_x][(int)player_pos[1]] == 0) player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0) player_pos[1] = new_y;
            }
            
            
        }
        
        // Mouse editing in edit mode
        if (game_state == STATE_EDIT)
        {
            // Save map with F5
            if (IsKeyPressed(KEY_F5))
            {
                if (save_map("custom_map.txt"))
                {
                    printf("Map saved successfully!\n");
                }
                else
                {
                    printf("Failed to save map!\n");
                }
            }
    
            // Load map with F9
            if (IsKeyPressed(KEY_F9))
            {
                if (load_map("custom_map.txt"))
                {
                    printf("Map loaded successfully!\n");
                }
                else
                {
                    printf("Failed to load map!\n");
                }
            }
            
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
        // Clamp player position to map bounds
        if (player_pos[0] < 0.1f) player_pos[0] = 0.1f;
        if (player_pos[0] > MAP_WIDTH - 1.1f) player_pos[0] = MAP_WIDTH - 1.1f;
        if (player_pos[1] < 0.1f) player_pos[1] = 0.1f;
        if (player_pos[1] > MAP_HEIGHT - 1.1f) player_pos[1] = MAP_HEIGHT - 1.1f;

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

            // Draw player direction
            Vector2 player_screen = {(int)(player_pos[0] * TILE_SIZE), (int)(player_pos[1] * TILE_SIZE)};
            Vector2 dir_end = {player_screen.x + player_dir[0] * 30, player_screen.y + player_dir[1] * 30};
            DrawLineV(player_screen, dir_end, RED);

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
            DrawText("M: PLAY mode", 10, 110, 20, RED);
            DrawText("F5: Save | F9: Load", 10, 140, 20, SKYBLUE);
            DrawText("H: Help", 10, 170, 20, SKYBLUE);
        }

        else  // STATE_PLAY
        {
            render_raycast_scene();
            
            DrawText("W A S D: Move | Arrows: Rotate | M: Edit | H: Help", 10, 10, 20, YELLOW);
            
            // ===== MINI-MAP (Bottom-Right Corner) =====
            int minimap_size = 4;
            int minimap_offset_x = SCREEN_WIDTH - (MAP_WIDTH * minimap_size) - 10;
            int minimap_offset_y = SCREEN_HEIGHT - (MAP_HEIGHT * minimap_size) - 10;
            
            // Draw semi-transparent background
            DrawRectangle(minimap_offset_x - 5, minimap_offset_y - 5, 
                         MAP_WIDTH * minimap_size + 10, 
                         MAP_HEIGHT * minimap_size + 10, 
                         (Color){0, 0, 0, 150});
            
            // Draw map cells
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                for (int y = 0; y < MAP_HEIGHT; y++)
                {
                    Color cell_color = (world_map[x][y] > 0) ? WHITE : DARKGRAY;
                    DrawRectangle(minimap_offset_x + x * minimap_size, 
                                 minimap_offset_y + y * minimap_size,
                                 minimap_size, minimap_size, cell_color);
                }
            }
            
            // Draw player position (red dot)
            int player_x = minimap_offset_x + (int)(player_pos[0] * minimap_size);
            int player_y = minimap_offset_y + (int)(player_pos[1] * minimap_size);
            DrawCircle(player_x, player_y, 3, RED);
            
            // Draw player direction (red line)
            int dir_end_x = player_x + (int)(player_dir[0] * 10);
            int dir_end_y = player_y + (int)(player_dir[1] * 10);
            DrawLine(player_x, player_y, dir_end_x, dir_end_y, RED);
        }

        // Current mode indicator
        const char* mode = (game_state == STATE_PLAY) ? "PLAY MODE" : "EDIT MODE";
        DrawText(mode, 10, SCREEN_HEIGHT - 30, 20, YELLOW);

        // Draw help screen if toggled
        if (show_help)
        {
            draw_help_screen();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}