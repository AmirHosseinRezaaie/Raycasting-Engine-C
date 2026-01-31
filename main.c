#include <math.h>
#include "raylib.h"
#include "constants.h"
#include "map.h"
#include "map_io.h"
#include "player.h"
#include "raycaster.h"

void draw_help_screen(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 200});
    
    int start_y = 80;
    int line_height = 30;
    
    DrawText("=== RAYCASTER CONTROLS ===", SCREEN_WIDTH / 2 - 200, start_y, 30, GOLD);
    start_y += 60;
    
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
    
    DrawText("Press H to close", SCREEN_WIDTH / 2 - 100, start_y, 20, YELLOW);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster - 2D Map Editor");
    init_player();
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        float move_speed = MOVE_SPEED * GetFrameTime();
        float rot_speed = ROTATION_SPEED * GetFrameTime();

        if (IsKeyPressed(KEY_M))
            game_state = (game_state == STATE_PLAY) ? STATE_EDIT : STATE_PLAY;

        if (IsKeyPressed(KEY_H))
            show_help = !show_help;

        if (game_state == STATE_PLAY)
        {
            if (IsKeyDown(KEY_W))
            {
                float new_x = player_pos[0] + player_dir[0] * move_speed;
                float new_y = player_pos[1] + player_dir[1] * move_speed;
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

            if (IsKeyDown(KEY_RIGHT))
            {
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * cos(-rot_speed) - player_dir[1] * sin(-rot_speed);
                player_dir[1] = old_dir_x * sin(-rot_speed) + player_dir[1] * cos(-rot_speed);

                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * cos(-rot_speed) - player_plane[1] * sin(-rot_speed);
                player_plane[1] = old_plane_x * sin(-rot_speed) + player_plane[1] * cos(-rot_speed);
            }

            if (IsKeyDown(KEY_LEFT))
            {
                float old_dir_x = player_dir[0];
                player_dir[0] = player_dir[0] * cos(rot_speed) - player_dir[1] * sin(rot_speed);
                player_dir[1] = old_dir_x * sin(rot_speed) + player_dir[1] * cos(rot_speed);

                float old_plane_x = player_plane[0];
                player_plane[0] = player_plane[0] * cos(rot_speed) - player_plane[1] * sin(rot_speed);
                player_plane[1] = old_plane_x * sin(rot_speed) + player_plane[1] * cos(rot_speed);
            }

            if (IsKeyDown(KEY_D))
            {
                float perp_x = player_dir[1];
                float perp_y = -player_dir[0];
                float new_x = player_pos[0] + perp_x * move_speed;
                float new_y = player_pos[1] + perp_y * move_speed;
                if (world_map[(int)new_x][(int)player_pos[1]] == 0)
                    player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0)
                    player_pos[1] = new_y;
            }

            if (IsKeyDown(KEY_A))
            {
                float perp_x = -player_dir[1];
                float perp_y = player_dir[0];
                float new_x = player_pos[0] + perp_x * move_speed;
                float new_y = player_pos[1] + perp_y * move_speed;
                if (world_map[(int)new_x][(int)player_pos[1]] == 0)
                    player_pos[0] = new_x;
                if (world_map[(int)player_pos[0]][(int)new_y] == 0)
                    player_pos[1] = new_y;
            }
        }
        
        if (game_state == STATE_EDIT)
        {
            if (IsKeyPressed(KEY_F5))
            {
                if (save_map(MAP_FILENAME))
                    printf("Map saved successfully!\n");
                else
                    printf("Failed to save map!\n");
            }
    
            if (IsKeyPressed(KEY_F9))
            {
                if (load_map(MAP_FILENAME))
                    printf("Map loaded successfully!\n");
                else
                    printf("Failed to load map!\n");
            }
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                int mouse_x = GetMouseX();
                int mouse_y = GetMouseY();
                int map_x = mouse_x / TILE_SIZE;
                int map_y = mouse_y / TILE_SIZE;
                
                if (map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT)
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        world_map[map_x][map_y] = WALL_VALUE;
                    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                        world_map[map_x][map_y] = EMPTY_VALUE;
                }
            }
        }

        if (player_pos[0] < PLAYER_BOUNDS_MIN)
            player_pos[0] = PLAYER_BOUNDS_MIN;
        if (player_pos[0] > MAP_WIDTH - PLAYER_BOUNDS_OFFSET)
            player_pos[0] = MAP_WIDTH - PLAYER_BOUNDS_OFFSET;
        if (player_pos[1] < PLAYER_BOUNDS_MIN)
            player_pos[1] = PLAYER_BOUNDS_MIN;
        if (player_pos[1] > MAP_HEIGHT - PLAYER_BOUNDS_OFFSET)
            player_pos[1] = MAP_HEIGHT - PLAYER_BOUNDS_OFFSET;

        BeginDrawing();
        ClearBackground(BLACK);

        if (game_state == STATE_EDIT)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++)
                {
                    int screen_x = x * TILE_SIZE;
                    int screen_y = y * TILE_SIZE;
                    Color color = (world_map[x][y] > 0) ? WHITE : DARKGRAY;
                    DrawRectangle(screen_x, screen_y, TILE_SIZE, TILE_SIZE, color);
                    DrawRectangleLines(screen_x, screen_y, TILE_SIZE, TILE_SIZE, GRAY);
                }
            }

            DrawCircle((int)(player_pos[0] * TILE_SIZE), (int)(player_pos[1] * TILE_SIZE), 
                      PLAYER_CIRCLE_RADIUS, RED);

            Vector2 player_screen = {
                (int)(player_pos[0] * TILE_SIZE), 
                (int)(player_pos[1] * TILE_SIZE)
            };
            Vector2 dir_end = {
                player_screen.x + player_dir[0] * DIRECTION_LINE_LENGTH, 
                player_screen.y + player_dir[1] * DIRECTION_LINE_LENGTH
            };
            DrawLineV(player_screen, dir_end, RED);

            int mouse_x = GetMouseX() / TILE_SIZE;
            int mouse_y = GetMouseY() / TILE_SIZE;
            if (mouse_x >= 0 && mouse_x < MAP_WIDTH && mouse_y >= 0 && mouse_y < MAP_HEIGHT)
            {
                DrawRectangleLines(mouse_x * TILE_SIZE, mouse_y * TILE_SIZE, 
                                  TILE_SIZE, TILE_SIZE, YELLOW);
            }

            DrawText("EDIT MODE", 10, 10, 30, YELLOW);
            DrawText("Left Click: Place Wall", 10, 50, 20, GREEN);
            DrawText("Right Click: Remove Wall", 10, 80, 20, RED);
            DrawText("M: PLAY mode", 10, 110, 20, RED);
            DrawText("F5: Save | F9: Load", 10, 140, 20, SKYBLUE);
            DrawText("H: Help", 10, 170, 20, SKYBLUE);
        }
        else
        {
            render_raycast_scene();
            
            DrawText("W A S D: Move | Arrows: Rotate | M: Edit | H: Help", 
                    10, 10, 20, YELLOW);
            
            int minimap_size = MINIMAP_CELL_SIZE;
            int minimap_offset_x = SCREEN_WIDTH - (MAP_WIDTH * minimap_size) - MINIMAP_PADDING;
            int minimap_offset_y = SCREEN_HEIGHT - (MAP_HEIGHT * minimap_size) - MINIMAP_PADDING;
            
            DrawRectangle(minimap_offset_x - MINIMAP_BORDER, minimap_offset_y - MINIMAP_BORDER, 
                         MAP_WIDTH * minimap_size + MINIMAP_BORDER * 2, 
                         MAP_HEIGHT * minimap_size + MINIMAP_BORDER * 2, 
                         (Color){0, 0, 0, 150});
            
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
            
            int player_map_x = minimap_offset_x + (int)(player_pos[0] * minimap_size);
            int player_map_y = minimap_offset_y + (int)(player_pos[1] * minimap_size);
            DrawCircle(player_map_x, player_map_y, MINIMAP_PLAYER_RADIUS, RED);
            
            int dir_end_x = player_map_x + (int)(player_dir[0] * MINIMAP_DIR_LENGTH);
            int dir_end_y = player_map_y + (int)(player_dir[1] * MINIMAP_DIR_LENGTH);
            DrawLine(player_map_x, player_map_y, dir_end_x, dir_end_y, RED);
        }

        const char *mode = (game_state == STATE_PLAY) ? "PLAY MODE" : "EDIT MODE";
        DrawText(mode, 10, SCREEN_HEIGHT - 30, 20, YELLOW);

        if (show_help)
            draw_help_screen();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}