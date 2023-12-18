#include <stdio.h>

#include "AI.h"
#include "game.h"

int main(){
    #ifdef GAME
    ONE_GAME_t classGame_t;
    gameInit(&classGame_t);
    draw_the_start_page(&classGame_t);
    input_game_mode(&classGame_t);
    draw_the_chessboard(&classGame_t);
    continue_the_game(&classGame_t);
    #endif
    return 0;
}