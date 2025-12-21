// player.c
#include "player.h"

// Global player variables - we use arrays instead of struct
float player_pos[2];
float player_dir[2];
float player_plane[2];

int game_state = STATE_PLAY;  // شروع در حالت بازی (3D)

void init_player(void)
{
    // Start position in the middle of an empty area
    player_pos[0] = 22.0f;
    player_pos[1] = 12.0f;

    // Initially looking left (negative X direction)
    player_dir[0] = -1.0f;
    player_dir[1] = 0.0f;

    // Camera plane for ~66 degree FOV
    // plane is perpendicular to direction
    player_plane[0] = 0.0f;
    player_plane[1] = 0.66f;
}