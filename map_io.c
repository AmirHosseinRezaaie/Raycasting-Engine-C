#include <stdio.h>
#include "map.h"
#include "map_io.h"

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
    return true;
}