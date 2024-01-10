#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "AI.h"
#include "game.h"
#include  "train.h"

/**
 * @brief 初始化
*/
void particles_init(ONE_PARTICLE_t *particles){
    srand((unsigned)time(NULL));
    for(uint8_t i=0;i<NUM_OF_PARTICLE;i++){
        particles[i].nowScores[SINGLE_0B]=rand()%100;
        particles[i].nowScores[SINGLE_1B]=rand()%100;
        particles[i].nowScores[SINGLE_2B]=rand()%100;
        particles[i].nowScores[TWO_JUMP3]=particles[i].nowScores[SINGLE_0B]+rand()%1000;
        particles[i].nowScores[TWO_JUMP2]=particles[i].nowScores[SINGLE_0B]+rand()%1000;
        particles[i].nowScores[TWO_JUMP1]=particles[i].nowScores[SINGLE_0B]+rand()%1000;
        particles[i].nowScores[TWO_0B]=particles[i].nowScores[SINGLE_0B]+rand()%1000;
        particles[i].nowScores[TWO_1B]=particles[i].nowScores[SINGLE_0B]+rand()%1000;
        particles[i].nowScores[TWO_2B]=particles[i].nowScores[SINGLE_0B]+rand()%1000;
        particles[i].nowScores[THREE_JUMP2]=particles[i].nowScores[TWO_JUMP3]+10*(rand()%10000);
        particles[i].nowScores[LIVE_THREE_JUMP1]=particles[i].nowScores[TWO_JUMP3]+10*(rand()%10000);
        particles[i].nowScores[FIGHT_THREE_JUMP1]=particles[i].nowScores[TWO_JUMP3]+10*(rand()%10000);
        particles[i].nowScores[FAKE_THREE]=particles[i].nowScores[TWO_JUMP3]+10*(rand()%10000);
        particles[i].nowScores[LIVE_THREE]=particles[i].nowScores[TWO_JUMP3]+10*(rand()%10000);
        particles[i].nowScores[JUMP_FOUR]=particles[i].nowScores[LIVE_THREE]+100*(rand()%100000);
        particles[i].nowScores[FIGHT_FOUR]=particles[i].nowScores[LIVE_THREE]+100*(rand()%100000);
        particles[i].nowScores[LIVE_FOUR]=particles[i].nowScores[LIVE_THREE]+100*(rand()%100000);
        particles[i].nowScores[FIVE]=particles[i].nowScores[LIVE_FOUR]+1000*(rand()%1000000);
    }
    for(uint8_t i=0;i<NUM_OF_PARTICLE;i++){
        for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
            particles[i].nowSpeed[j]=particles[i].nowScores[j]*(rand()%100)/100;
            if((rand()%2)!=0){
                particles[i].nowSpeed[j]=-particles[i].nowSpeed[j];
            }
        }
    }
}

/**
 * @brief 自动进行一场比赛
*/
uint8_t carry_out_one_game(ONE_AI_VS_AI_t *nowGame_t){
    game_init(&nowGame_t->thisGame_t);
    uint8_t nums=0;
    uint8_t flag=0;
    while (nowGame_t->thisGame_t.gameWinner==CONTINUE){
        if(nums==225){
            return CONTINUE;
        }
        if(nowGame_t->thisGame_t.playerFlag==BLACK_PLAYER){
            if(flag==0){
                nowGame_t->thisGame_t.blackInputChessPlace.row=nowGame_t->thisGame_t.blackInputChessPlace.col=7;
                nowGame_t->thisGame_t.blackInputChessPlace.flag=INPUT_UNUSED;
                flag++;
            }
            calc_next_input(&nowGame_t->thisGame_t,&nowGame_t->blackPlayer_t);
        }else{
            calc_next_input(&nowGame_t->thisGame_t,&nowGame_t->whitePlayer_t);
        }
        place_the_chess(&nowGame_t->thisGame_t);
        nowGame_t->thisGame_t.gameWinner=call_the_game(&nowGame_t->thisGame_t,nowGame_t->thisGame_t.lastBlackInputChessPlace.row,nowGame_t->thisGame_t.lastBlackInputChessPlace.col,1);
        if(nowGame_t->thisGame_t.gameWinner==CONTINUE){
            if(nowGame_t->thisGame_t.playerFlag==BLACK_PLAYER){
                    if(judge_forbidden_hand(&nowGame_t->thisGame_t,nowGame_t->thisGame_t.lastBlackInputChessPlace.row,nowGame_t->thisGame_t.blackInputChessPlace.col,2)==FORBIDDEN_HAND){
                    nowGame_t->thisGame_t.gameWinner=WHITE_WINE;
                    output_log("trainLog","Err:FORBIDDEN_HAND");
                }
            }
        }else if(nowGame_t->thisGame_t.gameWinner==FORBIDDEN_HAND){
            output_log("trainLog","Err:FORBIDDEN_HAND");
        }
        if(nowGame_t->thisGame_t.gameWinner!=CONTINUE){
            return nowGame_t->thisGame_t.gameWinner;
        }
        if(nowGame_t->thisGame_t.playerFlag==BLACK_PLAYER){
            nowGame_t->thisGame_t.playerFlag=WHITE_PLAYER;
        }else{
            nowGame_t->thisGame_t.playerFlag=BLACK_PLAYER;
        }
        nums++;
    }
}

