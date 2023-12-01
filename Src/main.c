#include <stdio.h>

#include "AI.h"
#include "game.h"

int main(){
    #ifdef GAME
    gameInit();
    draw_the_start_page();
    input_game_mode();
    draw_the_chessboard();
    continue_the_game();
    #endif
    return 0;
}