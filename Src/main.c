#include <stdio.h>

#include "AI.h"
#include "game.h"

int main(){
    #ifdef GAME
    draw_the_start_page();
    input_game_mode();
    draw_the_chessboard();
    #endif
    return 0;
}