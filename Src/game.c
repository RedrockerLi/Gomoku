#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h> 

#include "game.h"
#include "AI.h"

/********************************绘制棋盘宏定义****************************************/
#define CHESSBOARD_CORNER_1 "┓"
#define CHESSBOARD_CORNER_2 "┏"
#define CHESSBOARD_CORNER_3 "┗"
#define CHESSBOARD_CORNER_4 "┛"
#define CHESSBOARD_LEFT "┠"
#define CHESSBOARD_RIGHT "┨"
#define CHESSBOARD_UP "┯"
#define CHESSBOARD_DOWN "┷"
#define CHESSBOARD_CENTER "┼"
#define BLACK_CHESS "●"
#define LAST_BLACK_CHESS "▲"
#define WHITE_CHESS "◎"
#define LAST_WHITE_CHESS "△"
/***********************************************************************************/

// #define CLEAR //打开则有清屏功能
#define ONLY_BLACKPLEAR //打开则只有黑棋玩家，用于实验禁手

const int8_t direction[8]={0,1, 1,0, 1,1, 1,-1};//两个数一组描述4个方向，调用方式2n&2n+1(row&col)

void input_chess_place(ONE_GAME_t * const nowGame_t);

/**
 * @brief 初始化游戏
*/
void gameInit(ONE_GAME_t * const nowGame_t){
    //棋盘初始化
    for(int8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
        for(int8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
            nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=NONE;
        }
    }
    //先手初始化
    nowGame_t->playerFlag=BLACK_PLAYER;
    //落子接口初始化
    nowGame_t->blackInputChessPlace.flag=INPUT_USED;
    nowGame_t->whiteInputChessPlace.flag=INPUT_USED;
    //比赛 结果初始化
    nowGame_t->gameWinner=CONTINUE;
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
    #ifdef CLEAR
    system("clear");
    #endif
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
void input_game_mode(ONE_GAME_t * const nowGame_t){
    uint8_t i;
    scanf("%c",&i);
    int8_t clear;
    while((clear=getchar())!='\n'&&clear!=EOF);
    switch(i)
    {
    case 'A':
        nowGame_t->gameMode=PERSON_VS_PERSON;
        break;
    case 'B':
        nowGame_t->gameMode=PERSON_VS_COMPUTER;
        break;
    case 'C':
        nowGame_t->gameMode=COMPUTER_VS_PERSON;
        break;
    default:
        printf("Please choose the right game mode:\n");
        input_game_mode(nowGame_t);
        break;
    }
}

/**
 * @brief 绘制棋盘
*/
void draw_the_chessboard(ONE_GAME_t * const nowGame_t){
    #ifdef CLEAR
    system("clear");
    #endif
    uint8_t row,col;
    //第一行
    //第一列
    row=0,col=0;
    printf("%2d",RANGE_OF_CHESSBOARD-row);
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK: printf("%s",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK; break;
        case NONE:printf("%s",CHESSBOARD_CORNER_2); break;
        case WHITE:printf("%s",WHITE_CHESS); break;
        case LAST_WHITE: printf("%s",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
    }
    //中间列
    for(col=1;col<RANGE_OF_CHESSBOARD-1;col++){
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK; break;
            case NONE:printf("%s",CHESSBOARD_UP); break;
            case WHITE:printf("%s",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
        }
    }
    //最后列
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK:printf("%s\n",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s\n",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK; break;
        case NONE:printf("%s\n",CHESSBOARD_CORNER_1); break;
        case WHITE:printf("%s\n",WHITE_CHESS); break;
        case LAST_WHITE:printf("%s\n",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
    }
    //中间行
    for(row=1;row<RANGE_OF_CHESSBOARD-1;row++){
        //第一列
        col=0;
        printf("%2d",RANGE_OF_CHESSBOARD-row);
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK; break;
            case NONE:printf("%s",CHESSBOARD_LEFT); break;
            case WHITE:printf("%s",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
        }
        //中间列
        for(col=1;col<RANGE_OF_CHESSBOARD-1;col++){
            switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
                case BLACK:printf("%s",BLACK_CHESS); break;
                case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK;break;
                case NONE:printf("%s",CHESSBOARD_CENTER); break;
                case WHITE:printf("%s",WHITE_CHESS); break;
                case LAST_WHITE:printf("%s",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
            }
        }
        //最后列
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s\n",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s\n",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK;break;
            case NONE:printf("%s\n",CHESSBOARD_RIGHT); break;
            case WHITE:printf("%s\n",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s\n",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
        }
    }
    //最后行
    //第一列
    col=0;
    printf("%2d",RANGE_OF_CHESSBOARD-row);
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK:printf("%s",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK;break;
        case NONE:printf("%s",CHESSBOARD_CORNER_3); break;
        case WHITE:printf("%s",WHITE_CHESS); break;
        case LAST_WHITE:printf("%s",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
    }
    //中间列
    for(col=1;col<RANGE_OF_CHESSBOARD-1;col++){
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK;break;
            case NONE:printf("%s",CHESSBOARD_DOWN); break;
            case WHITE:printf("%s",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
        }   
    }
    //最后列
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK:printf("%s\n",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s\n",LAST_BLACK_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=BLACK;break;
        case NONE:printf("%s\n",CHESSBOARD_CORNER_4); break;
        case WHITE:printf("%s\n",WHITE_CHESS); break;
        case LAST_WHITE:printf("%s\n",LAST_WHITE_CHESS);nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=WHITE; break;
    }
    uint8_t i;
    printf(" ");
    for(i=0;i<RANGE_OF_CHESSBOARD-1;i++){
        printf("%2c",'A'+i);
    }
    printf("%2c\n",'A'+i);
}

/**
 * @brief 处理非法输入
*/
void deal_invalid_input(ONE_GAME_t * const nowGame_t){
    if(nowGame_t->playerFlag==BLACK_PLAYER){
        if(nowGame_t->blackInputChessPlace.row>=RANGE_OF_CHESSBOARD||nowGame_t->blackInputChessPlace.col>=RANGE_OF_CHESSBOARD){
            nowGame_t->blackInputChessPlace.flag=INDEX_OUT_OF_BOUNDS;
            input_chess_place(nowGame_t);
        }
        if(nowGame_t->stateOfChessboard[nowGame_t->blackInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->blackInputChessPlace.col]!=NONE){
            nowGame_t->blackInputChessPlace.flag=OCCUPYED;
            input_chess_place(nowGame_t);
        }
    }else if(nowGame_t->playerFlag==WHITE_PLAYER){
        if(nowGame_t->whiteInputChessPlace.row>=RANGE_OF_CHESSBOARD||nowGame_t->whiteInputChessPlace.col>=RANGE_OF_CHESSBOARD){
            nowGame_t->whiteInputChessPlace.flag=INDEX_OUT_OF_BOUNDS;
            input_chess_place(nowGame_t);
        }
        if(nowGame_t->stateOfChessboard[nowGame_t->whiteInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->whiteInputChessPlace.col]!=NONE){
            nowGame_t->whiteInputChessPlace.flag=OCCUPYED;
            input_chess_place(nowGame_t);
        }
    }
}

/**
 * @brief 输入并解析字符串
*/
void deal_input(ONE_GAME_t * const nowGame_t,int8_t *row,int8_t *col){
    uint8_t s[10];
    scanf("%10s",s);
    int8_t clear;
    while((clear=getchar())!='\n'&&clear!=EOF);
    if(strcmp(s,"quit")==0){
        exit(0);
    }
    *col=-1;
    for(int8_t i=0;i<10;i++){
        if(s[i]>='A'&&s[i]<='Z'){
            *col=s[i]-'A';
            break;
        }
        if(s[i]>='a'&&s[i]<='z'){
            *col=s[i]-'a';
            break;
        }
    }
    *row=0;
    for(int8_t i=0;i<10;i++){
        if(s[i]>='0'&&s[i]<='9'){
            *row=(*row)*10+s[i]-'0';
        }
    }
    if(*col==-1||*row==0){
        nowGame_t->blackInputChessPlace.flag=INVALID_FORM;
        input_chess_place(nowGame_t);
    }
    *row=RANGE_OF_CHESSBOARD-*row;
}

/**
 * @brief 落子输入
*/
void input_chess_place(ONE_GAME_t * const nowGame_t){
    int8_t row,col;
    if(nowGame_t->playerFlag==BLACK_PLAYER){
        if(nowGame_t->blackInputChessPlace.flag==INPUT_USED){
            if(nowGame_t->gameMode==PERSON_VS_PERSON||nowGame_t->gameMode==PERSON_VS_COMPUTER){
                printf("please input the next place of black:x y\n");
                deal_input(nowGame_t,&row,&col);
                nowGame_t->blackInputChessPlace.row=row;
                nowGame_t->blackInputChessPlace.col=col;
                nowGame_t->blackInputChessPlace.flag=INPUT_UNUSED;
                deal_invalid_input(nowGame_t);
            }
        }else if(nowGame_t->blackInputChessPlace.flag<INPUT_UNUSED){
            switch(nowGame_t->blackInputChessPlace.flag){
                case OCCUPYED:
                    printf("Too crowded! Please choose a right one.\n");
                    break;
                case INDEX_OUT_OF_BOUNDS:
                    printf("The chessboard can't be bigger. Please choose a right one.\n");
                    break;
                case INVALID_FORM:
                    printf("Please input like \"a1\"or\"1a\"or\"A1\"or\"1A\"\n");
                    break;
            }
            deal_input(nowGame_t,&row,&col);
            nowGame_t->blackInputChessPlace.row=row;
            nowGame_t->blackInputChessPlace.col=col;
            nowGame_t->blackInputChessPlace.flag=INPUT_UNUSED;
            deal_invalid_input(nowGame_t);
        }
    }else if(nowGame_t->playerFlag==WHITE_PLAYER){
        if(nowGame_t->whiteInputChessPlace.flag==INPUT_USED){
            if(nowGame_t->gameMode==PERSON_VS_PERSON||nowGame_t->gameMode==COMPUTER_VS_PERSON){
                printf("please input the next place of white:x y\n");
                deal_input(nowGame_t,&row,&col);
                nowGame_t->whiteInputChessPlace.row=row;
                nowGame_t->whiteInputChessPlace.col=col;
                nowGame_t->whiteInputChessPlace.flag=INPUT_UNUSED;
                deal_invalid_input(nowGame_t);
            }
        }else if(nowGame_t->whiteInputChessPlace.flag<INPUT_UNUSED){
            switch(nowGame_t->whiteInputChessPlace.flag){
                case OCCUPYED:
                    printf("Too crowded! Please choose a right one.\n");
                    break;
                case INDEX_OUT_OF_BOUNDS:
                    printf("The chessboard can't be bigger. Please choose a right one.\n");
                    break;
                case INVALID_FORM:
                    printf("Please input like \"a1\"or\"1a\"or\"A1\"or\"1A\"\n");
                    break;
            }
            deal_input(nowGame_t,&row,&col);
            nowGame_t->whiteInputChessPlace.row=row;
            nowGame_t->whiteInputChessPlace.col=col;
            nowGame_t->whiteInputChessPlace.flag=INPUT_UNUSED;
            deal_invalid_input(nowGame_t);
        }
    }
}

/**
 * @brief 棋盘落子
*/
void place_the_chess(ONE_GAME_t * const nowGame_t){
    if(nowGame_t->playerFlag==BLACK_PLAYER&&nowGame_t->blackInputChessPlace.flag==INPUT_UNUSED){
        // if(nowGame_t->lastBlackInputChessPlace.flag==INPUT_UNUSED){
        //     nowGame_t->stateOfChessboard[nowGame_t->lastBlackInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->lastBlackInputChessPlace.col]=BLACK;
        //     nowGame_t->lastBlackInputChessPlace.flag=INPUT_USED;
        // }
        nowGame_t->stateOfChessboard[nowGame_t->blackInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->blackInputChessPlace.col]=LAST_BLACK;
        nowGame_t->lastBlackInputChessPlace.row=nowGame_t->blackInputChessPlace.row;
        nowGame_t->lastBlackInputChessPlace.col=nowGame_t->blackInputChessPlace.col;
        // nowGame_t->lastBlackInputChessPlace.flag=INPUT_UNUSED;
        nowGame_t->blackInputChessPlace.flag=INPUT_USED;
    }else if(nowGame_t->playerFlag==WHITE_PLAYER&&nowGame_t->whiteInputChessPlace.flag==INPUT_UNUSED){
        // if(nowGame_t->lastWhiteInputChessPlace.flag==INPUT_UNUSED){
        //     nowGame_t->stateOfChessboard[nowGame_t->lastWhiteInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->lastWhiteInputChessPlace.col]=WHITE;
        //     nowGame_t->lastWhiteInputChessPlace.flag=INPUT_USED;
        // }
        nowGame_t->stateOfChessboard[nowGame_t->whiteInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->whiteInputChessPlace.col]=LAST_WHITE;
        // nowGame_t->lastWhiteInputChessPlace.row=nowGame_t->whiteInputChessPlace.row;
        // nowGame_t->lastWhiteInputChessPlace.col=nowGame_t->whiteInputChessPlace.col;
        // nowGame_t->lastWhiteInputChessPlace.flag=INPUT_UNUSED;
        nowGame_t->whiteInputChessPlace.flag=INPUT_USED;
    }
}

/**
 * @brief 判断胜负and长连禁手
*/
uint8_t call_the_game(ONE_GAME_t * const nowGame_t,uint8_t lastRow,uint8_t lastCol){
    uint8_t countBlack,countWhite,row,col;
    //按行搜索
    row=lastRow;
    countBlack=countWhite=0;
    for(col=0;col<RANGE_OF_CHESSBOARD;col++){
        if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
            countBlack++;
            countWhite=0;
            if(countBlack==5){
                if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col+1]==BLACK){
                    printf("Long-Ladder Suicide\n");
                    return FORBIDDEN_HAND;
                }else{
                    return BLACK_WINE;
                }
            }
        }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==WHITE){
            countBlack=0;
            countWhite++;
            if(countWhite==5){
                return WHITE_WINE;
            }
        }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
            countBlack=0;
            countWhite=0;
        }
    }
    //按列搜索
    col=lastCol;
    countBlack=countWhite=0;
    for(row=0;row<RANGE_OF_CHESSBOARD;row++){
        if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
            countBlack++;
            countWhite=0;
            if(countBlack==5){
                if(nowGame_t->stateOfChessboard[(row+1)*RANGE_OF_CHESSBOARD+col]==BLACK){
                    printf("Long-Ladder Suicide\n");
                    return FORBIDDEN_HAND;
                }else{
                    return BLACK_WINE;
                }
            }
        }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==WHITE){
            countBlack=0;
            countWhite++;
            if(countWhite==5){
                return WHITE_WINE;
            }
        }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
            countBlack=0;
            countWhite=0;
        }
    }
    //斜向搜索
    if(lastRow>=lastCol){
        for(row=lastRow-lastCol,col=0;row<RANGE_OF_CHESSBOARD&& col<RANGE_OF_CHESSBOARD;row++,col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    if(nowGame_t->stateOfChessboard[(row+1)*RANGE_OF_CHESSBOARD+(col+1)]==BLACK){
                        printf("Long-Ladder Suicide\n");
                        return FORBIDDEN_HAND;
                    }else{
                        return BLACK_WINE;
                    }
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }else{
        for(row=0,col=lastCol-lastRow;row<RANGE_OF_CHESSBOARD&& col<RANGE_OF_CHESSBOARD;row++,col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    if(nowGame_t->stateOfChessboard[(row+1)*RANGE_OF_CHESSBOARD+(col+1)]==BLACK){
                        printf("Long-Ladder Suicide\n");
                        return FORBIDDEN_HAND;
                    }else{
                        return BLACK_WINE;
                    }
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }
     //斜向搜索
    if(lastRow+lastCol>=RANGE_OF_CHESSBOARD-1){
        for(row=RANGE_OF_CHESSBOARD-1,col=lastRow+lastCol-(RANGE_OF_CHESSBOARD-1);row<RANGE_OF_CHESSBOARD&&col<RANGE_OF_CHESSBOARD;row--,col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    if(nowGame_t->stateOfChessboard[(row-1)*RANGE_OF_CHESSBOARD+(col+1)]==BLACK){
                        printf("Long-Ladder Suicide\n");
                        return FORBIDDEN_HAND;
                    }else{
                        return BLACK_WINE;
                    }
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }
    }else{
        for(row=0,col=lastRow+lastCol;row<RANGE_OF_CHESSBOARD&& col<RANGE_OF_CHESSBOARD;row++,col--){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    if(nowGame_t->stateOfChessboard[(row+1)*RANGE_OF_CHESSBOARD+(col-1)]==BLACK){
                        printf("Long-Ladder Suicide\n");
                        return FORBIDDEN_HAND;
                    }else{
                        return BLACK_WINE;
                    }
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==WHITE){
                countBlack=0;
                countWhite++;
                if(countWhite==5){
                    return WHITE_WINE;
                }
            }else if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
                countBlack=0;
                countWhite=0;
            }
        }   
    }
    return CONTINUE;
}

