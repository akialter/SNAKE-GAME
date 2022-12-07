// ============================================
// The Graphics class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "graphics.h"
#include "sprites.h"
#include "globals.h"

char str[1024];
char str_empty[1024];


void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00;   // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E;   // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY;      // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_wall(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, brick_wall);
}

void draw_plant(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, chicken);
}

void draw_water(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, poison);;
}

void draw_chest(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, apple);
}

void draw_viper_body(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, body);
}

void draw_viper_head(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, head);
}

void draw_viper_tail(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, tail);
}

void draw_angry_body(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, angry_body);
}

void draw_angry_head(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, angry_head);
}

void draw_angry_tail(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, angry_tail);
}

void draw_option_continue(int u, int v) 
{
    snprintf(str,sizeof(str),"Continue");
    uLCD.text_string(str,5,9,FONT_7X8,RED);
}

void draw_option_exit(int u, int v)
{
    snprintf(str,sizeof(str),"Exit");
    uLCD.text_string(str,5,11,FONT_7X8,RED);
}

void draw_option_start(int u, int v) 
{
    snprintf(str,sizeof(str),"Start");
    uLCD.text_string(str,5,9,FONT_7X8,RED);
}

void draw_option_mode(int u, int v)
{
    snprintf(str,sizeof(str),"Mode");
    uLCD.text_string(str,5,11,FONT_7X8,RED);
}

void draw_option_arrow(int arrow_status) // add u and v if draw as pixels
{
    // arrow's location is hard-coded (not wirh respect to u and v)
    snprintf(str,sizeof(str),"->");
    snprintf(str_empty,sizeof(str),"  ");
    
    switch (arrow_status) {
        case CONTINUE: 
            uLCD.text_string(str,3,9,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,11,FONT_7X8,GREEN);
            break;
        case EXIT:
            uLCD.text_string(str,3,11,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,9,FONT_7X8,GREEN);
            break;
        case START:
            uLCD.text_string(str,3,9,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,11,FONT_7X8,GREEN);
            break;
        case MODE:
            uLCD.text_string(str,3,11,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,9,FONT_7X8,GREEN);
            break;
        case EASY:
            uLCD.text_string(str,9,11,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,11,FONT_7X8,GREEN);
            snprintf(str,sizeof(str),"Easy  ");
            uLCD.text_string(str,11,11,FONT_7X8,RED);
            break;
        case MEDIUM:
            uLCD.text_string(str,9,11,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,11,FONT_7X8,GREEN);
            snprintf(str,sizeof(str),"Medium");
            uLCD.text_string(str,11,11,FONT_7X8,RED);
            break;
        case HARD:
            uLCD.text_string(str,9,11,FONT_7X8,GREEN);
            uLCD.text_string(str_empty,3,11,FONT_7X8,GREEN);
            snprintf(str,sizeof(str),"Hard  ");
            uLCD.text_string(str,11,11,FONT_7X8,RED);
            break;
    }
}

void draw_title(int u, int v)
{
    snprintf(str,sizeof(str),"HUNGRY VIPER");
    uLCD.text_string(str,2,5,FONT_7X8,RED);
}

void clear_screen(int screen_status)
{
    // clear the screen if screen_status is false
    if (!screen_status) uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
}

void clear_mode_selection (int arrow_status) {
    if (arrow_status != EASY && arrow_status != MEDIUM && arrow_status != HARD) {
        snprintf(str,sizeof(str),"Mode        ");
        uLCD.text_string(str,5,11,FONT_7X8,RED);
    }
}