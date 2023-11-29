#include <stdio.h>

#include "AI.h"
#include "chessboard.h"
#include "game.h"

int main(){
    #ifdef GAME
    game_init();
    draw_the_start_page();
    input_game_mode();
    draw_the_chessboard(stateOfChessboard_p);
    #endif
    return 0;
}