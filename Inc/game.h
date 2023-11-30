#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>

#include "AI.h"

#define RANGE_OF_CHESSBOARD 19

enum GAME_MODE{
    PERSON_VS_PERSON=0,
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON
};

enum STATE_OF_CHESSBOARD_E{
    BLACK=-1,
    NONE,
    WHITE
};

#ifdef TRAIN
typedef struct 
{
    uint8_t game_mode;                                                  //游戏模式
    int8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];  //棋盘落子状态
}ONE_GAME_t;
#endif



const int8_t * get_state_of_chessboard_point();

#ifdef GAME
void draw_the_start_page(void);
void input_game_mode(void);
void draw_the_chessboard(void);
#endif

#endif