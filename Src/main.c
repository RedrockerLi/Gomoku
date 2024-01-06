#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "AI.h"
#include "game.h"
#include "main.h"

/**
 * @brief 在棋盘放黑子
*/
void set_black(ONE_GAME_t * const nowGame_t,uint8_t row,uint8_t col){
    nowGame_t->stateOfChessboard[MAT(row,col)]=BLACK;
}
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
    write(file,output,strlen(output));
    close(file);
}

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
    #ifdef GAME
    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    const int32_t scoreChoose[LENGTH_OF_STATES]={5,10,20,15,20,30,60,100,50,1000,500,100000,200000,300000,6000000,8000000};
    AI_init(&classAI_t,scoreChoose);
    game_init(&classGame_t);
    draw_the_start_page();
    input_game_mode(&classGame_t);
    draw_the_chessboard(&classGame_t);
    continue_the_game(&classGame_t,&classAI_t);
    #endif
    #ifdef TEST_TIME
    ONE_GAME_t classGame_t;
    ONE_AI_t classAI_t;
    const int32_t scoreChoose[LENGTH_OF_STATES]={5,10,20,15,20,30,60,100,50,1000,500,10000,20000,30000,60000,80000};
    game_init(&classGame_t);
    AI_init(&classAI_t,scoreChoose);
    set_black(&classGame_t,7,7);
    classGame_t.gameMode=PERSON_VS_COMPUTER;
    classGame_t.playerFlag=WHITE_PLAYER;
    draw_the_chessboard(&classGame_t);
    continue_the_game(&classGame_t,&classAI_t);
    #endif
    #ifdef LOG
    output_log("runningLog","Exit:");
    time(&currentTime);
    timeString = ctime(&currentTime);
    output_log("runningLog",timeString);
    #endif
    return 0;
}