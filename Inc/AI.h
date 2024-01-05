#ifndef _AI_H
#define _AI_H

#include "game.h"

#ifndef _ONE_AI_t_H
#define _ONE_AI_t_H
typedef struct 
{
    int32_t scoreOfChessStates[LENGTH_OF_STATES];
    int32_t scoreOfEveryPlace[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];
}ONE_AI_t;

#endif

void calc_next_input(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t);
void AI_init(ONE_AI_t *nowAI_t,const int32_t scoreChoose[]);

#endif