#include "djikstra_maps.h"

boolean wall_map[MAP_X * MAP_Y]; // True if a tile is blocked by a wall

int value_map[MAP_X * MAP_Y]; // Holds the value

// Using a static map for simplicity and enabling quick change
//  of the map layout/goals
// Legend:
// '#' - wall
// '.' - floor
// '!' - point of interest, value 0, can be multiple
// '$' - point of interest, value -5, can be multiple
const char static_map[MAP_X * MAP_Y] = 
"####################"
"#....#......#......#"
"#....#.............#"
"#...........#......#"
"#....###.########.##"
"######........#....#"
"#....#..$.....#....#"
"#.##.#........#....#"
"#....#........#....#"
"#....#.#############"
"#....#.#...........#"
"#......#......#....#"
"##.###.#..#........#"
"#....#.#.........#.#"
"#....#.#######.....#"
"#..................#"
"##############.#####"
"#..............#...#"
"#....#........!....#"
"####################";

// Relative positions of 8 tiles around n in array[20*20]
const int neighbours[8] = {-20, 1, 20, -1, -21, -19, 21, 19};

void add_to_frontier(LIST frontier_list, int pos){
    // Adds a position to the frontier list sorted from lowest to highest value
    frontier_node* node = malloc(sizeof(frontier_node)); 
    node->position = pos;

    if( isEmpty(frontier_list) || getHead(frontier_list) == getHead(frontier_list)){
    // If the list is empty or contains just one item, add it to the end
        addTail(frontier_list, (NODE)node);
    }
    else{
        // otherwise place it before any higher value position already in
        int i = 1;
        frontier_node* i_node;
        for(i_node = (frontier_node*)getHead(frontier_list);
            i_node != NULL && value_map[pos] >= value_map[((frontier_node*)i_node->node.succ)->position];
                i_node = (frontier_node*)i_node->node.succ){
            i++;
        }
        insertAfter(frontier_list, (NODE)i_node, (NODE)node);
    }
}

