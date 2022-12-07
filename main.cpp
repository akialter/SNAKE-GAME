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
#include "hash_table.h"
#include "map.h"
#include "graphics.h"
#include "viper.h"
#include "angry_viper.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h> // for random function
#include <ctime>    // seed different random generator

// Important definitions
#define DEBUG 1;

/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Viper viper; // the HUNGRY VIPER
AngryViper angry_viper; // the ANGRY VIPER
int game_status = GAME_MENU; // check to see if we are in menu option
int viper_status = NEUTRAL; // status of the viper based on the boost items
int screen_status = 0; // check to see if the screen needs to be cleared
int status_color; // change the border's color to reflect status changes
int boost_timer = 0; // boost timer for boost up/down
int debug_status = 0; // toggle debug mode
int previous_action = NO_ACTION; // hold previous action for sticky control
int current_action = NO_ACTION; // hold current action for sticky control
int arrow_status = START; // the current location of the arrow (in menu screen selection)
int game_mode = HARD; // difficulty mode (EASY, MEDIUM, HARD)
int new_game = 1; // check if the game has just started
int angry_move_state = ANGRY_MOVE_RIGHT; // moving state of the angry viper (in a 5x5 rectangle)
int out_of_frame = 0; // don't have to update the angry viper if it's not visible


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
void init_game();

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

    // Initialize the game
    init_game();

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

        // start/increment the timer when viper receives boost
        if (viper_status && game_status == GAME_PLAY) {
            boost_timer++; 
        }

        // after some time has passed, remove the boost
        if (boost_timer == 30) {
            boost_timer = 0; // end the timer
            viper_status = NEUTRAL;
        }

        // 3b. Check for game over based on result
        // if we are in debug mode, the game is not over    
        if (result == GAME_OVER) {
            if (debug_status || viper_status == BOOST_UP) {
                viper.head_x = viper.head_px;
                viper.head_y = viper.head_py;
            } else {
                result = DRAW_OVER;
                game_status = GAME_OVER;
                screen_status = 0;
            }
        }

        //TODO: implement this here or as a new function.
        
        //      3b1. if game is over, then
        //      3b2. draw and display text for game over sign
                

        // 4. Draw screen -- provided.
        draw_game(result);

        // Compute update time
        t.stop();
        int dt = t.read_ms();

        // Display and wait
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
 * Initialize the map (when the game start or restart)
*/
void init_game() {
    // seed random generator 
    srand(time(NULL));

    // if we restarted the game
    if (!new_game) {
        // destroy current active map's items
        map_destroy();
        // clear screen
        screen_status = 0;
        // reset all parameters
        viper_status = NEUTRAL; // status of the viper based on the boost items
        boost_timer = 0; // boost timer for boost up/down
        debug_status = 0; // toggle debug mode
        previous_action = NO_ACTION; // hold previous action for sticky control
        current_action = NO_ACTION; // hold current action for sticky control
        arrow_status = START; // the current location of the arrow (in menu screen selection)
        game_mode = HARD; // difficulty mode (EASY, MEDIUM, HARD)
        angry_move_state = ANGRY_MOVE_RIGHT; // moving state of the angry viper (in a 5x5 rectangle)
        out_of_frame = 0; // don't have to update the angry viper if it's not visible
        new_game = 1; // set to new game (start over)
    }
    
    // Initialize the maps
    viper_init(&viper);
    angry_viper_init(&angry_viper);
    maps_init();
    init_main_map();

    // Initialize game state
    game_status = GAME_START;
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
    switch(game_status) {
        case GAME_START:
            if (inputs.ns_center) {
                return PRESS; // press the nav switch button to select
            } else if (inputs.ns_up) {
                return GO_UP;
            } else if (inputs.ns_down) {
                return GO_DOWN;
            } else {
                // If no button is pressed, just return no action value
                return NO_ACTION;
            }
        case GAME_MENU:
            if (inputs.ns_center) {
                return PRESS; // press the nav switch button to select
            } else if (inputs.ns_up) {
                return GO_UP;
            } else if (inputs.ns_down) {
                return GO_DOWN;
            } else {
                // If no button is pressed, just return no action value
                return NO_ACTION;
            }
        case GAME_PLAY:
            // TODO: Implement
            previous_action = current_action;
            // 1. Check your action and menu button inputs and return the corresponding action value
            // 2. Check for your navigation switch inputs and return the corresponding action value
            if (inputs.b1) {
                wait(0.2); // switch debounce
                return ACTION_BUTTON; // Implement DEBUG MODE HERE
            } else if (inputs.b2) {
                game_status = (!new_game) ? GAME_MENU : GAME_PLAY; // if we just start the game, no reason to pause
                wait(0.2); // switch debounce
                return MENU_BUTTON; // Toggle on menu screen
            } else if (inputs.ns_left) {
                current_action = (previous_action == GO_RIGHT) ? GO_RIGHT : GO_LEFT;
                return current_action;
            } else if (inputs.ns_right) {
                current_action = (previous_action == GO_LEFT) ? GO_LEFT : GO_RIGHT;
                return current_action;
            } else if (inputs.ns_up) {
                current_action = (previous_action == GO_DOWN) ? GO_DOWN : GO_UP;
                return current_action;
            } else if (inputs.ns_down) {
                current_action = (previous_action == GO_UP) ? GO_UP : GO_DOWN;
                return current_action;
            }
            // no change of input so we return previous_action
            return previous_action;    
        case GAME_OVER:
            if (inputs.ns_center) {
                return PRESS;
            } else {
                return NO_ACTION;
            }
    }   

    return NO_ACTION;
}

