// ============================================
// The Viper class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "viper.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h> // for random function
#include <ctime>    // seed different random generator


void viper_init (Viper * v)
{
    // TODO: Implement
    //1. Set (random) starting coordinates for your viper head and previous
    // random: (rand() % (upper - lower + 1)) + lower;
    v->head_x = v->head_px = (rand() % (map_width() - 5 - 5 + 1)) + 5;
    v->head_y = v->head_py = (rand() % (map_height() - 5 - 5 + 1)) + 5;

    //2. Initialize all location for your maximum viper body (loop through the viper)

    //3. Set starting length of your viper

    //4. Set starting coordinates for the rest of viper body from starting length
    v->length = 3;

    for (int i = 0; i < VIPER_MAX_LENGTH; i++) {
        if (i < (v->length - 1)) {
            v->locations[i].x = v->head_x - (i + 1);
            v->locations[i].y = v->head_y;
            continue;
        }
        v->locations[i].x = -99; // out of map's vision
        v->locations[i].y = -99;
    }

    //5. Initialize viper status
    v->score = 0;
}
