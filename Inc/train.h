#ifndef _TRAIN_H
#define _TRAIN_H
#include <stdint.h>

#include "main.h"
#include "AI.h"
#include "game.h"

#define NUM_OF_PARTICLE 32

typedef struct{
    ONE_GAME_t thisGame_t;
    ONE_AI_t blackPlayer_t;
    ONE_AI_t whitePlayer_t;
}ONE_AI_VS_AI_t;

#ifndef _ONE_PARTICLE_t
#define _ONE_PARTICLE_t
typedef struct{
    int32_t bestScores[LENGTH_OF_STATES];
    int32_t nowScores[LENGTH_OF_STATES];
    int32_t nowSpeed[LENGTH_OF_STATES];
}ONE_PARTICLE_t;
#endif

typedef struct{
    uint8_t omega;
    uint8_t c_1;
    uint8_t c_2;
}PARAMETER_t;

void particles_init(ONE_PARTICLE_t *particles);
uint8_t carry_out_one_game(ONE_AI_VS_AI_t *nowGame_t);
void refresh_max(ONE_PARTICLE_t *particles,int32_t *bestScoresInAll);
void refresh_speed(ONE_PARTICLE_t *particles,const int32_t * const bestScoresInAll,const PARAMETER_t const *parameters);
void refresh_scores(ONE_PARTICLE_t *particles,const PARAMETER_t * const parameters);
void refresh_progress(const ONE_PARTICLE_t * const particles,const int32_t * const bestScoresInAll,uint8_t *numsInTheCircle);
#endif