#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH  24
#define MAP_HEIGHT 24
#define TILE_SIZE 32

extern int world_map[MAP_HEIGHT][MAP_WIDTH];

void init_map(void);
void draw_minimap(void);
int is_wall(int x, int y);

#endif