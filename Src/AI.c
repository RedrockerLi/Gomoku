#include <stdint.h>

#include "AI.h"
#include "game.h"
#include "main.h"
#include "threadPool.h"

#define MIN_OF_INT32 -2147483648
#define MAX_OF_INT32 2147483647

#define MAX_DEPTH_OF_ALPHA_BETA 4 //事实上是在博弈树的第二层(MIN)层往下搜索的层数
#define NUM_OF_CHILDREN 3 //每一层搜索的子节点数量

#define MAX(a,b) a>b?a:b
#define MIN(a,b) a<b?a:b

typedef struct{
    ONE_GAME_t oneGame_t;
    ONE_AI_t oneAI_t;
    int32_t *ans;
}ONE_GAME_AND_AI_t;

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
                if(nowGame_t->playerFlag==BLACK_PLAYER){
                    return MAX_OF_INT32;
                }else{
                    return MIN_OF_INT32;
                }
            }else if(callTheGameAns==WHITE_WINE){
                if(nowGame_t->playerFlag==BLACK_PLAYER){
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
    if(nowGame_t->playerFlag==BLACK_PLAYER){
        return valueOfAll;
    }else{
        return -valueOfAll;
    }
}

#ifdef THREAD_POOL
/**
 * @brief 给局势打分(合并参数)
*/
void value_the_game_one_arg(ONE_GAME_AND_AI_t *oneGameAndAI){
    *(oneGameAndAI->ans)=value_the_game(&oneGameAndAI->oneGame_t,&oneGameAndAI->oneAI_t);
}

/**
 * @brief 多线程打分中转
*/
void value_the_game_with_thread_pool(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,threadpool *thpoolForAI,int32_t *ans){
    ONE_GAME_AND_AI_t oneGameAndAI;
    oneGameAndAI.ans=ans;
    oneGameAndAI.oneAI_t=*nowAI_t;
    oneGameAndAI.oneGame_t=*nowGame_t;
    #ifdef LOG
    if(oneGameAndAI.ans!=ans){
        output_log("runningLog","Error:value_the_game_with_thread_pool ans");
    }
    if(&oneGameAndAI.oneAI_t==nowAI_t){
        output_log("runningLog","Error:value_the_game_with_thread_pool nowAI_t");
    }
    if(&oneGameAndAI.oneGame_t==nowGame_t){
        output_log("runningLog","Error:value_the_game_with_thread_pool nowGame_t");
    }
    #endif
    while (thpool_add_work(*thpoolForAI,(void *)value_the_game_one_arg,(void*)&oneGameAndAI)!=0);
}
#endif

/**
 * @brief 预先找到比较好的点
*/
#ifndef THREAD_POOL
void fine_great_children(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,TO_CHECK_t greatChildrenGroup[],uint8_t playerFlag,int8_t depth){
#else
void fine_great_children(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,TO_CHECK_t greatChildrenGroup[],uint8_t playerFlag,int8_t depth,threadpool * thpoolForAI){
#endif
    uint8_t i,flag;
    int32_t scoreOfChild[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD];
    for(i=0;i<NUM_OF_CHILDREN;i++){
        if(depth%2==0){
            greatChildrenGroup[i].score=MAX_OF_INT32;
            greatChildrenGroup[i].row=greatChildrenGroup[i].col=0;
        }else{
            greatChildrenGroup[i].score=MIN_OF_INT32;
            greatChildrenGroup[i].row=greatChildrenGroup[i].col=0;
        }
    }
    if(depth%2==0){
        i=0;
        flag=1;
        for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
                if(nowGame_t->stateOfChessboard[MAT(row,col)]==NONE){
                    if(playerFlag==WHITE_PLAYER){
                        if(judge_forbidden_hand(nowGame_t,row,col,1)==FORBIDDEN_HAND){
                            scoreOfChild[MAT(row,col)]=MAX_OF_INT32;
                            continue;
                        }else{
                            nowGame_t->stateOfChessboard[MAT(row,col)]=AI_BLACK;
                            #ifndef THREAD_POOL
                            scoreOfChild[MAT(row,col)]=value_the_game(nowGame_t,nowAI_t);
                            #else
                            value_the_game_with_thread_pool(nowGame_t,nowAI_t,thpoolForAI,&scoreOfChild[MAT(row,col)]);
                            #endif
                        }
                    }else{
                        nowGame_t->stateOfChessboard[MAT(row,col)]=AI_WHITE;
                        #ifndef THREAD_POOL
                        scoreOfChild[MAT(row,col)]=value_the_game(nowGame_t,nowAI_t);
                        #else
                        value_the_game_with_thread_pool(nowGame_t,nowAI_t,thpoolForAI,&scoreOfChild[MAT(row,col)]);
                        #endif
                    }
                    nowGame_t->stateOfChessboard[MAT(row,col)]=NONE;
                }else{
                    scoreOfChild[MAT(row,col)]=MAX_OF_INT32;
                }
            }
        }
        thpool_wait(*thpoolForAI);
        for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
                while(greatChildrenGroup[i].score<scoreOfChild[MAT(row,col)]){
                    i++;
                    if(i==NUM_OF_CHILDREN){
                        flag=0;
                        break;
                    }
                }
                if(flag){
                    greatChildrenGroup[i].row=row;
                    greatChildrenGroup[i].col=col;
                    greatChildrenGroup[i].score=scoreOfChild[MAT(row,col)];
                    #ifdef LOG  
                        if(row>=RANGE_OF_CHESSBOARD||col>=RANGE_OF_CHESSBOARD){
                            output_log("runningLog","Error:greatChild\n");
                        }
                    #endif
                }else{
                    flag=1;
                }
            }
        }
    }else{
        i=0;
        flag=1;
        for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
                if(nowGame_t->stateOfChessboard[MAT(row,col)]==NONE){
                    if(playerFlag==BLACK_PLAYER){
                        if(judge_forbidden_hand(nowGame_t,row,col,1)==FORBIDDEN_HAND){
                            scoreOfChild[MAT(row,col)]=MIN_OF_INT32;
                            continue;
                        }else{
                            nowGame_t->stateOfChessboard[MAT(row,col)]=AI_BLACK;
                            #ifndef THREAD_POOL
                            scoreOfChild[MAT(row,col)]=value_the_game(nowGame_t,nowAI_t);
                            #else
                            value_the_game_with_thread_pool(nowGame_t,nowAI_t,thpoolForAI,&scoreOfChild[MAT(row,col)]);
                            #endif
                        }
                    }else{
                        nowGame_t->stateOfChessboard[MAT(row,col)]=AI_WHITE;
                        #ifndef THREAD_POOL
                        scoreOfChild[MAT(row,col)]=value_the_game(nowGame_t,nowAI_t);
                        #else
                        value_the_game_with_thread_pool(nowGame_t,nowAI_t,thpoolForAI,&scoreOfChild[MAT(row,col)]);
                        #endif
                    }
                    nowGame_t->stateOfChessboard[MAT(row,col)]=NONE;
                }else{
                    scoreOfChild[MAT(row,col)]=MIN_OF_INT32;
                }
            }
        }
        thpool_wait(*thpoolForAI);
        for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
            for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
                while(greatChildrenGroup[i].score>scoreOfChild[MAT(row,col)]){
                    i++;
                    if(i==NUM_OF_CHILDREN){
                        flag=0;
                        break;
                    }
                }
                if(flag){
                    greatChildrenGroup[i].row=row;
                    greatChildrenGroup[i].col=col;
                    greatChildrenGroup[i].score=scoreOfChild[MAT(row,col)];
                    #ifdef LOG  
                        if(row>=RANGE_OF_CHESSBOARD||col>=RANGE_OF_CHESSBOARD){
                            output_log("runningLog","Error:greatChild\n");
                        }
                    #endif
                }else{
                    flag=1;
                }
            }
        }
    }
}

