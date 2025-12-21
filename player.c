#include "player.h"

Player player;

// Initialize player at starting position
void init_player(void) {
    player.pos = (Vector2){22.0f, 12.0f};        // Start in open area
    player.dir = (Vector2){-1.0f, 0.0f};        // Looking left (west)
    player.plane = (Vector2){0.0f, 0.66f};       // FOV ~66 degrees
}