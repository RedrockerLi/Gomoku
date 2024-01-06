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
#include "threadPool.h"

#ifndef GAME
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
        printf("Error.Cannot find file runningLog\n");
    }
    close(file);
}

/**
 * @brief 日志输出
*/
void output_log(uint8_t *fileName,uint8_t *output){
    int file;
    if((file=open(fileName,O_WRONLY|O_APPEND))==-1){
        printf("Error.Cannot find file runningLog\n");
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
    #ifdef LOG
    log_init("runningLog");
    time_t currentTime;
    time(&currentTime);
    uint8_t *timeString = ctime(&currentTime);
    output_log("runningLog","Begin:");
    output_log("runningLog",timeString);
    #endif
    #ifdef THREAD_POOL
    #define NUM_OF_THREAD 16
    threadpool thpoolForAI=thpool_init(NUM_OF_THREAD);
    #endif
    #ifdef GAME
    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    const int32_t scoreChoose[LENGTH_OF_STATES]={5,10,20,15,20,30,60,100,50,1000,5000,100000,800000,20000000,20000000,800000000};
    AI_init(&classAI_t,scoreChoose);
    game_init(&classGame_t);
    draw_the_start_page();
    input_game_mode(&classGame_t);
    draw_the_chessboard(&classGame_t);
    #ifndef THREAD_POOL
    continue_the_game(&classGame_t,&classAI_t);
    #else
    continue_the_game(&classGame_t,&classAI_t,&thpoolForAI);
    #endif
    #endif
    #ifdef TEST_TIME
    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    const int32_t scoreChoose[LENGTH_OF_STATES]={5,10,20,15,20,30,60,100,50,1000,500,10000,20000,30000,60000,80000};
    game_init(&classGame_t);
    AI_init(&classAI_t,scoreChoose);
    set_black(&classGame_t,7,7);
    set_black(&classGame_t,8,7);
    classGame_t.gameMode=PERSON_VS_COMPUTER;
    classGame_t.playerFlag=WHITE_PLAYER;
    draw_the_chessboard(&classGame_t);
    #ifndef THREAD_POOL
    continue_the_game(&classGame_t,&classAI_t);
    #else
    continue_the_game(&classGame_t,&classAI_t,&thpoolForAI);
    #endif
    #endif
    #ifdef LOG
    output_log("runningLog","Exit:");
    time(&currentTime);
    timeString = ctime(&currentTime);
    output_log("runningLog",timeString);
    #endif
    return 0;
}