/**
 * @brief alpha-beta剪枝
*/
#ifndef THREAD_POOL
int32_t alpha_beta_pruning(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,int8_t depth,int32_t alpha,int32_t beta,uint8_t addRow,uint8_t addCol){
#else
int32_t alpha_beta_pruning(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,int8_t depth,int32_t alpha,int32_t beta,uint8_t addRow,uint8_t addCol,threadpool * thpoolForAI){
#endif
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
                if(ansOfCallTheGame==WHITE_WINE||ansOfCallTheGame==FORBIDDEN_HAND){
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
                if(ansOfCallTheGame==WHITE_WINE||ansOfCallTheGame==FORBIDDEN_HAND){
                    scoreOfChild=MAX_OF_INT32;
                }
            }
        }
    }else{
        if(depth==-1){
            #ifndef THREAD_POOL
            scoreOfChild=value_the_game(nowGame_t,nowAI_t);
            #else
            value_the_game_with_thread_pool(nowGame_t,nowAI_t,thpoolForAI,&scoreOfChild);
            #endif
            thpool_wait(*thpoolForAI);
        }else{
            uint8_t flag,i;
            if(depth%2==0){//算MIN层节点分数
                scoreOfChild=MAX_OF_INT32;
                TO_CHECK_t greatChildrenGroup[NUM_OF_CHILDREN];
                if(nowGame_t->playerFlag==WHITE_PLAYER){
                    #ifndef THREAD_POOL
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,BLACK_PLAYER,depth);
                    #else
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,BLACK_PLAYER,depth,thpoolForAI);
                    #endif
                }else{
                    #ifndef THREAD_POOL
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,WHITE_PLAYER,depth);
                    #else
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,WHITE_PLAYER,depth,thpoolForAI);
                    #endif
                }
                for(i=0,flag=1;flag==1&&i<NUM_OF_CHILDREN;i++){
                    #ifndef THREAD_POOL
                    scoreOfChild=MIN(scoreOfChild,alpha_beta_pruning(nowGame_t,nowAI_t,depth-1,alpha,beta,greatChildrenGroup[i].row,greatChildrenGroup[i].col));
                    #else
                    scoreOfChild=MIN(scoreOfChild,alpha_beta_pruning(nowGame_t,nowAI_t,depth-1,alpha,beta,greatChildrenGroup[i].row,greatChildrenGroup[i].col,thpoolForAI));
                    #endif
                    beta=MIN(beta,scoreOfChild);
                    if(beta<=alpha){
                        flag=0; //跳出两层循环
                    }
                }
            }else{
                scoreOfChild=MIN_OF_INT32;
                TO_CHECK_t greatChildrenGroup[NUM_OF_CHILDREN];
                if(nowGame_t->playerFlag==WHITE_PLAYER){
                    #ifndef THREAD_POOL
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,WHITE_PLAYER,depth);
                    #else
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,WHITE_PLAYER,depth,thpoolForAI);
                    #endif
                }else{
                    #ifndef THREAD_POOL
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,BLACK_PLAYER,depth);
                    #else
                    fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,BLACK_PLAYER,depth,thpoolForAI);
                    #endif
                }
                for(i=0,flag=1;flag==1&&i<NUM_OF_CHILDREN;i++){
                    #ifndef THREAD_POOL
                    scoreOfChild=MAX(scoreOfChild,alpha_beta_pruning(nowGame_t,nowAI_t,depth-1,alpha,beta,greatChildrenGroup[i].row,greatChildrenGroup[i].col));
                    #else
                    scoreOfChild=MAX(scoreOfChild,alpha_beta_pruning(nowGame_t,nowAI_t,depth-1,alpha,beta,greatChildrenGroup[i].row,greatChildrenGroup[i].col,thpoolForAI));
                    #endif
                    beta=MAX(beta,scoreOfChild);
                    if(beta<=alpha){
                        flag=0;
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

/**
 * @brief 下一手棋
*/
#ifndef THREAD_POOL
void calc_next_input(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t){
#else
void calc_next_input(ONE_GAME_t * const nowGame_t,ONE_AI_t * const nowAI_t,threadpool * thpoolForAI){
#endif
    int32_t nowMax=MIN_OF_INT32;
    TO_CHECK_t greatChildrenGroup[NUM_OF_CHILDREN];
    if(nowGame_t->playerFlag==BLACK_PLAYER){
        #ifndef THREAD_POOL
        fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,BLACK_PLAYER,MAX_DEPTH_OF_ALPHA_BETA+1);
        #else
        fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,BLACK_PLAYER,MAX_DEPTH_OF_ALPHA_BETA+1,thpoolForAI);
        #endif
        for(uint8_t i=0;i<NUM_OF_CHILDREN;i++){
            #ifndef THREAD_POOL
            nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)]=alpha_beta_pruning(nowGame_t,nowAI_t,MAX_DEPTH_OF_ALPHA_BETA,MIN_OF_INT32,MAX_OF_INT32,greatChildrenGroup[i].row,greatChildrenGroup[i].col);
            #else
            nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)]=alpha_beta_pruning(nowGame_t,nowAI_t,MAX_DEPTH_OF_ALPHA_BETA,MIN_OF_INT32,MAX_OF_INT32,greatChildrenGroup[i].row,greatChildrenGroup[i].col,thpoolForAI);
            #endif
            if(nowMax<nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)]){
                nowMax=nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)];
                nowGame_t->blackInputChessPlace.row=greatChildrenGroup[i].row;
                nowGame_t->blackInputChessPlace.col=greatChildrenGroup[i].col;
            }   
        }
        nowGame_t->blackInputChessPlace.flag=INPUT_UNUSED;
    }else{
        #ifndef THREAD_POOL
        fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,WHITE_PLAYER,MAX_DEPTH_OF_ALPHA_BETA+1);
        #else
        fine_great_children(nowGame_t,nowAI_t,greatChildrenGroup,WHITE_PLAYER,MAX_DEPTH_OF_ALPHA_BETA+1,thpoolForAI);
        #endif
        for(uint8_t i=0;i<NUM_OF_CHILDREN;i++){
            #ifndef THREAD_POOL
            nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)]=alpha_beta_pruning(nowGame_t,nowAI_t,MAX_DEPTH_OF_ALPHA_BETA,MIN_OF_INT32,MAX_OF_INT32,greatChildrenGroup[i].row,greatChildrenGroup[i].col);
            #else
            nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)]=alpha_beta_pruning(nowGame_t,nowAI_t,MAX_DEPTH_OF_ALPHA_BETA,MIN_OF_INT32,MAX_OF_INT32,greatChildrenGroup[i].row,greatChildrenGroup[i].col,thpoolForAI);
            #endif
            if(nowMax<nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)]){
                nowMax=nowAI_t->scoreOfEveryPlace[MAT(greatChildrenGroup[i].row,greatChildrenGroup[i].col)];
                nowGame_t->whiteInputChessPlace.row=greatChildrenGroup[i].row;
                nowGame_t->whiteInputChessPlace.col=greatChildrenGroup[i].col;
            }   
        }
        nowGame_t->whiteInputChessPlace.flag=INPUT_UNUSED;
    }
}