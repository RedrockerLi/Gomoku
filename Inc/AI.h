#ifndef _AI_H
#define _AI_H

#include "game.h"

typedef struct 
{
    int32_t scoreOfChessStates[LENGTH_OF_STATES];
    int32_t scoreOfEveryPlace[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];
}ONE_AI_t;

#endif