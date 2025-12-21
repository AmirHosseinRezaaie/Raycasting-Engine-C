// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"

// Player position: pos[0] = x, pos[1] = y
extern float player_pos[2];

// Direction vector: dir[0] = x, dir[1] = y (unit vector)
extern float player_dir[2];

// Camera plane: plane[0] = x, plane[1] = y (perpendicular to dir)
extern float player_plane[2];

// Current game state
extern int game_state;

void init_player(void);

#endif