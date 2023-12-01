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

typedef struct 
{
    uint8_t x;
    uint8_t y;
    int8_t flag; //1代表已输入未使用，0代表已使用，-1代表输入非法
}INPUT_CHESS_PLACE_t;


#ifdef TRAIN
typedef struct 
{
    uint8_t game_mode;                                                  //游戏模式
    int8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];  //棋盘落子状态
    INPUT_CHESS_PLACE_t blackInputChessPlace;
    INPUT_CHESS_PLACE_t whiteInputChessPlace;
}ONE_GAME_t;
#endif

#ifdef GAME
void draw_the_start_page(void);
void input_game_mode(void);
void draw_the_chessboard(void);
void input_chess_place(void);
void place_the_chess(void);
void continue_a_round(void);
const int8_t * get_state_of_chessboard_point(void);
#endif

#endif