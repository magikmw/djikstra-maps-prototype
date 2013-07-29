#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strlen() when printing the values on the map

#include <SFML/Graphics.h>

#include "double_linked_list.h"

#define WINDOW_TITLE         "Djikstra Maps Prototype"
#define PATH_SPRITES        "sprites.png"
#define PATH_FONT           "mandrill.ttf"

#define MAP_X               20              // Size of the map in tiles
#define MAP_Y               20              // If you want to change the size
                                            // of the map, make sure to update
                                            // the static_map[] in the .c file
                                            // and relative positions in
                                            // int neighbours[]

#define TILE_SIZE           32              // Tile size in pixels

#define WINDOW_X            MAP_X*TILE_SIZE // Window size in pixels
#define WINDOW_Y            MAP_Y*TILE_SIZE

#define INFINITE_DISTANCE   MAP_X*MAP_Y+1   // Max possible travel distance
                                            // Used to reset the value map

#define boolean             char            // Simple bool 'type'
#define true                1
#define false               0

#define MAP_COORD(x, y) (MAP_X * (y) + (x)) // Map coordinate helper
                                            // Converts (x,y) position to
                                            // an index in array[MAP_X * MAP_Y]

#define COLOR_BACKGROUND    (sfColor){78, 78, 98, 255}
#define COLOR_FONT          sfMagenta

typedef struct{
    struct MNode node;
    int position;
} frontier_node;

void add_to_frontier(LIST, int);