int main(void)
{
    // Frontier double linked list for the floodfill
    // Holds the position of unchecked tile in (MAP_X * (y) + (x)) format
    LIST frontier_list;
    frontier_list = newList();
    frontier_node* temp_node;

    // Translate the static map layout to the wall_map and value_map
    // setup values of the djikstra map
    // [-9999]              very low values for walls
    // [0]                  zero for any points of interest '!' [goals/targets/you-name-it]
    // [-5]                 -5 for any points of interest '$' - 'more important ones'
    // [INFINITE_DISTANCE]  'safe' value for free floor tiles - those we are going to change
    for (int x = 0; x < MAP_X; x++){
        for (int y = 0; y < MAP_Y; y++){
            if (x == 0 || y == 0 || x==MAP_X-1 || y==MAP_Y-1){ // redundant bonduary checking
                wall_map[MAP_COORD(x,y)] = true;
                value_map[MAP_COORD(x,y)] = -INFINITE_DISTANCE;
            }
            else if (static_map[MAP_COORD(x,y)] == '#'){
                wall_map[MAP_COORD(x,y)] = true;
                value_map[MAP_COORD(x,y)] = -INFINITE_DISTANCE;
            }
            else if (static_map[MAP_COORD(x,y)] == '!'){
                wall_map[MAP_COORD(x,y)] = false;
                value_map[MAP_COORD(x,y)] = 0;
                // add the point of interest position to the frontier
                add_to_frontier(frontier_list, MAP_COORD(x,y));
            }
            else if (static_map[MAP_COORD(x,y)] == '$'){
                wall_map[MAP_COORD(x,y)] = false;
                value_map[MAP_COORD(x,y)] = -5;
                // add the point of interest position to the frontier
                add_to_frontier(frontier_list, MAP_COORD(x,y));
            }
            else if (static_map[MAP_COORD(x,y)] == '.'){
                wall_map[MAP_COORD(x,y)] = false;
                value_map[MAP_COORD(x,y)] = INFINITE_DISTANCE;
            }
        }
    }

    // Floodfill changing the values of newly encountered tiles
    // We finish when the frontier is empty - no more tiles to check
    while( !isEmpty(frontier_list)){
        // remove the firstmost position saved in frontier
        // save it's value for this cycle of the loop
        temp_node = (frontier_node*)remHead(frontier_list);
        int current = temp_node -> position;
        for(int n = 0; n < 8; n++){ // run around the current tile checking it's neighbours
            // switch above to n < 8 for 4-way floodfill [usable for 4-way movement]
            if (current+neighbours[n] >= 0 && current+neighbours[n] < MAP_X*MAP_Y // Map bounduary checks
                && value_map[current+neighbours[n]] > value_map[current]){ // AND is checked tile's value higher than current's?
                value_map[current+neighbours[n]] = value_map[current] + 1; // Set the value of a tile

                // add the position to the frontier, sorted
                add_to_frontier(frontier_list, current+neighbours[n]);
            }
        }
    }

    free(frontier_list);

    // That's pretty much it for the Djikstra Maps, below is just the SFML/display part.

    // Window setup
    sfVideoMode mode = {WINDOW_X, WINDOW_Y, 32};
    sfRenderWindow* window;
    sfEvent event;
    window = sfRenderWindow_create(mode, WINDOW_TITLE, sfClose, NULL);
    sfRenderWindow_setVerticalSyncEnabled(window, true);

    // Load sprites
    sfTexture* texture = sfTexture_createFromFile(PATH_SPRITES, NULL);
    
    sfSprite* sprite_free = sfSprite_create();
    sfSprite_setTexture(sprite_free, texture, sfTrue);
    sfSprite_setTextureRect(sprite_free, (sfIntRect){0*TILE_SIZE, 0*TILE_SIZE, TILE_SIZE, TILE_SIZE});

    sfSprite* sprite_blocked = sfSprite_create();
    sfSprite_setTexture(sprite_blocked, texture, sfTrue);
    sfSprite_setTextureRect(sprite_blocked, (sfIntRect){1*TILE_SIZE, 0*TILE_SIZE, TILE_SIZE, TILE_SIZE});

    // Load the font
    sfFont* font = sfFont_createFromFile(PATH_FONT);
    sfText* text_value = sfText_create();
    sfText_setFont(text_value, font);
    sfText_setCharacterSize(text_value, 12);
    sfText_setColor(text_value, COLOR_FONT);

    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            // Close on ESC and clicking on the close button
            if (event.type == sfEvtClosed || (event.type == sfEvtKeyPressed && sfKeyboard_isKeyPressed(sfKeyEscape)))
                sfRenderWindow_close(window);
        }

        // Clear the window with the background color
        sfRenderWindow_clear(window, COLOR_BACKGROUND);

        // Draw
        for (int x = 0; x < MAP_X; x++){
            for (int y = 0; y < MAP_Y; y++){
                if (wall_map[MAP_COORD(x,y)] == true){
                    sfSprite_setPosition(sprite_blocked, (sfVector2f){x*TILE_SIZE, y*TILE_SIZE});
                    sfRenderWindow_drawSprite(window, sprite_blocked, NULL);
                }
                else {
                    sfSprite_setPosition(sprite_free, (sfVector2f){x*TILE_SIZE, y*TILE_SIZE});
                    sfRenderWindow_drawSprite(window, sprite_free, NULL);
                }

                // convert the value to a string
                char str_value[5];
                sprintf(str_value, "%d", value_map[MAP_COORD(x,y)]);

                // print the value on the tile
                sfText_setString(text_value, str_value);
                sfText_setPosition(text_value, (sfVector2f){TILE_SIZE * x + TILE_SIZE/2 - (sfText_getCharacterSize(text_value) * strlen(sfText_getString(text_value))/5), TILE_SIZE * y + TILE_SIZE/4});
                sfRenderWindow_drawText(window, text_value, NULL);
            }
        }

        // Display changes in the window
        sfRenderWindow_display(window);
    }

    return 0;
}