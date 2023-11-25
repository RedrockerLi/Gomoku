#include <stdio.h>

#include "AI.h"
#include "chessboard.h"
#include "game.h"

int main(){
    chessboard_init();
    draw_the_start_page();
    get_game_mode();
    return 0;
}