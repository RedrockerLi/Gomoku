#include <stdint.h>

#include "AI.h"

#define MIN_OF_INT32 -2147483648
#define MAX_OF_INT32 2147483647

#define MAX_DEPTH_OF_ALPHA_BETA 4 //事实上是在博弈树的第二层(MIN)层往下搜索的层数

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

/**
 * @brief 给局势打分
*/
int32_t value_the_game(){
    return 0;
}

/**
 * @brief alpha-beta剪枝
*/
int32_t alpha_beta_pruning(ONE_GAME_t * const nowGame_t,uint8_t depth,int32_t alpha,int32_t beta){
    if(depth==4){
        return value_the_game(); //打分
    }
    if(depth%2==0){//算MIN层节点分数
        int32_t scoreOfChild=MAX_OF_INT32;
        uint8_t row,col;
        for(row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(col=0;col<RANGE_OF_CHESSBOARD&&nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE&&(nowGame_t->playerFlag==WHITE_PLAYER||judge_forbidden_hand(nowGame_t,row,col,1)!=FORBIDDEN_HAND);col++){
                scoreOfChild=min(scoreOfChild,alpha_beta_pruning(nowGame_t,depth-1,alpha,beta));
            }
        }
    }else{

    }
}