#ifndef _MAIN_H
#define _MAIN_H
#include <stdint.h>

// #define GAME        //比赛模式
// #define TEST_TIME   //检测AI计算时间
#define TRAIN    //训练模式

// #define THREAD_POOL_FOR_AI //打开则有线程池功能
// #define CLEAR //打开则有清屏功能
// #define ONLY_BLACKPLEAR //打开则只有黑棋玩家，用于实验禁手
// #define SHOW_INPUT //打开即可从input文件中读入数据并打印出来
#define LOG //控制日志函数,需要LOG必须打开(TRAIN模式必须打开)
// #define DEBUG_LOG //打开则会在运行时输出一些日志,最好搭配检测AI时间模式使用,否则生成的文件会过大.

void log_init(uint8_t *fileName);
void output_log(uint8_t *fileName,uint8_t *output);
char* int_to_string(int num);
#endif