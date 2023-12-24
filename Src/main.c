#include <stdio.h>

#include "AI.h"
#include "game.h"

void set_black(ONE_GAME_t *nowGame_t,uint8_t row,uint8_t col){
    nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK;
}

int main(){
    #ifdef GAME
    ONE_GAME_t classGame_t;
    gameInit(&classGame_t);
    draw_the_start_page();
    input_game_mode(&classGame_t);
    set_black(&classGame_t,5,5);
    set_black(&classGame_t,5,6);
    set_black(&classGame_t,6,7);
    set_black(&classGame_t,7,7);
    set_black(&classGame_t,8,6);
    set_black(&classGame_t,8,5);
    set_black(&classGame_t,7,4);
    set_black(&classGame_t,6,4);
    draw_the_chessboard(&classGame_t);
    continue_the_game(&classGame_t);
    #endif
    return 0;
}