/**
 * @brief 清理棋盘中的TEMP_BLACK
*/
void clean_temp_black(ONE_GAME_t * const nowGame_t){
    for(int row=0;row<RANGE_OF_CHESSBOARD;row++){
        for(int col=0;col<RANGE_OF_CHESSBOARD;col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==TEMP_BLACK){
                nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=NONE;
            }
        }
    }
}

/**
 * @brief 判断棋子状态
 * @param direction 范围{0-3}表示4个方向
 * @param stdChess 下在（row,col）的棋子类型
*/
uint8_t judge_state_of_chess(ONE_GAME_t * const nowGame_t,const uint8_t row, const uint8_t col,const uint8_t stdChess,const uint8_t directionChoice){
    uint32_t lineToJudge=0; //两位一组，00为不可落子，11为空，01为和stdchess相同,相同方向没有特殊要求
    uint8_t count=0;
    uint8_t stdPlace=0;//待判断的棋子在第几位.
    for(int8_t i=-(row<col?row:col);row+i*direction[2*directionChoice]<RANGE_OF_CHESSBOARD&&col+i*direction[2*directionChoice]<RANGE_OF_CHESSBOARD;i++,count++){
        if(nowGame_t->stateOfChessboard[(row+i*direction[2*directionChoice])*RANGE_OF_CHESSBOARD+(col+i*direction[2*directionChoice])]==stdChess){
            lineToJudge=lineToJudge+1<<(count*2);
        }else if(nowGame_t->stateOfChessboard[(row+i*direction[2*directionChoice])*RANGE_OF_CHESSBOARD+(col+i*direction[2*directionChoice])]==NONE){
            if(judge_forbidden_hand(nowGame_t,row+i*direction[2*directionChoice],col+i*direction[2*directionChoice])!=FORBIDDEN_HAND){
                lineToJudge=lineToJudge+3<<(count*2);
            }
        }
        if(i==0){
            stdPlace=count;
        }
    }  
    uint8_t stateOfChess=0;
    uint8_t fourCount=0;
    for(uint32_t window=(0xFFF)<<(stdPlace*2),count=0;count<6&&count<stdPlace;count++,window=window>>2){
        if((lineToJudge&window)==0b110101010111){
            stateOfChess=LIVE_FOUR;
            fourCount++;
            if(fourCount==2){
                return FORBIDDEN_HAND;
            }
        }else if((lineToJudge&window)==0b000101010111||(lineToJudge&window)==0b110101010100||(lineToJudge&window)==0b011101010100||(lineToJudge&window)==0b011101010111){
            stateOfChess=FIGHT_FOUR;
            fourCount++;
            if(fourCount==2){
                return FORBIDDEN_HAND;
            }
        }else if(count!=0&&((lineToJudge&window)>>2==0b0101011101||(lineToJudge&window)>>2==0b0101110101)){
            stateOfChess=FIGHT_FOUR;
            fourCount++;
            if(fourCount==2){
                return FORBIDDEN_HAND;
            }
        }else if((lineToJudge&window)==0b110101011111||(lineToJudge&window)==0b110101110111||(lineToJudge&window)==0b110111010111||(lineToJudge&window)==0b111101010111){
            stateOfChess=LIVE_THREE;
        }
    }
    return stateOfChess;
}

