//=================================================================
// The main program file.
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// PROJECT INCLUDES
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "viper.h"

#include <math.h>
#include <stdio.h>

// Important definitions
#define DEBUG 1;

/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Viper viper;
int status = NEUTRAL; // status of the viper based on the boost items
int status_color; // change the border's color to reflect status changes
int boost_timer = 0; // boost timer for boost up/down
int debug_status = 0; // toggle debug mode

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char* wav);
int get_action(GameInputs inputs);
int update_game(int action);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DEFINITIONS
/**
 * Program entry point! This is where it all begins.
 * This function is for all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
    viper_init(&viper);
    
    // 0. Initialize the maps
    // TODO: implement maps_init() and init_main_map() function in map.cpp:
    maps_init();
    init_main_map();

    // Initialize game state
    // set_active_map(0);
    // viper.head_x = viper.head_y = 5;
    
    // Initial drawing
    draw_game(FULL_DRAW);

    // Main game loop
    while(1) {        
        // Timer to measure game update speed
        Timer t;
        t.start();

        // 1. Read inputs 
        //TODO: implement read_inputs() function in hardware.cpp:
        GameInputs inputs = read_inputs();
        
        // 2. Determine action (move, act, menu, etc.)
        //TODO: implement get_action() function:
        int action = get_action(inputs);
        
        // 3. Update game
        //TODO: implement update_game() function:
        int result = update_game(action);

        uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
        char str[1024];
        if (debug_status) {
            snprintf(str,sizeof(str),"Pos:%d,%d DEBUG",viper.head_x,viper.head_y);
        } else {
            snprintf(str,sizeof(str),"Pos:%d,%d Score:%d",viper.head_x,viper.head_y,viper.score);
        }
        uLCD.text_string(str,0,0,FONT_7X8,RED);
        
        // printf("Snake current length: %d\r\n", viper.length);
        // printf("Viper previous location (%d, %d)\r\n", viper.head_px, viper.head_py);
        // printf("%d\r\n", result);

        // 3b. Check for game over based on result
        // and if so, handle game over
        if (result == GAME_OVER) {
            // are we in debug mode?
            if (debug_status) {
                viper.head_x = viper.head_px;
                viper.head_y = viper.head_py;
            } else {
                uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
                snprintf(str,sizeof(str),"Game Over");
                uLCD.text_string(str,5,8,FONT_7X8,RED);
                break;
            }
        }

        //TODO: implement this here or as a new function.
        
        //      3b1. if game is over, then
        //      3b2. draw and display tezt for game over sign
                
        // 4. Draw screen -- provided.
        draw_game(result);
        
        // start/increment the timer when viper receives boost
        if (status) {
            draw_border();
            boost_timer++; 
        }

        // after some time has passed, remove the boost
        if (boost_timer == 50) {
            boost_timer = 0; // end the timer
            status = NEUTRAL;
            draw_border(); // redrawn the border based on the status
        }

        // Compute update time
        t.stop();
        int dt = t.read_ms();

        // Display and wait
        // NOTE: Text is 8 pixels tall
        if (dt < 100) wait_ms(100 - dt);
    }
}

/**
 * Plays a .wav file
 */
void playSound(char* wav)
{
    // OPTIONAL: Implement
    
}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
int get_action(GameInputs inputs)
{
    // TODO: Implement
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    // 2. Check for your navigation switch inputs and return the corresponding action value
    if (inputs.b1) {
        return ACTION_BUTTON; // Implement DEBUG MODE HERE
    } else if (inputs.b2) {
        return MENU_BUTTON; // Implement MENU BUTTON
    } else if (inputs.ns_left) {
        return GO_LEFT;
    } else if (inputs.ns_right) {
        return GO_RIGHT;
    } else if (inputs.ns_up) {
        return GO_UP;
    } else if (inputs.ns_down) {
        return GO_DOWN;
    } else {
        // If no button is pressed, just return no action value
        return NO_ACTION;
    }
}

/**
 * This function is called by update game to check when the snake 
 * gets an object (in other words, when a snake eats an object). We 
 * update the snake and game conditions as needed based on the object type
 */
