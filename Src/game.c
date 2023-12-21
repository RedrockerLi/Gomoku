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

void input_chess_place(ONE_GAME_t *nowGame_t);

/**
 * @brief 初始化游戏
*/
void gameInit(ONE_GAME_t *nowGame_t){
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
    system("clear");
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
void input_game_mode(ONE_GAME_t *nowGame_t){
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
void draw_the_chessboard(ONE_GAME_t *nowGame_t){
    system("clear");
    uint8_t row,col;
    //第一行
    //第一列
    row=0,col=0;
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK: printf("%s",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s",LAST_BLACK_CHESS);break;
        case NONE:printf("%s",CHESSBOARD_CORNER_2); break;
        case WHITE:printf("%s",WHITE_CHESS); break;
        case LAST_WHITE: printf("%s",LAST_WHITE_CHESS); break;
    }
    //中间列
    for(col=1;col<RANGE_OF_CHESSBOARD-1;col++){
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s",LAST_BLACK_CHESS); break;
            case NONE:printf("%s",CHESSBOARD_UP); break;
            case WHITE:printf("%s",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s",LAST_WHITE_CHESS); break;
        }
    }
    //最后列
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK:printf("%s\n",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s\n",LAST_BLACK_CHESS); break;
        case NONE:printf("%s\n",CHESSBOARD_CORNER_1); break;
        case WHITE:printf("%s\n",WHITE_CHESS); break;
        case LAST_WHITE:printf("%s\n",LAST_WHITE_CHESS); break;
    }
    //中间行
    for(row=1;row<RANGE_OF_CHESSBOARD-1;row++){
        //第一列
        col=0;
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s",LAST_BLACK_CHESS); break;
            case NONE:printf("%s",CHESSBOARD_LEFT); break;
            case WHITE:printf("%s",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s",LAST_WHITE_CHESS); break;
        }
        //中间列
        for(col=1;col<RANGE_OF_CHESSBOARD-1;col++){
            switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
                case BLACK:printf("%s",BLACK_CHESS); break;
                case LAST_BLACK:printf("%s",LAST_BLACK_CHESS); break;
                case NONE:printf("%s",CHESSBOARD_CENTER); break;
                case WHITE:printf("%s",WHITE_CHESS); break;
                case LAST_WHITE:printf("%s",LAST_WHITE_CHESS); break;
            }
        }
        //最后列
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s\n",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s\n",LAST_BLACK_CHESS); break;
            case NONE:printf("%s\n",CHESSBOARD_RIGHT); break;
            case WHITE:printf("%s\n",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s\n",LAST_WHITE_CHESS); break;
        }
    }
    //最后行
    //第一列
    col=0;
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK:printf("%s",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s",LAST_BLACK_CHESS); break;
        case NONE:printf("%s",CHESSBOARD_CORNER_3); break;
        case WHITE:printf("%s",WHITE_CHESS); break;
        case LAST_WHITE:printf("%s",LAST_WHITE_CHESS); break;
    }
    //中间列
    for(col=1;col<RANGE_OF_CHESSBOARD-1;col++){
        switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
            case BLACK:printf("%s",BLACK_CHESS); break;
            case LAST_BLACK:printf("%s",LAST_BLACK_CHESS); break;
            case NONE:printf("%s",CHESSBOARD_DOWN); break;
            case WHITE:printf("%s",WHITE_CHESS); break;
            case LAST_WHITE:printf("%s",LAST_WHITE_CHESS); break;
        }   
    }
    //最后列
    switch (nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]){
        case BLACK:printf("%s\n",BLACK_CHESS); break;
        case LAST_BLACK:printf("%s\n",LAST_BLACK_CHESS); break;
        case NONE:printf("%s\n",CHESSBOARD_CORNER_4); break;
        case WHITE:printf("%s\n",WHITE_CHESS); break;
        case LAST_WHITE:printf("%s\n",LAST_WHITE_CHESS); break;
    }
}

