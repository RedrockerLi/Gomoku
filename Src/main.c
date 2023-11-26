#include <stdio.h>

#include "AI.h"
#include "chessboard.h"
#include "game.h"

int main(){
    chessboard_init();
    draw_the_start_page();
    input_game_mode();
    draw_the_chessboard();
    return 0;
}