int get_object(){
    
    //TODO: Implement
    // 1. Get item at current head location
    MapItem* currItem = get_current(viper.head_x, viper.head_y);
    
    // 2. Initialize a return value variable to denote the item type we got
    int objectType;
    
    // 3. Check if item is valid and is a chest
    //    3a. if so, increment the score, extend the length of the viper
    //    3b. Remove the chest
    //    3c. set the return value variable to GOT_LENGTH

    // 4. Else, check if the item is valid a boost-up/boost-down
    //    4a. if so, set the ability for the viper based on the type of item
    //          for the right amount fo time
    //    4b. Remove the boost-up/down item
    //    4c. Set the return value variable to GOT_OBJ
    //    Hint: You have to do this check for all your boost-ups/downs. Might be 
    //          easier to use Switch statements.
    
    if (currItem->type == CHEST) {
        if (status != BOOST_DOWN) viper.score++;
        viper.length++;
        map_erase(viper.head_x, viper.head_y);
        objectType = GOT_LENGTH;
    } 

    switch(currItem->type) {
        case PLANT: // PLANT is boost up
            objectType = GOT_OBJ;
            status = BOOST_UP;
            boost_timer = 0; // reset timer
            map_erase(viper.head_x, viper.head_y);
            break;
        case WATER: // WATER is boost down
            objectType = GOT_OBJ;
            status = BOOST_DOWN;
            boost_timer = 0; // reset timer
            map_erase(viper.head_x, viper.head_y);
            break; 
    }
    

    // 5. Check if the return value variable is GOT_LENGTH
    //    5a. If so, then increase the snake length and update its locations as needed
    //        Hint: Refer to Step 6 below; the main difference is that you need 
    //        to lengthen the snake (what should we change the snake tail position to?)
    
    // save original location of the viper's tail
        int tail_x = viper.locations[viper.length - 2].x;
        int tail_y = viper.locations[viper.length - 2].y;
        // iterate each location of the viper's body
        for (int i = viper.length - 2; i > 0; i--) {
            // move forward current body's location to the next body's location
            viper.locations[i].x = viper.locations[i - 1].x;
            viper.locations[i].y = viper.locations[i - 1].y;
        }
        // update initial viper location to the previous head's location
        viper.locations[0].x = viper.head_px;
        viper.locations[0].y = viper.head_py;
        // add a viper head to head's location
        add_viper_head(viper.head_x, viper.head_y);
    
    if (objectType == GOT_LENGTH) {
        // add a viper body (including the original tail) to the location after the head
        for (int i = 0; i < viper.length - 1; i++) {
            add_viper_body(viper.locations[i].x, viper.locations[i].y);
        }
        // add a viper tail to the original's tail location
        add_viper_tail(tail_x, tail_y);
    }

    // 6. For any other object, do the following default steps
    //    6a. initialize two variables to to denote the original location for the x and y tail of the viper
    //    6b. For each location of the viper body, move them forward by one location
    //    6c. Update the initial viper location to the viper head x and y location
    //    6d. call a function to add a viper head given the head x and y location
    //    6e. call a function to add a viper body to the location after the head
    //    6f. call a function to add a viper tail to the location of the last viper locations
    //    6g. call a function to map erase the original location for the x and y tail
    //    6h. return the return value variable

    else {
        // add a viper body to the location after the head
        for (int i = 0; i < viper.length - 2; i++) {
            add_viper_body(viper.locations[i].x, viper.locations[i].y);
        }
        // add a viper tail to the last body's location
        add_viper_tail(viper.locations[viper.length - 2].x, viper.locations[viper.length - 2].y);
        // erase the original location of the tail
        map_erase(tail_x, tail_y);
    }

    return objectType;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the viper position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the viper has not moved.
 */ 
int update_game(int action)
{
    // TODO: Implement
    
    // 1. Check the viper speed and update viper speed status
    // implement this later

    // DEBUG mode (Permanent boost-up)
    if (action == ACTION_BUTTON) {
        if (debug_status) debug_status = 0;
        else debug_status = DEBUG; // toggle debug mode
    }
    
    // 2. Update the previous viper head position to the current position
    viper.head_px = viper.head_x;
    viper.head_py = viper.head_y;
    
    // 3. Check if action is to go up
            
    //      3a. Get item north of the current viper head
    //      3b. Update the current viper head position upwards
    //      3c. Check if the item exist and item is walkable
    //          3c1. Check if the viper head does not collide with itself
    //          3c2. If so, return game over
    //          3c3. Otherwise, update viper head back to its original position
    //               and return no result.
    //      3d. Check if the viper gets an object (eats) by calling another function
    //          3d1. Check if viper length is more than the maximum length
    //          3d2. If so, return game over
    //  ````3e. Finally, return moved since the viper has moved
     
    // 4. Else check if action is to go left, right, and down
    //    NOTE: Repeat same general steps as in go up          
    
    // If no action was given, then we simply return no result
    
    MapItem* checkItem;
    int objectType;
    switch (action) {
        case GO_UP:
            checkItem = get_north(viper.head_x, viper.head_y);
            viper.head_y--;
            if (checkItem && !checkItem->walkable) {
                // check for invalid movement/viper is invincible
                if (status == BOOST_UP || viper.head_y == viper.locations[0].y) {
                    viper.head_y++;
                    return NO_RESULT;
                }
                return GAME_OVER;
            }
            objectType = get_object();
            if (objectType == GOT_LENGTH) {
                if (viper.length > VIPER_MAX_LENGTH) {
                    return GAME_OVER;
                }
            }
            return MOVED;
        case GO_DOWN:
            checkItem = get_south(viper.head_x, viper.head_y);
            viper.head_y++;
            if (checkItem && !checkItem->walkable) {
                // check for invalid movement/viper is invincible
                if (status == BOOST_UP || viper.head_y == viper.locations[0].y) {
                    viper.head_y--;
                    return NO_RESULT;
                }
                return GAME_OVER;
            }
            objectType = get_object();
            if (objectType == GOT_LENGTH) {
                if (viper.length > VIPER_MAX_LENGTH) {
                    return GAME_OVER;
                }
            }
            return MOVED;
        case GO_LEFT:
            checkItem = get_west(viper.head_x, viper.head_y);
            viper.head_x--;
            if (checkItem && !checkItem->walkable) {
                // check for invalid movement/viper is invincible
                if (status == BOOST_UP || viper.head_x == viper.locations[0].x) {
                    viper.head_x++;
                    return NO_RESULT;
                }
                return GAME_OVER;
            }
            objectType = get_object();
            if (objectType == GOT_LENGTH) {
                if (viper.length > VIPER_MAX_LENGTH) {
                    return GAME_OVER;
                }
            }
            return MOVED;
        case GO_RIGHT:
            checkItem = get_east(viper.head_x, viper.head_y);
            viper.head_x++;
            if (checkItem && !checkItem->walkable) {
                // check for invalid movement/viper is invincible
                if (status == BOOST_UP || viper.head_x == viper.locations[0].x) {
                    viper.head_x--;
                    return NO_RESULT;
                }
                return GAME_OVER;
            }
            objectType = get_object();
            if (objectType == GOT_LENGTH) {
                if (viper.length > VIPER_MAX_LENGTH) {
                    return GAME_OVER;
                }
            }
            return MOVED;
        default:
            // no action was given
            return NO_RESULT;
    }  
}

/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    uLCD.line(0, 9, 127, 9, GREEN);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.line(0, 118, 127, 118, GREEN);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    switch (status) {
        case NEUTRAL:
            status_color = WHITE;
            break;
        case BOOST_UP:
            status_color = RED;
            break;
        case BOOST_DOWN:
            status_color = LGREY;
            break;
    }

    uLCD.filled_rectangle(  0,   9, 127,  14, status_color);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, status_color);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, status_color);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, status_color);   // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)
{
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_viper_head(u, v);
        draw_viper_body(u-11, v);
        draw_viper_tail(u-22, v);
        return;
    }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + viper.head_x;
            int y = j + viper.head_y;

            // Compute the previous map (px, py) of this tile
            int px = i + viper.head_px;
            int py = j + viper.head_py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);

    // add viper's food
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_chest(i % map_width(), i / map_width());
    }
    
    // add boost-up/down items
    for(int i = map_width() + 3; i < map_area(); i += 65) {
        add_plant(i % map_width(), i / map_width());
    }

    for(int i = map_width() + 3; i < map_area(); i += 92) {
        add_water(i % map_width(), i / map_width());
    }

    pc.printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    pc.printf("Walls done!\r\n");
    
    add_viper_head(viper.head_x, viper.head_y);
    add_viper_body(viper.locations[0].x, viper.locations[0].y);
    add_viper_tail(viper.locations[1].x, viper.locations[1].y);
    
    pc.printf("Add extra chamber\r\n");
    add_wall(30,  0,   VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39,  0,   VERTICAL, 10);
    pc.printf("Added!\r\n");
    
}