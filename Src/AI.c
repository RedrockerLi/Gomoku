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
int32_t value_the_game(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t){
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
int32_t alpha_beta_pruning(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,int8_t depth,int32_t alpha,int32_t beta,uint8_t addRow,uint8_t addCol){
    int32_t scoreOfChild=0;
    if((depth+1)%2==0){//下MIN节点的棋[对方的棋]
        if(nowGame_t->playerFlag==BLACK){
            if(nowGame_t->stateOfChessboard[MAT(addRow,addCol)]==NONE){
                nowGame_t->stateOfChessboard[MAT(addRow,addCol)]=AI_WHITE;
            }
        }else{
            if(nowGame_t->stateOfChessboard[MAT(addRow,addCol)]==NONE){
                nowGame_t->stateOfChessboard[MAT(addRow,addCol)]=AI_BLACK;
            }
        }
    }else{
        if(nowGame_t->playerFlag==BLACK){
            if(nowGame_t->stateOfChessboard[MAT(addRow,addCol)]==NONE){
                nowGame_t->stateOfChessboard[MAT(addRow,addCol)]=AI_BLACK;
            }
        }else{
            if(nowGame_t->stateOfChessboard[MAT(addRow,addCol)]==NONE){
                nowGame_t->stateOfChessboard[MAT(addRow,addCol)]=AI_WHITE;
            }
        }
    }
    uint8_t ansOfCallTheGame=call_the_game(nowGame_t,addRow,addCol,1);
    if(ansOfCallTheGame!=CONTINUE){
        if((depth+1)%2==0){//[对方的棋]
            if(nowGame_t->playerFlag==BLACK){//检查
                if(ansOfCallTheGame==WHITE_WINE){
                    scoreOfChild=MIN_OF_INT32;
                }
            }else{
                if(ansOfCallTheGame==BLACK_WINE){
                    scoreOfChild=MIN_OF_INT32;
                }
            }
        }else{
            if(nowGame_t->playerFlag==BLACK){
                if(ansOfCallTheGame==BLACK_WINE){
                    scoreOfChild=MAX_OF_INT32;
                }
            }else{
                if(ansOfCallTheGame==WHITE_WINE){
                    scoreOfChild=MAX_OF_INT32;
                }
            }
        }
    }else{
        if(depth==-1){
            scoreOfChild=value_the_game(nowGame_t,nowAI_t); 
        }else{
            uint8_t flag=1;
            if(depth%2==0){//算MIN层节点分数
                scoreOfChild=MAX_OF_INT32;
                uint8_t row,col;
                for(row=0,flag=1;flag=1&&row<RANGE_OF_CHESSBOARD;row++){
                    for(col=0;flag==1&&col<RANGE_OF_CHESSBOARD;col++){
                        if(nowGame_t->stateOfChessboard[MAT(row,col)]==NONE){
                            if(nowGame_t->playerFlag==WHITE_PLAYER){ //检查
                                if(judge_forbidden_hand(nowGame_t,row,col,1)==FORBIDDEN_HAND){
                                    continue;
                                }
                            }
                            scoreOfChild=MIN(scoreOfChild,alpha_beta_pruning(nowGame_t,nowAI_t,depth-1,alpha,beta,row,col));
                            beta=MIN(beta,scoreOfChild);
                            if(beta<=alpha){
                                flag=0; //跳出两层循环
                            }
                        }
                    }
                }
            }else{
                scoreOfChild=MIN_OF_INT32;
                uint8_t row,col;
                for(row=0,flag=1;flag=1&&row<RANGE_OF_CHESSBOARD;row++){
                    for(col=0;flag=1&&col<RANGE_OF_CHESSBOARD;col++){
                        if(nowGame_t->stateOfChessboard[MAT(row,col)]==NONE){
                            if(nowGame_t->playerFlag==BLACK_PLAYER){
                                if(judge_forbidden_hand(nowGame_t,row,col,1)==FORBIDDEN_HAND){
                                    continue;
                                }
                            }
                            scoreOfChild=MAX(scoreOfChild,alpha_beta_pruning(nowGame_t,nowAI_t,depth-1,alpha,beta,row,col));
                            beta=MAX(beta,scoreOfChild);
                            if(beta<=alpha){
                                flag=0;
                            }
                        }
                    }
                }
            }
        }
    }
    if(nowGame_t->stateOfChessboard[MAT(addRow,addCol)]==AI_BLACK||nowGame_t->stateOfChessboard[MAT(addRow,addCol)]==AI_WHITE){
        nowGame_t->stateOfChessboard[MAT(addRow,addCol)]=NONE;
    }
    return scoreOfChild;
}