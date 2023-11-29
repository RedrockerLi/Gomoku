#include <stdint.h>
#include <stdio.h>

#include "chessboard.h"
#include "AI.h"

#ifdef GAME
static int8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];

/**
 * @brief 提供只读的接口
*/
const int8_t * get_state_of_chessboard_point(){
    return &stateOfChessboard;
}
#endif

/**
 * @brief 初始化棋盘状态
*/
void chessboard_init(int8_t *stateOfChessboard){
    int8_t i,j;
    for(i=0;i<RANGE_OF_CHESSBOARD;i++){
        for(j=0;j<RANGE_OF_CHESSBOARD;j++){
            stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]=0;
        }
    }
}