/**
 * @brief 判断禁手
*/
uint8_t judge_forbidden_hand(ONE_GAME_t * const nowGame_t,uint8_t row,uint8_t col){
    if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==NONE){
        nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]=TEMP_BLACK;
    }
    uint8_t countBlack,countWhite;
    
    return 0;
}

/** 
 * @brief 继续游戏直到结束
*/
void continue_the_game(ONE_GAME_t * const nowGame_t){
    if(nowGame_t->gameMode==PERSON_VS_PERSON){
        while (nowGame_t->gameWinner==CONTINUE){
            input_chess_place(nowGame_t);
            place_the_chess(nowGame_t);
            draw_the_chessboard(nowGame_t);
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->gameWinner=call_the_game(nowGame_t,nowGame_t->lastBlackInputChessPlace.row,nowGame_t->lastBlackInputChessPlace.col);
            }else{
                nowGame_t->gameWinner=call_the_game(nowGame_t,nowGame_t->lastWhiteInputChessPlace.row,nowGame_t->lastWhiteInputChessPlace.col);
            }
            if(nowGame_t->gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(nowGame_t->gameWinner==WHITE_WINE||nowGame_t->gameWinner==FORBIDDEN_HAND){
                printf("WINNER:WHITE\n");
                return;
            }
            #ifdef ONLY_BLACK
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->playerFlag=WHITE_PLAYER;
            }else{
                nowGame_t->playerFlag=BLACK_PLAYER;
            }
            #endif
        }
    }else if(nowGame_t->gameMode==PERSON_VS_COMPUTER){
        while (nowGame_t->gameWinner==CONTINUE){
            input_chess_place(nowGame_t);
            place_the_chess(nowGame_t);
            draw_the_chessboard(nowGame_t);
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->gameWinner=call_the_game(nowGame_t,nowGame_t->lastBlackInputChessPlace.row,nowGame_t->lastBlackInputChessPlace.col);
            }else{
                nowGame_t->gameWinner=call_the_game(nowGame_t,nowGame_t->lastWhiteInputChessPlace.row,nowGame_t->lastWhiteInputChessPlace.col);
            }
            if(nowGame_t->gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else{
                printf("WINNER:WHITE\n");
                return;
            }
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->playerFlag=WHITE_PLAYER;
            }else{
                nowGame_t->playerFlag=BLACK_PLAYER;
            }
        }
    }else if(nowGame_t->gameMode==COMPUTER_VS_PERSON){
        while (nowGame_t->gameWinner==CONTINUE){
            input_chess_place(nowGame_t);
            place_the_chess(nowGame_t);
            draw_the_chessboard(nowGame_t);
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->gameWinner=call_the_game(nowGame_t,nowGame_t->lastBlackInputChessPlace.row,nowGame_t->lastBlackInputChessPlace.col);
            }else{
                nowGame_t->gameWinner=call_the_game(nowGame_t,nowGame_t->lastWhiteInputChessPlace.row,nowGame_t->lastWhiteInputChessPlace.col);
            }
            if(nowGame_t->gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else{
                printf("WINNER:WHITE\n");
                return;
            }
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->playerFlag=WHITE_PLAYER;
            }else{
                nowGame_t->playerFlag=BLACK_PLAYER;
            }
        }
    }
}

/**
 * @brief 提供只读的棋盘接口
*/
// const uint8_t * get_state_of_chessboard_point(ONE_GAME_t * const nowGame_t){
//     return (const uint8_t *)&nowGame_t->stateOfChessboard;
// }