#include <stdint.h>
#include <stdio.h>

#include "chessboard.h"

static int8_t stateOfChessboard[RANGE_OF_CHESSBOARD][RANGE_OF_CHESSBOARD];

/**
 * @brief 初始化棋盘状态
*/
void chessboard_init(void){
    int8_t i,j;
    for(i=0;i<RANGE_OF_CHESSBOARD;i++){
        for(j=0;j<RANGE_OF_CHESSBOARD;j++){
            stateOfChessboard[i][j]=0;
        }
    }
}


void draw_the_chessboard(void){
    int8_t i,j;
    for(j=0;j<RANGE_OF_CHESSBOARD;j++){
        for(i=0;i<RANGE_OF_CHESSBOARD;i++){
            if(i<RANGE_OF_CHESSBOARD-1){
                if(stateOfChessboard[i][j]==BLACK){
                    printf("*--");
                }else if(stateOfChessboard[i][j]==NONE){
                    printf(" --");
                }else if(stateOfChessboard[i][j]==WHITE){
                    printf("#--");
                }
            }else{
                if(stateOfChessboard[i][j]==BLACK){
                    printf("*\n");
                }else if(stateOfChessboard[i][j]==NONE){
                    printf(" \n");
                }else if(stateOfChessboard[i][j]==WHITE){
                    printf("#\n");
                }
            }
        }
        if(j<RANGE_OF_CHESSBOARD-1){
            for(i=0;i<RANGE_OF_CHESSBOARD;i++){
                if(i<RANGE_OF_CHESSBOARD-1){
                    printf("|  ");
                }else{
                    printf("|\n");
                }
            }
        }
    }
}