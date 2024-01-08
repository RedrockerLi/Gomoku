#include <stdint.h>

#include "main.h"
#include "AI.h"
#include "game.h"
#include  "train.h"

/**
 * @brief 自动进行一场比赛
*/
uint8_t carry_out_one_game(ONE_AI_VS_AI_t *nowGame_t){
    game_init(&nowGame_t->thisGame_t);
    uint8_t nums=0;
    while (nowGame_t->thisGame_t.gameWinner==CONTINUE){
        if(nums==225){
            return CONTINUE;
        }
        if(nowGame_t->thisGame_t.playerFlag==BLACK_PLAYER){
            calc_next_input(&nowGame_t->thisGame_t,&nowGame_t->blackPlayer_t);
        }else{
            calc_next_input(&nowGame_t->thisGame_t,&nowGame_t->whitePlayer_t);
        }
        place_the_chess(&nowGame_t->thisGame_t);
        nowGame_t->thisGame_t.gameWinner=call_the_game(&nowGame_t->thisGame_t,nowGame_t->thisGame_t.lastBlackInputChessPlace.row,nowGame_t->thisGame_t.lastBlackInputChessPlace.col,1);
        if(nowGame_t->thisGame_t.gameWinner==CONTINUE){
            if(nowGame_t->thisGame_t.playerFlag==BLACK_PLAYER){
                    if(judge_forbidden_hand(&nowGame_t->thisGame_t,nowGame_t->thisGame_t.lastBlackInputChessPlace.row,nowGame_t->thisGame_t.blackInputChessPlace.col,2)==FORBIDDEN_HAND){
                    nowGame_t->thisGame_t.gameWinner=WHITE_WINE;
                    output_log("trainLog","Err:FORBIDDEN_HAND");
                }
            }
        }else if(nowGame_t->thisGame_t.gameWinner==FORBIDDEN_HAND){
            output_log("trainLog","Err:FORBIDDEN_HAND");
        }
        if(nowGame_t->thisGame_t.gameWinner!=CONTINUE){
            return nowGame_t->thisGame_t.gameWinner;
        }
        if(nowGame_t->thisGame_t.playerFlag==BLACK_PLAYER){
            nowGame_t->thisGame_t.playerFlag=WHITE_PLAYER;
        }else{
            nowGame_t->thisGame_t.playerFlag=BLACK_PLAYER;
        }
        nums++;
    }
}