#include <stdio.h>

#include "AI.h"
#include "game.h"

void set_black(ONE_GAME_t *nowGame_t,uint8_t row,uint8_t col){
    nowGame_t->stateOfChessboard[MAT(row,col)]=BLACK;
}
/***************************复杂禁手********************************************/
// set_black(&classGame_t,5,5);
// set_black(&classGame_t,5,6);
// set_black(&classGame_t,6,7);
// set_black(&classGame_t,7,7);
// set_black(&classGame_t,8,6);
// set_black(&classGame_t,8,5);
// set_black(&classGame_t,7,4);
// set_black(&classGame_t,6,4);
/****************************判断胜负bug********************************************/
// set_black(&classGame_t,15,0);
// set_black(&classGame_t,14,0);
// set_black(&classGame_t,13,0);

int main(){
    #ifdef GAME
    ONE_GAME_t classGame_t;
    gameInit(&classGame_t);
    draw_the_start_page();
    input_game_mode(&classGame_t);
    draw_the_chessboard(&classGame_t);
    continue_the_game(&classGame_t);
    #endif
    return 0;
}