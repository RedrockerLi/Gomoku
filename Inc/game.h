#ifndef _GAME_H
#define _GAME_H

#include <stdint.h>

#include "main.h"
#include "threadPool.h"

#define MAT(row,col) row*RANGE_OF_CHESSBOARD+col

#define RANGE_OF_CHESSBOARD 15 //因为棋形判断时采用了位运算，所以不能随意更改此数值

enum STATE_OF_GAME_E{
    PERSON_VS_PERSON,//游戏模式
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON,
    COMPUTER_VS_COMPUTER,
    EXIT,

    BLACK_WINE,//黑方胜利
    CONTINUE,//继续比赛
    WHITE_WINE,//白方胜利

    BLACK, //棋盘是黑子
    LAST_BLACK, //刚刚落下的黑子
    TEMP_BLACK, //判断复杂禁手时落下的临时黑子
    AI_BLACK, //AI落下的临时黑子
    NONE,
    WHITE,
    AI_WHITE,
    LAST_WHITE,

    BLACK_PLAYER, //游戏玩家
    WHITE_PLAYER,

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
    THREE_JUMP2,
    LIVE_THREE_JUMP1,
    FIGHT_THREE_JUMP1,
    FAKE_THREE,
    LIVE_THREE,
    JUMP_FOUR,
    FIGHT_FOUR,
    LIVE_FOUR,
    FIVE,
    LENGTH_OF_STATES,
    FORBIDDEN_HAND,
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

#ifndef _ONE_AI_t
#define _ONE_AI_t
typedef struct 
{
    int32_t scoreOfChessStates[LENGTH_OF_STATES];
    int32_t scoreOfEveryPlace[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];
}ONE_AI_t;
#endif
void game_init(ONE_GAME_t * const nowGame_t);
void draw_the_start_page(void);
void input_game_mode(ONE_GAME_t * const nowGame_t);
void draw_the_chessboard(ONE_GAME_t * const nowGame_t);
void place_the_chess(ONE_GAME_t * const nowGame_t);
uint8_t judge_forbidden_hand(ONE_GAME_t * const nowGame_t,uint8_t row,uint8_t col,uint8_t mode);
uint8_t call_the_game(ONE_GAME_t * const nowGame_t,uint8_t lastRow,uint8_t lastCol,uint8_t mode);
uint8_t judge_state_of_chess(ONE_GAME_t * const nowGame_t,const uint8_t row, const uint8_t col,const uint8_t stdChess,const uint8_t directionChoice,uint8_t mode);
#ifndef THREAD_POOL_FOR_AI
void continue_the_game(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t);
#else
void continue_the_game(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,threadpool * thpoolForAI);
#endif
#endif