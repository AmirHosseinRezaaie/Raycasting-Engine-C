#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"

extern float player_pos[2];
extern float player_dir[2];
extern float player_plane[2];
extern int game_state;
extern int show_help;

void init_player(void);

#endif