/**
 * @brief 处理非法输入
*/
void deal_invalid_input(ONE_GAME_t *nowGame_t){
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
 * @brief myatoi(只看数字)
*/
int8_t myatoi(uint8_t s[]){
    int8_t ans=0;
    for(uint8_t i=0;i<strlen(s)&&s[i]>='0'&&s[i]<='9';i++){
        ans=ans*10+s[i]-'0';
    }
    return ans;
}

/**
 * @brief 落子输入,格式：row col（数字）
*/
void input_chess_place(ONE_GAME_t *nowGame_t){
    int8_t row,col;
    uint8_t s[10];
    if(nowGame_t->playerFlag==BLACK_PLAYER){
        if(nowGame_t->blackInputChessPlace.flag==INPUT_USED){
            if(nowGame_t->gameMode==PERSON_VS_PERSON||nowGame_t->gameMode==PERSON_VS_COMPUTER){
                printf("please input the next place of black:x y\n");
                scanf("%10s",s);
                if(strcmp(s,"quit")==0){
                    exit(0);
                }
                row=myatoi(s);
                col=-1;
                for(int8_t i=0;i<10&&((s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z'));i++){
                    if(s[i]>='A'&&s[i]<='Z'){
                        col=s[i]-'A';
                    }
                    if(s[i]>='a'&&s[i]<='z'){
                        col=s[i]-'a';
                    }
                }
                if(col==-1){
                    nowGame_t->blackInputChessPlace.flag=INVALID_FORM;
                    input_chess_place(nowGame_t);
                }
                int8_t clear;
                while((clear=getchar())!='\n'&&clear!=EOF);
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
            scanf("%10s",s);
            if(strcmp(s,"quit")==0){
                exit(0);
            }
            row=myatoi(s);
            col=-1;
            for(int8_t i=0;i<10&&((s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z'));i++){
                if(s[i]>='A'&&s[i]<='Z'){
                    col=s[i]-'A';
                }
                if(s[i]>='a'&&s[i]<='z'){
                    col=s[i]-'a';
                }
            }
            if(col==-1){
                nowGame_t->blackInputChessPlace.flag=INVALID_FORM;
                input_chess_place(nowGame_t);
            }
            int8_t clear;
            while((clear=getchar())!='\n'&&clear!=EOF);
            nowGame_t->blackInputChessPlace.row=row;
            nowGame_t->blackInputChessPlace.col=col;
            nowGame_t->blackInputChessPlace.flag=INPUT_UNUSED;
            deal_invalid_input(nowGame_t);
        }
    }else if(nowGame_t->playerFlag==WHITE_PLAYER){
        if(nowGame_t->whiteInputChessPlace.flag==INPUT_USED){
            if(nowGame_t->gameMode==PERSON_VS_PERSON||nowGame_t->gameMode==COMPUTER_VS_PERSON){
                printf("please input the next place of white:x y\n");
                scanf("%10s",s);
                if(strcmp(s,"quit")==0){
                    exit(0);
                }
                row=myatoi(s);
                col=-1;
                for(int8_t i=0;i<10&&((s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z'));i++){
                    if(s[i]>='A'&&s[i]<='Z'){
                        col=s[i]-'A';
                    }
                    if(s[i]>='a'&&s[i]<='z'){
                        col=s[i]-'a';
                    }
                }
                if(col==-1){
                    nowGame_t->blackInputChessPlace.flag=INVALID_FORM;
                    input_chess_place(nowGame_t);
                }
                int8_t clear;
                while((clear=getchar())!='\n'&&clear!=EOF);
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
            scanf("%10s",s);
            if(strcmp(s,"quit")==0){
                exit(0);
            }
            row=myatoi(s);
            col=-1;
            for(int8_t i=0;i<10&&((s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z'));i++){
                if(s[i]>='A'&&s[i]<='Z'){
                    col=s[i]-'A';
                }
                if(s[i]>='a'&&s[i]<='z'){
                    col=s[i]-'a';
                }
            }
            if(col==-1){
                nowGame_t->blackInputChessPlace.flag=INVALID_FORM;
                input_chess_place(nowGame_t);
            }
            int8_t clear;
            while((clear=getchar())!='\n'&&clear!=EOF);
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
void place_the_chess(ONE_GAME_t *nowGame_t){
    if(nowGame_t->playerFlag==BLACK_PLAYER&&nowGame_t->blackInputChessPlace.flag==INPUT_UNUSED){
        if(nowGame_t->lastBlackInputChessPlace.flag==INPUT_UNUSED){
            nowGame_t->stateOfChessboard[nowGame_t->lastBlackInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->lastBlackInputChessPlace.col]=BLACK;
            nowGame_t->lastBlackInputChessPlace.flag=INPUT_USED;
        }
        nowGame_t->stateOfChessboard[nowGame_t->blackInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->blackInputChessPlace.col]=LAST_BLACK;
        nowGame_t->lastBlackInputChessPlace.row=nowGame_t->blackInputChessPlace.row;
        nowGame_t->lastBlackInputChessPlace.col=nowGame_t->blackInputChessPlace.col;
        nowGame_t->lastBlackInputChessPlace.flag=INPUT_UNUSED;
        nowGame_t->blackInputChessPlace.flag=INPUT_USED;
    }else if(nowGame_t->playerFlag==WHITE_PLAYER&&nowGame_t->whiteInputChessPlace.flag==INPUT_UNUSED){
        if(nowGame_t->lastWhiteInputChessPlace.flag==INPUT_UNUSED){
            nowGame_t->stateOfChessboard[nowGame_t->lastWhiteInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->lastWhiteInputChessPlace.col]=WHITE;
            nowGame_t->lastWhiteInputChessPlace.flag=INPUT_USED;
        }
        nowGame_t->stateOfChessboard[nowGame_t->whiteInputChessPlace.row*RANGE_OF_CHESSBOARD+nowGame_t->whiteInputChessPlace.col]=LAST_WHITE;
        nowGame_t->lastWhiteInputChessPlace.row=nowGame_t->whiteInputChessPlace.row;
        nowGame_t->lastWhiteInputChessPlace.col=nowGame_t->whiteInputChessPlace.col;
        nowGame_t->lastWhiteInputChessPlace.flag=INPUT_UNUSED;
        nowGame_t->whiteInputChessPlace.flag=INPUT_USED;
    }
}

/**
 * @brief 判断胜负
*/
uint8_t call_the_game(ONE_GAME_t *nowGame_t){
    uint8_t countBlack,countWhite;
    //按行搜索
    for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
        countBlack=countWhite=0;
        for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
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
    //按列搜索
    for(uint8_t col=0;col<RANGE_OF_CHESSBOARD;col++){
        countBlack=countWhite=0;
        for(uint8_t row=0;row<RANGE_OF_CHESSBOARD;row++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
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
    for(uint8_t k=0;k<RANGE_OF_CHESSBOARD;k++){
        countBlack=countWhite=0;
        uint8_t row,col;
        for(row=k,col=0;row<RANGE_OF_CHESSBOARD&& col<RANGE_OF_CHESSBOARD;row++,col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
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
        for(row=0,col=k;row<RANGE_OF_CHESSBOARD&& col<RANGE_OF_CHESSBOARD;row++,col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
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
    for(uint8_t k=0;k<RANGE_OF_CHESSBOARD;k++){
        countBlack=countWhite=0;
        int8_t row,col;
        for(row=k,col=0;row>=0&&col<RANGE_OF_CHESSBOARD;row--,col++){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
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
        for(row=0,col=k;row<RANGE_OF_CHESSBOARD&& col>=0;row++,col--){
            if(nowGame_t->stateOfChessboard[row*RANGE_OF_CHESSBOARD+col]==BLACK){
                countBlack++;
                countWhite=0;
                if(countBlack==5){
                    return BLACK_WINE;
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
 * @brief 判断禁手
*/
uint8_t judge_the_forbidden_hand(ONE_GAME_t *nowGame_t){
    return 0;
}

/** 
 * @brief 继续游戏直到结束
*/
void continue_the_game(ONE_GAME_t *nowGame_t){
    if(nowGame_t->gameMode==PERSON_VS_PERSON){
        while (nowGame_t->gameWinner==CONTINUE){
            input_chess_place(nowGame_t);
            place_the_chess(nowGame_t);
            draw_the_chessboard(nowGame_t);
            nowGame_t->gameWinner=call_the_game(nowGame_t);
            if(nowGame_t->gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(nowGame_t->gameWinner==WHITE_WINE){
                printf("WINNER:WHITE\n");
                return;
            }
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->playerFlag=WHITE_PLAYER;
            }else if(nowGame_t->playerFlag==WHITE_PLAYER){
                nowGame_t->playerFlag=BLACK_PLAYER;
            }
        }
    }else if(nowGame_t->gameMode==PERSON_VS_COMPUTER){
        while (nowGame_t->gameWinner==CONTINUE){
            input_chess_place(nowGame_t);
            place_the_chess(nowGame_t);
            draw_the_chessboard(nowGame_t);
            nowGame_t->gameWinner=call_the_game(nowGame_t);
            if(nowGame_t->gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(nowGame_t->gameWinner==WHITE_WINE){
                printf("WINNER:WHITE\n");
                return;
            }
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->playerFlag=WHITE_PLAYER;
            }else if(nowGame_t->playerFlag==WHITE_PLAYER){
                nowGame_t->playerFlag=BLACK_PLAYER;
            }
        }
    }else if(nowGame_t->gameMode==COMPUTER_VS_PERSON){
        while (nowGame_t->gameWinner==CONTINUE){
            input_chess_place(nowGame_t);
            place_the_chess(nowGame_t);
            draw_the_chessboard(nowGame_t);
            nowGame_t->gameWinner=call_the_game(nowGame_t);
            if(nowGame_t->gameWinner==BLACK_WINE){
                printf("WINNER:BLACK\n");
                return;
            }else if(nowGame_t->gameWinner==WHITE_WINE){
                printf("WINNER:WHITE\n");
                return;
            }
            if(nowGame_t->playerFlag==BLACK_PLAYER){
                nowGame_t->playerFlag=WHITE_PLAYER;
            }else if(nowGame_t->playerFlag==WHITE_PLAYER){
                nowGame_t->playerFlag=BLACK_PLAYER;
            }
        }
    }
}

/**
 * @brief 提供只读的棋盘接口
*/
// const uint8_t * get_state_of_chessboard_point(ONE_GAME_t *nowGame_t){
//     return (const uint8_t *)&nowGame_t->stateOfChessboard;
// }