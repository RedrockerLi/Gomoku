#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>

#include "AI.h"

#define RANGE_OF_CHESSBOARD 19


enum STATE_OF_GAME_E{
    PERSON_VS_PERSON,//游戏模式
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON,

    BLACK, //棋盘是黑子
    NONE,
    WHITE,

    BLACK_PLAYER, //游戏玩家
    WHITE_PLAYER,

    BLACK_WINE,//黑方胜利
    CONTINUE,//继续比赛
    WHITE_WINE,//白方胜利

    INDEX_OUT_OF_BOUNDS,
    OCCUPYED, //表示落子的地方已有其他棋子
    INPUT_UNUSED, //输入未使用
    INPUT_USED,//输入已使用
}; 

typedef struct 
{
    int32_t x;
    int32_t y;
    int8_t flag; //已输入未使用，已使用，输入非法
}INPUT_CHESS_PLACE_t;


#ifdef TRAIN
typedef struct 
{
    int8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD]; //棋盘
    uint8_t gameMode; //比赛模式
    int8_t playerFlag;//落子权归属
    INPUT_CHESS_PLACE_t blackInputChessPlace; //黑子落子接口
    INPUT_CHESS_PLACE_t whiteInputChessPlace; //白子落子接口
}ONE_GAME_t;
#endif

const uint8_t * get_state_of_chessboard_point(void);

#ifdef GAME
void gameInit(void);
void draw_the_start_page(void);
void input_game_mode(void);
void draw_the_chessboard(void);
void continue_the_game(void);
#endif

#endif