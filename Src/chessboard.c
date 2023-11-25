#include <stdint.h>

#include "chessboard.h"

int8_t stateOfChessboard[RANGE_OF_CHESSBOARD][RANGE_OF_CHESSBOARD];

void chessboard_init(void){
    int8_t i,j;
    for(i=0;i<RANGE_OF_CHESSBOARD;i++){
        for(j=0;j<RANGE_OF_CHESSBOARD;j++){
            stateOfChessboard[i][j]=0;
        }
    }
}