/**
 * This function add/update the angry viper to the HashTable when it moves
*/
void add_angry_viper(AngryViper * a) {
    // if angry viper's head location doesn't change, don't have to update
    if (angry_viper.head_x == angry_viper.head_px
            && angry_viper.head_y == angry_viper.head_py) {
        return;
    }
    
    // save the original angry viper's tail location
    int angry_tail_x = a->locations[a->length - 2].x;
    int angry_tail_y = a->locations[a->length - 2].y;
    // move each body part from current to next location
    for (int i = a->length - 2; i > 0; i--) {
        a->locations[i].x = a->locations[i - 1].x;
        a->locations[i].y = a->locations[i - 1].y;
    }
    // update initial angry viper location to the previous angry viper head's location
    a->locations[0].x = a->head_px;
    a->locations[0].y = a->head_py;
    // add the angry viper body after the head
        for (int i = 0; i < a->length - 2; i++) {
            add_angry_body(a->locations[i].x, a->locations[i].y);
        }
    // add angry viper's head
    add_angry_head(a->head_x, a->head_y);
    // add angry viper's tail
    add_angry_tail(a->locations[a->length - 2].x, a->locations[a->length - 2].y);
    // erase the original angry viper's tail
    map_erase(angry_tail_x, angry_tail_y);
    // update the previous angry viper head position to the current position
    a->head_px = a->head_x;
    a->head_py = a->head_y;
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
        if (viper_status != BOOST_DOWN) viper.score++;
        viper.length++;
        map_erase(viper.head_x, viper.head_y);
        objectType = GOT_LENGTH;
    } 

    switch(currItem->type) {
        case PLANT: // PLANT is boost up
            objectType = GOT_OBJ;
            viper_status = BOOST_UP;
            boost_timer = 0; // reset timer
            map_erase(viper.head_x, viper.head_y);
            break;
        case WATER: // WATER is boost down
            objectType = GOT_OBJ;
            viper_status = BOOST_DOWN;
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
        // add a viper body to the location after the head
        for (int i = 0; i < viper.length - 2; i++) {
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
 *   Update status of the Viper during game play
 */
int update_game_play(int action) {
    // DEBUG mode (Permanent boost-up)
    if (action == ACTION_BUTTON) {
        if (debug_status) debug_status = 0;
        else debug_status = DEBUG; // toggle debug mode
    } 
    
    if (new_game) {
        // only when input nav switch (moving the viper) then we enter game play
        if (action == NO_ACTION || action == MENU_BUTTON || action == ACTION_BUTTON) return FULL_DRAW;
        new_game = 0;
    }

    // TODO: Implement
    // 1. Check the game_mode (EASY, MEDIUM, HARD) and update viper speed status accordingly
    switch (game_mode) {
        case EASY:
            wait(0.5); // wait to change speed
            break;
        case MEDIUM:
            wait(0.2);
            break;
        case HARD:
            // normal speed in hard, don't have to wait
            break;
    }


    // 1.5. Update moving objects (ANGRY VIPER)
    // Angry VIPER will move in a 5x5 rectangle, move 1 step for each GAME_PLAY update
    // If Angry Viper is out of visible frame, don't have to update
    if (action != NO_ACTION && !out_of_frame) {
        int previous_distance = angry_viper.distance;
        int previous_state = angry_move_state;
        switch(angry_move_state) {
            case ANGRY_MOVE_RIGHT:
                angry_viper.distance--;
                angry_viper.step_x = 1;
                angry_viper.step_y = 0;
                // move to next state when angry viper already traversed its distance
                if (!angry_viper.distance) {
                    angry_move_state = ANGRY_MOVE_UP;
                    angry_viper.distance = 5; // reset distance
                }
                break;
            case ANGRY_MOVE_UP:
                angry_viper.distance--;
                angry_viper.step_x = 0;
                angry_viper.step_y = 1;
                // move to next state when angry viper already traversed its distance
                if (!angry_viper.distance) {
                    angry_move_state = ANGRY_MOVE_LEFT;
                    angry_viper.distance = 5; // reset distance
                }
                break;
            case ANGRY_MOVE_LEFT:
                angry_viper.distance--;
                angry_viper.step_x = -1;
                angry_viper.step_y = 0;
                // move to next state when angry viper already traversed its distance
                if (!angry_viper.distance) {
                    angry_move_state = ANGRY_MOVE_DOWN;
                    angry_viper.distance = 5; // reset distance
                }
                break;
            case ANGRY_MOVE_DOWN:
                angry_viper.distance--;
                angry_viper.step_x = 0;
                angry_viper.step_y = -1;
                // move to next state when angry viper already traversed its distance
                if (!angry_viper.distance) {
                    angry_move_state = ANGRY_MOVE_RIGHT;
                    angry_viper.distance = 5; // reset distance
                }
                break;
        }
    
        angry_viper.head_x = angry_viper.head_x + angry_viper.step_x;
        angry_viper.head_y = angry_viper.head_y + angry_viper.step_y;

        // check to see if the angry viper hits the hungry viper
        MapItem * checkBody = get_current(angry_viper.head_x, angry_viper.head_y); 
        if (checkBody->type == VIPER_BODY) {
            // if viper is invincible, angry viper just stops
            if (viper_status == BOOST_UP || debug_status) {
                angry_viper.head_x = angry_viper.head_px;
                angry_viper.head_y = angry_viper.head_py;
                angry_viper.distance = previous_distance;
                angry_move_state = previous_state;
            } else {
                return GAME_OVER; // player loses if angry viper bites hungry viper
            }
            
        }

        // update the angry viper to the map
        add_angry_viper(&angry_viper);

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
                if (viper.head_y == viper.locations[0].y) {
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
                if (viper.head_y == viper.locations[0].y) {
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
                if (viper.head_x == viper.locations[0].x) {
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
                if (viper.head_x == viper.locations[0].x) {
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
 * Update the user's input during menu screen
 */
int update_game_menu(int action) {
    switch (action) {
        case GO_UP:
            arrow_status = CONTINUE;
            break;
        case GO_DOWN:
            arrow_status = EXIT;
            break;
        case PRESS:
            if (arrow_status == CONTINUE) {
                game_status = GAME_PLAY;
            } else if (arrow_status == EXIT) {
                screen_status = 0; // clear gameplay screen to return to start screen
                game_status = GAME_START;
                arrow_status = START;
            }
            wait(0.2); // holding the switch can transit between screens too quickly
            break;
        default:
            // first time pausing, arrow places at CONTINUE
            if (arrow_status != EXIT) {
                arrow_status = CONTINUE;
            }
    }
    return DRAW_MENU;
}


/**
 * Update the user's input during the game start screen
 */
int update_game_start(int action) {
    switch (action) {
        case GO_UP:
            switch (arrow_status) {
                case START:
                    arrow_status = START;
                    break;
                case MODE:
                    arrow_status = START;
                    break;
                case EASY:
                    arrow_status = HARD;
                    break;
                case MEDIUM:
                    arrow_status = EASY;
                    break;
                case HARD:
                    arrow_status = MEDIUM;
                    break;
            }
            wait(0.1); // scrolling through the options can be too fast
            break;
        case GO_DOWN:
            switch (arrow_status) {
                case START:
                    arrow_status = MODE;
                    break;
                case MODE:
                    arrow_status = MODE;
                    break;
                case EASY:
                    arrow_status = MEDIUM;
                    break;
                case MEDIUM:
                    arrow_status = HARD;
                    break;
                case HARD:
                    arrow_status = EASY;
                    break;
            }
            wait(0.1); // scrolling through the options can be too fast
            break;
        case PRESS:
            switch (arrow_status) {
                case START:
                    game_status = GAME_PLAY;
                    break;
                case MODE:
                        switch (game_mode) {
                            case EASY:
                                arrow_status = EASY;
                                break;
                            case MEDIUM:
                                arrow_status = MEDIUM;
                                break;
                            case HARD:
                                arrow_status = HARD;
                                break;
                        }
                    break;
                case EASY:
                    game_mode = EASY; 
                    arrow_status = MODE;
                    break;
                case MEDIUM:
                    game_mode = MEDIUM;
                    arrow_status = MODE;
                    break;
                case HARD:
                    game_mode = HARD; 
                    arrow_status = MODE;
                    break;
            }
            wait(0.2); // holding the switch can transit between screens too quickly
            break;
    }
    return DRAW_START;
}

/**
 * Update the user's input during the game over screen (to restart the game)
 */
int update_game_over(int action)
{
    if (action == PRESS) {
        init_game(); // press to restart the game in game over screen
        wait(0.1); // holding the switch can transit between screens too quickly
        return DRAW_START;
    }
    return DRAW_OVER;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the viper position accordingly. If the user
 * is at menu screen, control input should navigate the menu.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the viper has not moved.
 */ 
int update_game(int action)
{
    switch (game_status) {
        case GAME_START:
            return update_game_start(action);
        case GAME_MENU:
            return update_game_menu(action);
        case GAME_PLAY:
            return update_game_play(action);
        case GAME_OVER:
            return update_game_over(action);
    }

    return NO_RESULT;
}

/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    uLCD.line(0, 9, 127, 9, status_color);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.line(0, 118, 127, 118, status_color);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(  0,  10, 127,  14, WHITE);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, WHITE);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, WHITE);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE);   // Right
}

/**
 *
*/
void draw_viper_status()
{
    char str[1024];
    switch (viper_status) {
        case NEUTRAL:
            snprintf(str,sizeof(str),"Status: Neutral");
            break;
        case BOOST_UP:
            snprintf(str,sizeof(str),"Status: Up");
            break;
        case BOOST_DOWN:
            snprintf(str,sizeof(str),"Status: Down");
            break;
    }
    uLCD.text_string(str,1,2,FONT_7X8,RED);
    
    switch(game_mode) {
        case EASY:
            snprintf(str,sizeof(str),"Mode: Easy");
            break;
        case MEDIUM:
            snprintf(str,sizeof(str),"Mode: Medium");
            break;
        case HARD:
            snprintf(str,sizeof(str),"Mode: Hard");
            break;
    }
    uLCD.text_string(str,1,4,FONT_7X8,RED);
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
    draw_border();
    
    // check viper's current status for status bar drawings
        switch (viper_status) {
            case NEUTRAL: // green means viper is neutral
                status_color = GREEN;
                break;
            case BOOST_UP: // red means viper received boost up
                status_color = RED;
                break;
            case BOOST_DOWN: // blue means viper received boost down
                status_color = BLUE;
                break;
        }

        char str[1024];
        if (game_status != GAME_START && game_status != GAME_OVER) {
            if (debug_status) {
                snprintf(str,sizeof(str),"Pos:%d,%d DEBUG           ",viper.head_x,viper.head_y);
            } else {
                snprintf(str,sizeof(str),"Pos:%d,%d Score:%d        ",viper.head_x,viper.head_y,viper.score);
            }
            uLCD.text_string(str,0,0,FONT_7X8,RED);
        }

    switch(draw_option) {

        // Draw game over screen
        case DRAW_OVER: 
            clear_screen(screen_status);
            screen_status = (!screen_status) ? 1: screen_status;
            snprintf(str,sizeof(str),"Game Over");
            uLCD.text_string(str,5,7,FONT_7X8,RED);
            snprintf(str,sizeof(str),"Score: %d", viper.score);
            uLCD.text_string(str,5,9,FONT_7X8,RED);
            snprintf(str,sizeof(str),"Press to restart");
            uLCD.text_string(str,1,11,FONT_7X8,RED);
            return;

        // Draw the pause screen
        case DRAW_MENU: 
            draw_option_continue(0, 0);
            draw_option_exit(0, 0);
            draw_viper_status();
            draw_option_arrow(arrow_status); // hard-coded (does not draw pixels with respect to u and v)
            return;

        // Draw the start (menu) screen
        case DRAW_START: 
            clear_screen(screen_status);
            screen_status = (!screen_status) ? 1: screen_status;
            draw_title(0 , 0);
            
            // if we started the game then return to menu screen, START changes to CONTINUE
            if (!new_game) draw_option_continue(0, 0);
            else draw_option_start(0, 0);
            
            draw_option_mode(0, 0);
            draw_option_arrow(arrow_status); // hard-coded (does not draw pixels with respect to u and v)
            clear_mode_selection(arrow_status); // after user finished choosing difficulty options
            return;

        // Full draw if we first time enter the game
        case FULL_DRAW:
            for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
                for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
                    // Here, we have a given (i,j)

                    // Compute the current map (x,y) of this tile
                    int x = i + viper.head_x;
                    int y = j + viper.head_y;

                    // Compute u,v coordinates for drawing
                    int u = (i+5)*11 + 3;
                    int v = (j+4)*11 + 15;

                    // Figure out what to draw
                    DrawFunc draw = NULL;
                    if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                        MapItem* curr_item = get_here(x, y);
                        if (curr_item) { // There's something here! Draw it
                            draw = curr_item->draw;
                        } else {
                            draw = draw_nothing;
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
            
            return;

        // default drawing (during game play)
        default:
            int check_out_of_frame = 0;
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
                        check_out_of_frame = (curr_item->type == ANGRY_BODY) ? 1 : check_out_of_frame; // don't have to draw the angry viper if not visible
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
            // check to see if the angry viper is out of visible frame
            out_of_frame = (check_out_of_frame) ? 0 : 1;
            
            // Draw status bars
            draw_upper_status();
            draw_lower_status();
    }
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // Set active map
    Map* map = set_active_map(0);

    // random: (rand() % (upper - lower + 1)) + lower;

    // random viper's food (30 foods) location
    for(int i = 0; i < 29; i++) {
        add_chest((rand() % (map_width() - 5 - 5 + 1)) + 5,
                (rand() % (map_height() - 5 - 5 + 1)) + 5);
    }
    
    
    // random viper's boost up items (5 items)
    // random viper's boost down items (5 items)
    for (int i = 0; i < 4; i++) {
        add_plant((rand() % (map_width() - 5 - 5 + 1)) + 5,
                (rand() % (map_height() - 5 - 5 + 1)) + 5);
        add_water((rand() % (map_width() - 5 - 5 + 1)) + 5,
                (rand() % (map_height() - 5 - 5 + 1)) + 5);
    }


    pc.printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    pc.printf("Walls done!\r\n");
    
    // add the initial HUNGRY VIPER
    add_viper_head(viper.head_x, viper.head_y);
    add_viper_body(viper.locations[0].x, viper.locations[0].y);
    add_viper_tail(viper.locations[1].x, viper.locations[1].y);

    // add the initial ANGRY VIPER
    add_angry_head(angry_viper.head_x, angry_viper.head_y);
    for (int i = 0; i < angry_viper.length - 2; i++) {
        add_angry_body(angry_viper.locations[i].x, angry_viper.locations[i].y);
    }
    add_angry_tail(angry_viper.locations[angry_viper.length - 2].x,
            angry_viper.locations[angry_viper.length - 2].y);

    pc.printf("Add extra chamber\r\n");
    //add_wall(30,  0,   VERTICAL, 10);
    //add_wall(30, 10, HORIZONTAL, 10);
    //add_wall(39,  0,   VERTICAL, 10);
    pc.printf("Added!\r\n");
}