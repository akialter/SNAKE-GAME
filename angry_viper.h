// ============================================
// The header file for Viper class file.
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#ifndef ANGRY_VIPER_H
#define ANGRY_VIPER_H

#define ANGRY_VIPER_LENGTH 5
#define ANGRY_MOVE_UP 1 // each moving state of the angry viper
#define ANGRY_MOVE_DOWN 2
#define ANGRY_MOVE_LEFT 3
#define ANGRY_MOVE_RIGHT 4
#define ANGRY_STOP 5



// Structure of coordinates in the map
typedef struct{
    int x;
    int y;
} Location;

// Viper Structure
typedef struct {
    int head_x, head_y, head_px, head_py; // Location of the head of the angry viper
    int length; // length of the angry viper
    Location locations[ANGRY_VIPER_LENGTH]; // Angry viper body locations
    int step_x, step_y; // the step and direction of the angry viper (1, 0 or -1 for both values)
    int distance; // the distance that the angry viper moves
} AngryViper;

// Initialize a viper structure
// You want to assign initial values to each of the variables defined above
// in the viper structure.
void angry_viper_init(AngryViper* angry_viper);

#endif ANGRY_VIPER_H  // VIPER_H