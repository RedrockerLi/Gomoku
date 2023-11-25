#ifndef _CHESSBOARD_H
#define _CHESSBOARD_H

#define RANGE_OF_CHESSBOARD 19

enum STATE_OF_CHESSBOARD_E{
    BLACK=-1,
    NONE,
    WHITE
};

extern int8_t stateOfChessboard[RANGE_OF_CHESSBOARD][RANGE_OF_CHESSBOARD];

void chessboard_init(void);
#endif