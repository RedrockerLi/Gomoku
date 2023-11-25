#ifndef _GAME_H
#define _GAME_H

enum GAME_MODE{
    PERSON_VS_PERSON=0,
    PERSON_VS_COMPUTER,
    COMPUTER_VS_PERSON
};

void draw_the_start_page(void);
void get_game_mode(void);
#endif