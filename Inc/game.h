#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>

#include "chessboard.h"
#include "AI.h"

enum GAME_MODE{
    PERSON_VS_PERSON=0,
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON
};

#ifdef TRAIN
typedef struct 
{
    uint8_t game_mode;                                                  //游戏模式
    int8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];  //棋盘落子状态
}ONE_GAME_t;
#endif

#ifdef GAME
extern const int8_t *stateOfChessboard_p;
void game_init(void);
void draw_the_start_page(void);
void input_game_mode(void);
#endif
void draw_the_chessboard(const int8_t *stateOfChessboard);
#endif