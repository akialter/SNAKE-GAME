// ============================================
// The header file for general project settings
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#ifndef GLOBAL_H
#define GLOBAL_H

// Include all the hardware libraries
#include "mbed.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"
#include "Nav_Switch.h"

// Declare the hardware interface objects
// The (extern) declarations of the globals.
// "extern" means "this is instantiated somewhere, but here's 
// what the name means.
extern uLCD_4DGL uLCD;      // LCD Screen
extern Serial pc;           // USB Console output
extern DigitalIn button1;   // Pushbuttons
extern DigitalIn button2;   // ...
extern DigitalIn button3;   // ...
extern Nav_Switch navs;     // Nav Switch
extern AnalogOut DACout;    // Speaker
extern PwmOut speaker;      // ...
extern wave_player waver;   // Wave player

// Global settings
#define F_DEBUG   1                     // Debug flag

#define BACKGROUND_COLOR 0x000000       // Black

#define PI 3.1415926

#define NO_RESULT 0
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define GAME_OVER 7
#define FULL_DRAW 8
#define MOVED 9
#define GOT_LENGTH 10
#define GOT_OBJ 11
#define GAME_MENU 12
#define GAME_PLAY 13
#define DRAW_MENU 14
#define PRESS 15
#define CONTINUE 16
#define EXIT 17
#define START 18
#define MODE 19
#define EASY 20
#define MEDIUM 21
#define HARD 22
#define GAME_START 23
#define DRAW_START 24
#define DRAW_OVER 25

// === [define the macro of error handle function] ===
// when the condition (c) is not true, assert the program and show error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        pc.printf("\nERROR:%d\n",e); \
        while(1); \
    } \
} while (0)

// === [error code] ===
#define ERROR_NONE 0 // All good in the hood
#define ERROR_MEH -1 // This is how errors are done

#endif //GLOBAL_H