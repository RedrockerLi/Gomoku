#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>

#include "chessboard.h"

enum GAME_MODE{
    PERSON_VS_PERSON=0,
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON
};


void game_init(void);
void draw_the_start_page(void);
void input_game_mode(void);
void draw_the_chessboard(void);
#endif