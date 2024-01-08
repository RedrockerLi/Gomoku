#ifndef _TRAIN_H
#define _TRAIN_H
#include <stdint.h>

#include "main.h"
#include "AI.h"
#include "game.h"

typedef struct{
    ONE_GAME_t thisGame_t;
    ONE_AI_t blackPlayer_t;
    ONE_AI_t whitePlayer_t;
}ONE_AI_VS_AI_t;

uint8_t carry_out_one_game(ONE_AI_VS_AI_t *nowGame_t);
#endif