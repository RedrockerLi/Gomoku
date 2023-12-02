#include <stdio.h>
#include <stdint.h>

#include "game.h"
#include "AI.h"

#ifdef GAME
static uint8_t stateOfChessboard[RANGE_OF_CHESSBOARD*RANGE_OF_CHESSBOARD]; //棋盘
static uint8_t gameMode; //比赛模式
static uint8_t playerFlag; //落子权归属
uint8_t gameWinner;//比赛结果
static INPUT_CHESS_PLACE_t blackInputChessPlace; //黑子落子接口
static INPUT_CHESS_PLACE_t whiteInputChessPlace; //白子落子接口

void input_chess_place(void);

/**
 * @brief 初始化游戏
*/
void gameInit(void){
    //棋盘初始化
    for(int8_t i=0;i<RANGE_OF_CHESSBOARD;i++){
        for(int8_t j=0;j<RANGE_OF_CHESSBOARD;j++){
            stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]=NONE;
        }
    }
    //先手初始化
    playerFlag=BLACK_PLAYER;
    //落子接口初始化
    blackInputChessPlace.flag=INPUT_USED;
    whiteInputChessPlace.flag=INPUT_USED;
    //比赛 结果初始化
    gameWinner=CONTINUE;
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
    int8_t clear;
    while((clear=getchar())!='\n'&&clear!=EOF);
    switch(i)
    {
    case 'A':
        gameMode=PERSON_VS_PERSON;
        break;
    case 'B':
        gameMode=PERSON_VS_COMPUTER;
        break;
    case 'C':
        gameMode=COMPUTER_VS_PERSON;
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
void draw_the_chessboard(void){
    uint8_t i,j;
    for(i=0;i<RANGE_OF_CHESSBOARD;i++){
        for(j=0;j<RANGE_OF_CHESSBOARD-1;j++){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                printf("*——");
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                printf(" ——");
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                printf("#——");
            }   
        }
        if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
            printf("*\n");
        }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
            printf(" \n");
        }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
            printf("#\n");
        }
        if(i<RANGE_OF_CHESSBOARD-1){
            for(j=0;j<RANGE_OF_CHESSBOARD-1;j++){
                printf("|  ");
            }
            printf("|\n");
        }
    }
}

/**
 * @brief 处理非法输入
*/
void deal_invalid_input(void){
    if(playerFlag==BLACK_PLAYER){
        if(blackInputChessPlace.x>=RANGE_OF_CHESSBOARD||blackInputChessPlace.y>=RANGE_OF_CHESSBOARD){
            blackInputChessPlace.flag=INDEX_OUT_OF_BOUNDS;
            input_chess_place();
        }
        if(stateOfChessboard[blackInputChessPlace.x*RANGE_OF_CHESSBOARD+blackInputChessPlace.y]!=NONE){
            blackInputChessPlace.flag=OCCUPYED;
            input_chess_place();
        }
    }else if(playerFlag==WHITE_PLAYER){
        if(whiteInputChessPlace.x>=RANGE_OF_CHESSBOARD||whiteInputChessPlace.y>=RANGE_OF_CHESSBOARD){
            whiteInputChessPlace.flag=INDEX_OUT_OF_BOUNDS;
            input_chess_place();
        }
        if(stateOfChessboard[whiteInputChessPlace.x*RANGE_OF_CHESSBOARD+whiteInputChessPlace.y]!=NONE){
            whiteInputChessPlace.flag=OCCUPYED;
            input_chess_place();
        }
    }
}

/**
 * @brief 落子输入,格式：x y（数字）
*/
void input_chess_place(void){
    int32_t x,y;
    if(playerFlag==BLACK_PLAYER){
        if(blackInputChessPlace.flag==INPUT_USED){
            if(gameMode==PERSON_VS_PERSON||gameMode==PERSON_VS_COMPUTER){
                printf("please input the next place of black:x y\n");
                scanf("%d %d",&x,&y);
                int8_t clear;
                while((clear=getchar())!='\n'&&clear!=EOF);
                blackInputChessPlace.x=x;
                blackInputChessPlace.y=y;
                blackInputChessPlace.flag=INPUT_UNUSED;
                deal_invalid_input();
            }
        }else if(blackInputChessPlace.flag<INPUT_UNUSED){
            switch(blackInputChessPlace.flag){
                case OCCUPYED:
                    printf("Too crowded! Please choose a right one.\n");
                    break;
                case INDEX_OUT_OF_BOUNDS:
                    printf("The chessboard can't be bigger. Please choose a right one.\n");
                    break;
            }
            scanf("%d %d",&x,&y);
            int8_t clear;
            while((clear=getchar())!='\n'&&clear!=EOF);
            blackInputChessPlace.x=x;
            blackInputChessPlace.y=y;
            blackInputChessPlace.flag=INPUT_UNUSED;
            deal_invalid_input();
        }
    }else if(playerFlag==WHITE_PLAYER){
        if(whiteInputChessPlace.flag==INPUT_USED){
            if(gameMode==PERSON_VS_PERSON||gameMode==COMPUTER_VS_PERSON){
                printf("please input the next place of white:x y\n");
                scanf("%d %d",&x,&y);
                int8_t clear;
                while((clear=getchar())!='\n'&&clear!=EOF);
                whiteInputChessPlace.x=x;
                whiteInputChessPlace.y=y;
                whiteInputChessPlace.flag=INPUT_UNUSED;
                deal_invalid_input();
            }
        }else if(whiteInputChessPlace.flag<INPUT_UNUSED){
            switch(whiteInputChessPlace.flag){
                case OCCUPYED:
                    printf("Too crowded! Please choose a right one.\n");
                    break;
                case INDEX_OUT_OF_BOUNDS:
                    printf("The chessboard can't be bigger. Please choose a right one.\n");
                    break;
            }
            scanf("%d %d",&x,&y);
            int8_t clear;
            while((clear=getchar())!='\n'&&clear!=EOF);
            whiteInputChessPlace.x=x;
            whiteInputChessPlace.y=y;
            whiteInputChessPlace.flag=INPUT_UNUSED;
            deal_invalid_input();
        }
    }
}

