#include <stdio.h>

#include "AI.h"
#include "game.h"
#include "main.h"

void set_black(ONE_GAME_t * const nowGame_t,uint8_t row,uint8_t col){
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
/****************************测试AI下棋时间********************************************/

int main(){
    #ifdef GAME
    // ONE_GAME_t classGame_t;
    // ONE_AI_t classAI_t;
    // game_init(&classGame_t);
    // draw_the_start_page();
    // input_game_mode(&classGame_t);
    // draw_the_chessboard(&classGame_t);
    // continue_the_game(&classGame_t,&classAI_t);

    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    const int32_t scoreChoose[LENGTH_OF_STATES]={5,10,20,15,20,30,60,100,50,1000,500,10000,20000,30000,60000,80000};
    game_init(&classGame_t);
    AI_init(&classAI_t,scoreChoose);
    set_black(&classGame_t,7,7);
    classGame_t.gameMode=PERSON_VS_COMPUTER;
    classGame_t.playerFlag=WHITE_PLAYER;
    draw_the_chessboard(&classGame_t);
    continue_the_game(&classGame_t,&classAI_t);
    #endif
    return 0;
}