#include <stdio.h>
#include <stdint.h>

#include "game.h"
#include "chessboard.h"
#include "AI.h"

#ifdef GAME
static uint8_t game_mode;
const int8_t *stateOfChessboard_p;

/**
 * @brief 比赛初始化
*/
void game_init(void){
    stateOfChessboard_p=get_state_of_chessboard_point();
    chessboard_init(stateOfChessboard_p);
}

//             .-'''-.                        .-'''-.                             //
//            '   _    \                     '   _    \                           //
//          /   /` '.   \  __  __   ___    /   /` '.   \     .                    //
//   .--./).   |     \  ' |  |/  `.'   `. .   |     \  '   .'|                    //
//  /.''\\ |   '      |  '|   .-.  .-.   '|   '      |  '.'  |                    //
// | |  | |\    \     / / |  |  |  |  |  |\    \     / /<    |                    //
//  \`-' /  `.   ` ..' /  |  |  |  |  |  | `.   ` ..' /  |   | ____      _    _   //
//  /("'`      '-...-'`   |  |  |  |  |  |    '-...-'`   |   | \ .'     | '  / |  //
//  \ '---.               |  |  |  |  |  |               |   |/  .     .' | .' |  //
//   /'""'.\              |__|  |__|  |__|               |    /\  \    /  | /  |  //
//  ||     ||                                            |   |  \  \  |   `'.  |  //
//  \'. __//                                             '    \  \  \ '   .'|  '/ //
//   `'---'                                             '------'  '---'`-'  `--'  //
// Choose game mode:                                                              //
//      A:Person VS Person  B:Person VS Computer  C:Computer VS Person            //

/**
 * @brief 绘制首页
*/
void draw_the_start_page(void){
    printf("             .-'''-.                        .-'''-.                             \n");
    printf("            '   _    \\                     '   _    \\                           \n");
    printf("          /   /` '.   \\  __  __   ___    /   /` '.   \\     .                    \n");
    printf("   .--./).   |     \\  ' |  |/  `.'   `. .   |     \\  '   .'|                    \n");
    printf("  /.''\\\\ |   '      |  '|   .-.  .-.   '|   '      |  '.'  |                    \n");
    printf(" | |  | |\\    \\     / / |  |  |  |  |  |\\    \\     / /<    |                    \n");
    printf("  \\`-' /  `.   ` ..' /  |  |  |  |  |  | `.   ` ..' /  |   | ____      _    _   \n");
    printf("  /(\"'`      '-...-'`   |  |  |  |  |  |    '-...-'`   |   | \\ .'     | '  / |  \n");
    printf("  \\ '---..              |  |  |  |  |  |               |   |/  .     .' | .' |  \n");
    printf("   /'""'.  \\\\             |__|  |__|  |__|               |    /\\  \\    /  | /  |  \n");
    printf("  ||     ||                                            |   |  \\  \\  |   `'.  |  \n");
    printf("  \\'. __//                                             '    \\  \\  \\ '   .'|  '/ \n");
    printf("   `'---'                                             '------'  '---'`-'  `--'  \n");
    printf(" Choose game mode:                                                              \n");
    printf("      A:Person VS Person  B:Person VS Computer  C:Computer VS Person            \n");
}

/**
 * @brief 输入比赛模式
*/
void input_game_mode(void){
    uint8_t i;
    scanf("%c",&i);
    switch(i)
    {
    case 'A':
        game_mode=PERSON_VS_PERSON;
        break;
    case 'B':
        game_mode=PERSON_VS_COMPUTER;
        break;
    case 'C':
        game_mode=COMPUTER_VS_PERSON;
        break;
    default:
        printf("Please choose the right game mode:\n");
        input_game_mode();
        break;
    }
}

/**
 * @brief 绘制棋盘
*/
void draw_the_chessboard(const int8_t *stateOfChessboard){
    int8_t i,j;
    for(j=0;j<RANGE_OF_CHESSBOARD;j++){
        for(i=0;i<RANGE_OF_CHESSBOARD;i++){
            if(i<RANGE_OF_CHESSBOARD-1){
                if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                    printf("*--");
                }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                    printf(" --");
                }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                    printf("#--");
                }
            }else{
                if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                    printf("*\n");
                }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                    printf(" \n");
                }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                    printf("#\n");
                }
            }
        }
        if(j<RANGE_OF_CHESSBOARD-1){
            for(i=0;i<RANGE_OF_CHESSBOARD;i++){
                if(i<RANGE_OF_CHESSBOARD-1){
                    printf("|  ");
                }else{
                    printf("|\n");
                }
            }
        }
    }
}
#endif