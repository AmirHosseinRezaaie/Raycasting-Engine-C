#include "constants.h"
#include "player.h"

float player_pos[2];
float player_dir[2];
float player_plane[2];

int game_state = STATE_PLAY;
int show_help = 0;

void init_player(void)
{
    player_pos[0] = PLAYER_START_X;
    player_pos[1] = PLAYER_START_Y;

    player_dir[0] = PLAYER_DIR_X;
    player_dir[1] = PLAYER_DIR_Y;

    player_plane[0] = PLAYER_PLANE_X;
    player_plane[1] = PLAYER_PLANE_Y;
}