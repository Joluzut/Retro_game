/*
 * highscore.h
 *
 *  Created on: Apr 4, 2024
 *      Author: Tjebbe
 */

#ifndef INC_HIGHSCORE_H_
#define INC_HIGHSCORE_H_
#include <stdio.h>
#include "letterBox.h"
#include "global.h"
#include "button.h"

void compareScores(uint16_t new_score);
int getHighscore();
int changeScorePlayer(uint8_t hit_type,int16_t current_score);
int changeScoreOpponent(uint8_t hit_type,int16_t current_score);
int addTurnScore(uint16_t turn_n ,int16_t current_score);

void highscoreMenu();


extern char highscore[15];

#endif /* INC_HIGHSCORE_H_ */
