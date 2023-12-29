#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>

#include "main.h"

#define RANGE_OF_CHESSBOARD 15 //因为棋形判断时采用了位运算，所以不能随意更改此数值


enum STATE_OF_GAME_E{
    PERSON_VS_PERSON,//游戏模式
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON,
    EXIT,

    BLACK, //棋盘是黑子
    LAST_BLACK, //刚刚落下的黑子
    TEMP_BLACK, //判断复杂禁手时落下的临时黑子
    NONE,
    WHITE,
    LAST_WHITE,

    BLACK_PLAYER, //游戏玩家
    WHITE_PLAYER,

    BLACK_WINE,//黑方胜利
    CONTINUE,//继续比赛
    WHITE_WINE,//白方胜利

    INDEX_OUT_OF_BOUNDS, //输入在棋盘外
    OCCUPYED, //表示落子的地方已有其他棋子
    INVALID_FORM,//错误输入形式
    INPUT_UNUSED, //输入未使用
    INPUT_USED,//输入已使用
}; 

enum STATE_OF_CHASS_E{
    SINGLE_0B,//单子0空
    SINGLE_1B,
    SINGLE_2B,
    TWO_JUMP3,
    TWO_JUMP2,
    TWO_JUMP1,
    TWO_0B,
    TWO_1B,
    TWO_2B,
    FAKE_THREE,
    LIVE_THREE,
    FIGHT_FOUR,
    LIVE_FOUR,
    FORBIDDEN_HAND,
    LENGTH_OF_STATES,
};

typedef struct 
{
    int8_t row;
    int8_t col;
    uint8_t flag; //已输入未使用，已使用，输入非法
}INPUT_CHESS_PLACE_t;

typedef struct 
{
    int8_t row;
    int8_t col;
    uint8_t flag; //已输入未使用，已使用，输入非法
}LAST_CHESS_PLACE_t;

typedef struct 
{
    int8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD]; //棋盘
    uint8_t gameMode; //比赛模式
    int8_t playerFlag;//落子权归属
    uint8_t gameWinner;//比赛结果
    INPUT_CHESS_PLACE_t blackInputChessPlace; //黑子落子接口
    INPUT_CHESS_PLACE_t whiteInputChessPlace; //白子落子接口
    LAST_CHESS_PLACE_t lastBlackInputChessPlace; //上一个落子
    LAST_CHESS_PLACE_t lastWhiteInputChessPlace; 
}ONE_GAME_t;

const uint8_t * get_state_of_chessboard_point(void);

void gameInit(ONE_GAME_t * const nowGame_t);
void draw_the_start_page(void);
void input_game_mode(ONE_GAME_t * const nowGame_t);
void draw_the_chessboard(ONE_GAME_t * const nowGame_t);
uint8_t judge_forbidden_hand(ONE_GAME_t * const nowGame_t,uint8_t row,uint8_t col,uint8_t mode);
void continue_the_game(ONE_GAME_t * const nowGame_t);

#endif