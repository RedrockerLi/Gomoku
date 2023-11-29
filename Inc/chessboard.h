#ifndef _CHESSBOARD_H
#define _CHESSBOARD_H
#include <stdint.h>

#define RANGE_OF_CHESSBOARD 19

enum STATE_OF_CHESSBOARD_E{
    BLACK=-1,
    NONE,
    WHITE
};

void chessboard_init(int8_t *stateOfChessboard);

#ifndef GAME
const int8_t * get_state_of_chessboard_point();
#endif

#endif