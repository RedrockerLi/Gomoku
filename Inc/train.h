#include <stdint.h>

#include "main.h"
#include "AI.h"
#include "game.h"

typedef struct{
    ONE_GAME_t thisGame;
    ONE_AI_t blackPlayer;
    ONE_AI_t whitePlayer;
}