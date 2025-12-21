#ifndef PLAYER_H
#define PLAYER_H

// We use Vector2 from raylib because it's simple and we are allowed
typedef struct Player {
    Vector2 pos;    // Player position in world (float)
    Vector2 dir;    // Direction vector (unit vector)
    Vector2 plane;  // Camera plane (perpendicular to dir)
} Player;

extern Player player;

void init_player(void);

#endif