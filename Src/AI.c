#include <stdint.h>

#include "AI.h"

#define MIN_OF_INT32 -2147483648
#define MAX_OF_INT32 2147483647

#define MAX_DEPTH_OF_ALPHA_BETA 4 //事实上是在博弈树的第二层(MIN)层往下搜索的层数

#define MAX(a,b) a>b?a:b
#define MIN(a,b) a<b?a:b

/**
 * @brief AI初始化
*/
void AI_init(ONE_AI_t *nowAI_t,const int32_t scoreChoose[]){
    for(uint8_t i=0;i<LENGTH_OF_STATES;i++){
        nowAI_t->scoreOfChessStates[i]=scoreChoose[i];
    }
    for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
        for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
            nowAI_t->scoreOfEveryPlace[MAT(row,col)]=0;
        }
    }
}


/**
 * @brief 给局势打分
*/
int32_t value_the_game(ONE_GAME_t * const nowGame_t,ONE_AI_t *nowAI_t){
    int32_t valueOfAll=0;
    for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
        for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
            uint8_t callTheGameAns=0;
            callTheGameAns=call_the_game(nowGame_t,row,col,1);
            if(callTheGameAns==BLACK_WINE){
                if(nowGame_t->playerFlag=BLACK_PLAYER){
                    return MAX_OF_INT32;
                }else{
                    return MIN_OF_INT32;
                }
            }else if(callTheGameAns==WHITE_WINE){
                if(nowGame_t->playerFlag=BLACK_PLAYER){
                    return MIN_OF_INT32;
                }else{
                    return MAX_OF_INT32;
                }
            }
        }
    }
    for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
        for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
            if(nowGame_t->stateOfChessboard[MAT(row,col)]==BLACK){
                for(uint8_t directionChoice=0;directionChoice<4;directionChoice++){
                    valueOfAll+=nowAI_t->scoreOfChessStates[judge_state_of_chess(nowGame_t,row,col,BLACK,directionChoice,2)];
                }
            }else if(nowGame_t->stateOfChessboard[MAT(row,col)]==WHITE){
                for(uint8_t directionChoice=0;directionChoice<4;directionChoice++){
                    valueOfAll-=nowAI_t->scoreOfChessStates[judge_state_of_chess(nowGame_t,row,col,WHITE,directionChoice,2)];
                }
            }
        }
    }
    if(nowGame_t->playerFlag=BLACK_PLAYER){
        return valueOfAll;
    }else{
        return -valueOfAll;
    }
}

/**
 * @brief alpha-beta剪枝
*/
int32_t alpha_beta_pruning(ONE_GAME_t * const nowGame_t,uint8_t depth,int32_t alpha,int32_t beta,uint8_t addRow,uint8_t addCol){
    if(depth%2==0){

    }
    if(depth==0){
        // return value_the_game(); //打分
    }
    if(depth%2==0){//算MIN层节点分数//下白棋
        int32_t scoreOfChild=MAX_OF_INT32;
        uint8_t row,col;
        for(row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(col=0;col<RANGE_OF_CHESSBOARD;col++){
                if(nowGame_t->stateOfChessboard[1])
                scoreOfChild=MIN(scoreOfChild,alpha_beta_pruning(nowGame_t,depth-1,alpha,beta,row,col));
                beta=MIN(beta,scoreOfChild);
                if(beta<=alpha){
                    break;
                }
            }
            return scoreOfChild;
        }
    }else{
        int32_t scoreOfChild=MIN_OF_INT32;
        uint8_t row,col;
        for(row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(col=0;col<RANGE_OF_CHESSBOARD;col++){
                scoreOfChild=MAX(scoreOfChild,alpha_beta_pruning(nowGame_t,depth-1,alpha,beta,row,col));
                beta=MAX(beta,scoreOfChild);
                if(beta<=alpha){
                    break;
                }
            }
            return scoreOfChild;
        }
    }
}