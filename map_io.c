#include <stdio.h>
#include <math.h>
#include "map.h"
#include "map_io.h"
#include "player.h"
#include "constants.h"

bool save_map(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
        return false;

    fprintf(file, "%d %d\n", MAP_WIDTH, MAP_HEIGHT);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            fprintf(file, "%d", world_map[x][y]);
            if (x < MAP_WIDTH - 1)
                fprintf(file, " ");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

bool load_map(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return false;

    int width, height;
    if (fscanf(file, "%d %d", &width, &height) != 2)
    {
        fclose(file);
        return false;
    }

    if (width != MAP_WIDTH || height != MAP_HEIGHT)
    {
        fclose(file);
        return false;
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (fscanf(file, "%d", &world_map[x][y]) != 1)
            {
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);

    int player_cell_x = (int)player_pos[0];
    int player_cell_y = (int)player_pos[1];

    if (player_cell_x >= 0 && player_cell_x < MAP_WIDTH &&
        player_cell_y >= 0 && player_cell_y < MAP_HEIGHT &&
        world_map[player_cell_x][player_cell_y] != EMPTY_VALUE)
    {
        world_map[player_cell_x][player_cell_y] = EMPTY_VALUE;
        printf("Note: Cleared wall at player position (%d, %d)\n", 
            player_cell_x, player_cell_y);
    }

    return true;
}