/**
 * @brief 更新这一轮的最大值情况
*/
void refresh_max(ONE_PARTICLE_t *particles,int32_t *bestScoresInAll){
    uint8_t ansOfOneGame;
    ONE_AI_VS_AI_t nowGame_t;
    for(int i=0;i<NUM_OF_PARTICLE;i++){
        AI_init(&nowGame_t.blackPlayer_t,particles[i].bestScores);
        AI_init(&nowGame_t.whitePlayer_t,particles[i].nowScores);
        ansOfOneGame=carry_out_one_game(&nowGame_t);
        if(ansOfOneGame==WHITE_WINE||ansOfOneGame==CONTINUE){
            for(int j=0;j<LENGTH_OF_STATES;j++){
                particles[i].bestScores[j]=particles[i].nowScores[j];
            }
        }
    }
    for(int i=0;i<NUM_OF_PARTICLE;i++){
        AI_init(&nowGame_t.blackPlayer_t,bestScoresInAll);
        AI_init(&nowGame_t.whitePlayer_t,particles[i].nowScores);
        ansOfOneGame=carry_out_one_game(&nowGame_t);
        if(ansOfOneGame==WHITE_WINE||ansOfOneGame==CONTINUE){
            for(int j=0;j<LENGTH_OF_STATES;j++){
                bestScoresInAll[j]=particles[i].nowScores[j];
            }
        }
    }
}

/**
 * @brief 更新每一个粒子的速度
*/
void refresh_speed(ONE_PARTICLE_t *particles,const int32_t * const bestScoresInAll,const PARAMETER_t const *parameters){
    for(uint8_t i=0;i<NUM_OF_PARTICLE;i++){
        for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
            if(particles[i].nowSpeed[j]>0){//绝对值向上取整
                particles[i].nowSpeed[j]=(parameters->omega*particles[i].nowSpeed[j]+99)/100;
            }else{
                particles[i].nowSpeed[j]=(parameters->omega*particles[i].nowSpeed[j]-99)/100;
            }
            if(particles[i].bestScores[j]-particles[i].nowScores[j]>0){
                particles[i].nowSpeed[j]=particles[i].nowSpeed[j]+(parameters->c_1*(particles[i].bestScores[j]-particles[i].nowScores[j])+99)/100;
            }else{
                particles[i].nowSpeed[j]=particles[i].nowSpeed[j]+(parameters->c_1*(particles[i].bestScores[j]-particles[i].nowScores[j])-99)/100;
            }
            if(bestScoresInAll[j]-particles[i].nowScores[j]>0){
                particles[i].nowSpeed[j]=particles[i].nowSpeed[j]+(parameters->c_2*(bestScoresInAll[j]-particles[i].nowScores[j])+99)/100;
            }else{
                particles[i].nowSpeed[j]=particles[i].nowSpeed[j]+(parameters->c_2*(bestScoresInAll[j]-particles[i].nowScores[j])-99)/100;
            }
        }
    }
}

/**
 * @brief 更新每一个粒子的分数数组
*/
void refresh_scores(ONE_PARTICLE_t *particles,const PARAMETER_t * const parameters){
    int32_t lastScores;
    for(uint8_t i=0;i<NUM_OF_PARTICLE;i++){
        for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
            lastScores=particles[i].nowScores[j];
            particles[i].nowScores[j]=particles[i].nowScores[j]+particles[i].nowSpeed[j];
            if(particles[i].nowSpeed[j]>0){
                if(particles[i].nowScores[j]<lastScores){
                    particles[i].nowScores[j]=lastScores;
                }
            }else if(particles[i].nowSpeed[j]<0){
                if(particles[i].nowScores[j]>lastScores){
                    particles[i].nowScores[j]=lastScores;
                }
            }
        }
    }
}

/**
 * @brief 更新进度
*/
void refresh_progress(const ONE_PARTICLE_t * const particles,const int32_t * const bestScoresInAll,uint8_t *numsInTheCircle){
    uint16_t error=0;
    *numsInTheCircle=0;
    for(uint8_t i=0;i<NUM_OF_PARTICLE;i++){
        for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
            error=error+abs(bestScoresInAll[j]-particles[i].nowScores[j])*10000/bestScoresInAll[j];
        }
        if(error<500){
            (*numsInTheCircle)++;
        }
    }
}