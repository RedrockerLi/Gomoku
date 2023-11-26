#include <stdio.h>
#include <stdint.h>

#include "game.h"
#include "chessboard.h"

static uint8_t game_mode;

/**
 * @brief 比赛初始化
*/
void game_init(void){
    chessboard_init();
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