/**
 * @brief 棋盘落子
*/
void place_the_chess(void){
    if(playerFlag==BLACK_PLAYER&&blackInputChessPlace.flag==INPUT_UNUSED){
        stateOfChessboard[blackInputChessPlace.x*RANGE_OF_CHESSBOARD+blackInputChessPlace.y]=BLACK;
        blackInputChessPlace.flag=INPUT_USED;
    }else if(playerFlag==WHITE_PLAYER&&whiteInputChessPlace.flag==INPUT_UNUSED){
        stateOfChessboard[whiteInputChessPlace.x*RANGE_OF_CHESSBOARD+whiteInputChessPlace.y]=WHITE;
        whiteInputChessPlace.flag=INPUT_USED;
    }
}

/**
 * @brief 判断胜负
*/
uint8_t call_the_game(void){
    uint8_t countBlack,countWhite;
    //按行搜索
    for(uint8_t i=0;i<RANGE_OF_CHESSBOARD;i++){
        countBlack=countWhite=0;
        for(uint8_t j=0;j<RANGE_OF_CHESSBOARD;j++){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }
    //按列搜索
    for(uint8_t j=0;j<RANGE_OF_CHESSBOARD;j++){
        countBlack=countWhite=0;
        for(uint8_t i=0;i<RANGE_OF_CHESSBOARD;i++){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }
    //斜向搜索
    for(uint8_t k=0;k<RANGE_OF_CHESSBOARD;k++){
        countBlack=countWhite=0;
        uint8_t i,j;
        for(i=k,j=0;i<RANGE_OF_CHESSBOARD&& j<RANGE_OF_CHESSBOARD;i++,j++){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
        for(i=0,j=k;i<RANGE_OF_CHESSBOARD&& j<RANGE_OF_CHESSBOARD;i++,j++){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }
     //斜向搜索
    for(uint8_t k=0;k<RANGE_OF_CHESSBOARD;k++){
        countBlack=countWhite=0;
        int8_t i,j;
        for(i=k,j=0;i>=0&&j<RANGE_OF_CHESSBOARD;i--,j++){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
        for(i=0,j=k;i<RANGE_OF_CHESSBOARD&& j>=0;i++,j--){
            if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(stateOfChessboard[i*RANGE_OF_CHESSBOARD+j]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }
    return CONTINUE;
}

/** 
 * @brief 继续游戏直到结束
*/
void continue_the_game(void){
    if(gameMode==PERSON_VS_PERSON){
        while (gameWinner==CONTINUE){
            input_chess_place();
            place_the_chess();
            draw_the_chessboard();
            gameWinner=call_the_game();
            if(gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(gameWinner==WHITE_WINE){
                printf("WINNER:WHITE\n");
                return;
            }
            if(playerFlag==BLACK_PLAYER){
                playerFlag=WHITE_PLAYER;
            }else if(playerFlag==WHITE_PLAYER){
                playerFlag=BLACK_PLAYER;
            }
        }
    }else if(gameMode==PERSON_VS_COMPUTER){
        while (gameWinner==CONTINUE){
            input_chess_place();
            place_the_chess();
            draw_the_chessboard();
            gameWinner=call_the_game();
            if(gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(gameWinner==WHITE_WINE){
                printf("WINNER:WHITE\n");
                return;
            }
            if(playerFlag==BLACK_PLAYER){
                playerFlag=WHITE_PLAYER;
            }else if(playerFlag==WHITE_PLAYER){
                playerFlag=BLACK_PLAYER;
            }
        }
    }else if(gameMode==COMPUTER_VS_PERSON){
        while (gameWinner==CONTINUE){
            input_chess_place();
            place_the_chess();
            draw_the_chessboard();
            gameWinner=call_the_game();
            if(gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(gameWinner==WHITE_WINE){
                printf("WINNER:WHITE\n");
                return;
            }
            if(playerFlag==BLACK_PLAYER){
                playerFlag=WHITE_PLAYER;
            }else if(playerFlag==WHITE_PLAYER){
                playerFlag=BLACK_PLAYER;
            }
        }
    }
}

/**
 * @brief 提供只读的棋盘接口
*/
const uint8_t * get_state_of_chessboard_point(void){
    return (const uint8_t *)&stateOfChessboard;
}

#endif