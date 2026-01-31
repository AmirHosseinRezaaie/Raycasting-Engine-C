// map_io.h
#ifndef MAP_IO_H
#define MAP_IO_H

#include <stdbool.h>

bool save_map(const char* filename);
bool load_map(const char* filename);

#endif