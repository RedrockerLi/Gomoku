#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "AI.h"
#include "game.h"
#include "main.h"
#include "train.h"
#include "threadPool.h"

#ifdef TEST_TIME
/**
 * @brief 在棋盘放黑子
*/
void set_black(ONE_GAME_t * const nowGame_t,uint8_t row,uint8_t col){
    nowGame_t->stateOfChessboard[MAT(row,col)]=BLACK;
}
#endif
/***************************复杂禁手********************************************/
// set_black(&classGame_t,5,5);
// set_black(&classGame_t,5,6);
// set_black(&classGame_t,6,7);
// set_black(&classGame_t,7,7);
// set_black(&classGame_t,8,6);
// set_black(&classGame_t,8,5);
// set_black(&classGame_t,7,4);
// set_black(&classGame_t,6,4);
/****************************判断胜负bug********************************************/
// set_black(&classGame_t,15,0);
// set_black(&classGame_t,14,0);
// set_black(&classGame_t,13,0);
#ifdef LOG
/**
 * @brief 日志初始化
*/
void log_init(uint8_t *fileName){
    int file;
    if((file=open(fileName,O_WRONLY|O_TRUNC))==-1){
        printf("Error.Cannot find file %s\n",fileName);
    }
    close(file);
}

/**
 * @brief 日志输出
*/
void output_log(uint8_t *fileName,uint8_t *output){
    int file;
    if((file=open(fileName,O_WRONLY|O_APPEND))==-1){
        printf("Error.Cannot find file %s\n",fileName);
    }
    while(write(file,output,strlen(output))==-1);
    close(file);
}

/**
 * @brief 整数转字符串
*/
char* int_to_string(int num){
    int temp = num;
    int digits = 0;
    if (temp == 0) {
        digits = 1;
    } else {
        while (temp != 0) {
            temp /= 10;
            digits++;
        }
    }
    int sign = num < 0 ? 1 : 0;
    int size = digits + sign + 1;
    char* str = (char*)malloc(size * sizeof(char));
    if (str == NULL) {
        return NULL;
    }
    if (sign) {
        num = -num;  
        str[0] = '-';
    }
    for (int i = digits - 1 + sign; i >= sign; i--) {
        str[i] = '0' + num % 10;
        num /= 10;
    }
    str[size - 1] = '\0'; 
    return str;
}
#endif

