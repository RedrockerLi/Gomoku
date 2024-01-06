#ifndef _MAIN_H
#define _MAIN_H
#include <stdint.h>

// #define GAME        //比赛模式
// #define TRAIN    //训练模式
#define TEST_TIME   //检测AI计算时间

// #define CLEAR //打开则有清屏功能
// #define ONLY_BLACKPLEAR //打开则只有黑棋玩家，用于实验禁手
// #define SHOW_INPUT //打开即可从input文件中读入数据并打印出来
#define LOG //打开则会在运行时输出一些日志

void output_log(uint8_t *output);

#endif