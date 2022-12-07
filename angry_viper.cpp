// ============================================
// The Viper class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "angry_viper.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h> // for random function
#include <ctime>    // seed different random generator


void angry_viper_init (AngryViper * a)
{
    // TODO: Implement
    //1. Set (random) starting coordinates for angry viper head and previous
    // random: (rand() % (upper - lower + 1)) + lower;
    a->head_x = a->head_px = (rand() % (map_width() - 5 - 5 + 1)) + 5;
    a->head_y = a->head_py = (rand() % (map_height() - 5 - 5 + 1)) + 5;

    a->step_x = 1; 
    a->step_y = 0;
    a->distance = 5; // angry viper moves in 5x5 rectangle

    //2. Initialize all location for the angry viper body
    //3. Set starting coordinates for the rest of angry viper body from starting length
    a->length = ANGRY_VIPER_LENGTH;

    for (int i = 0; i < a->length; i++) {
        a->locations[i].x = a->head_x;
        a->locations[i].y = a->head_y - (i + 1);
    }
}