int main(){
    #ifdef SHOW_INPUT
    #define BUFF 1024
    uint8_t fileBuf[BUFF];
    uint16_t size;
    int file;
    if((file=open("input",O_RDONLY))==-1){
        printf("Error.Cannot find file input\n");
        return 1;
    }
    size=read(file,fileBuf,BUFF);
    fileBuf[size]='\0';
    close(file);
    printf("%s",fileBuf);
    #endif
    #ifdef DEBUG_LOG
    log_init("debugLog");
    time_t currentTime;
    time(&currentTime);
    uint8_t *timeString = ctime(&currentTime);
    output_log("debugLog","Begin:");
    output_log("debugLog",timeString);
    #endif
    #ifdef THREAD_POOL_FOR_AI
    #define NUM_OF_THREAD 2
    threadpool thpoolForAI=thpool_init(NUM_OF_THREAD);
    #endif
    #ifdef GAME
    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    // const int32_t scoreChoose[LENGTH_OF_STATES]={0,0,366,0,0,2918,4031,2400,1885,0,41096,95202,3203,0,1736115,2055141,989820,28867934};
    // const int32_t scoreChoose[LENGTH_OF_STATES]={0,325,801,0,0,0,5911,12976,0,31364,0,22189,0,3550,4156804,1876986,6266044,92611006};
    // const int32_t scoreChoose[LENGTH_OF_STATES]={24,0,1440,0,0,0,2919,2781,8862,119900,41250,28107,0,0,0,102045,4180884,173766529};
    // AI_init(&classAI_t,scoreChoose);
    classAI_t.scoreOfChessStates[SINGLE_0B]=0;
    classAI_t.scoreOfChessStates[SINGLE_1B]=5;
    classAI_t.scoreOfChessStates[SINGLE_2B]=41;
    classAI_t.scoreOfChessStates[TWO_JUMP3]=10;
    classAI_t.scoreOfChessStates[TWO_JUMP2]=81;
    classAI_t.scoreOfChessStates[TWO_JUMP1]=325;
    classAI_t.scoreOfChessStates[TWO_0B]=0;
    classAI_t.scoreOfChessStates[TWO_1B]=11;
    classAI_t.scoreOfChessStates[TWO_2B]=324;
    classAI_t.scoreOfChessStates[THREE_JUMP2]=2953;
    classAI_t.scoreOfChessStates[LIVE_THREE_JUMP1]=23626;
    classAI_t.scoreOfChessStates[FIGHT_THREE_JUMP1]=11812;
    classAI_t.scoreOfChessStates[FAKE_THREE]=11812;
    classAI_t.scoreOfChessStates[LIVE_THREE]=23625;
    classAI_t.scoreOfChessStates[JUMP_FOUR]=18910;
    classAI_t.scoreOfChessStates[FIGHT_FOUR]=9454;
    classAI_t.scoreOfChessStates[LIVE_FOUR]=18909;
    classAI_t.scoreOfChessStates[FIVE]=1512073;
    game_init(&classGame_t);
    draw_the_start_page();
    input_game_mode(&classGame_t);
    draw_the_chessboard(&classGame_t);
    #ifndef THREAD_POOL_FOR_AI
    continue_the_game(&classGame_t,&classAI_t);
    #else
    continue_the_game(&classGame_t,&classAI_t,&thpoolForAI);
    #endif
    #endif
    #ifdef TEST_TIME
    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    const int32_t scoreChoose[LENGTH_OF_STATES]={0,10,20,15,20,30,60,100,50,1000,5000,10000,300000,100000,100000,50000000,400000000};
    game_init(&classGame_t);
    AI_init(&classAI_t,scoreChoose);
    set_black(&classGame_t,5,7);
    classGame_t.gameMode=PERSON_VS_COMPUTER;
    classGame_t.playerFlag=WHITE_PLAYER;
    draw_the_chessboard(&classGame_t);
    #ifndef THREAD_POOL_FOR_AI
    continue_the_game(&classGame_t,&classAI_t);
    #else
    continue_the_game(&classGame_t,&classAI_t,&thpoolForAI);
    #endif
    #endif
    #ifdef DEBUG_LOG
    output_log("debugLog","Exit:");
    time(&currentTime);
    timeString = ctime(&currentTime);
    output_log("debugLog",timeString);
    #endif
    #ifdef TRAIN
    #define NUM_OF_THREAD 16
    time_t currentTime;
    uint8_t *timeString;
    log_init("trainLog");
    output_log("trainLog","Begin: ");
    time(&currentTime);
    timeString = ctime(&currentTime);
    output_log("trainLog",timeString);
    output_log("trainLog","\n");
    ONE_PARTICLE_t partices[NUM_OF_PARTICLE];
    PARAMETER_t parameters;
    int32_t bestScoresInAll[LENGTH_OF_STATES]={1,1,2,1,2,3,60,10,5,100,500,1000,30000,10000,10000,5000000,40000000,400000000};
    uint8_t numsInTheCircle=0;//在全局最大周围的粒子
    uint16_t counters=0;
    parameters.omega=parameters.c_1=parameters.c_2=100;
    particles_init(partices);
    while(numsInTheCircle<=29&&counters<200){
        counters++;
        refresh_max(partices,bestScoresInAll);
        refresh_speed(partices,bestScoresInAll,&parameters);
        refresh_scores(partices,&parameters);
        refresh_progress(partices,bestScoresInAll,&numsInTheCircle);
        if(parameters.omega>40){
            parameters.omega--;
        }
        if(parameters.c_1>40){
            parameters.c_1--;
        }
        if(parameters.c_2>40){
            parameters.c_2--;
        }
        output_log("trainLog","Time: ");
        time(&currentTime);
        timeString = ctime(&currentTime);
        output_log("trainLog",timeString);
        uint8_t *message;
        message=int_to_string(counters);
        output_log("trainLog","Interation times: ");
        output_log("trainLog",message);
        output_log("trainLog","\n");
        output_log("trainLog","bestScoresInAll:\n");
        for(uint8_t i=0;i<LENGTH_OF_STATES;i++){
            message=int_to_string(bestScoresInAll[i]);
            output_log("trainLog",message);
            output_log("trainLog"," ");
        }
        output_log("trainLog","\n");
        for(uint8_t i=0;i<NUM_OF_PARTICLE;i++){
            output_log("trainLog","partices:\n");
            output_log("trainLog","bestScoress:\n");
            for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
                message=int_to_string(partices[i].bestScores[j]);
                output_log("trainLog",message);
                output_log("trainLog"," ");
            }
            output_log("trainLog","\n");
            output_log("trainLog","nowScores:\n");
            for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
                message=int_to_string(partices[i].nowScores[j]);
                output_log("trainLog",message);
                output_log("trainLog"," ");
            }
            output_log("trainLog","\n");
            output_log("trainLog","nowSpeed:\n");
            for(uint8_t j=0;j<LENGTH_OF_STATES;j++){
                message=int_to_string(partices[i].nowSpeed[j]);
                output_log("trainLog",message);
                output_log("trainLog"," ");
            }
            output_log("trainLog","\n\n");
        }
    }
    time(&currentTime);
    timeString = ctime(&currentTime);
    output_log("trainLog",timeString);
    output_log("trainLog","\n");
    #endif
    return 0;
}