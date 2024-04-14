/*
 * player.h
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */

#ifndef INC_PLAYER_H_
#define INC_PLAYER_H_

#include "global.h"
#include "button.h"
#include "grondMap.h"
#include "letterBox.h"
#include "spiData.h"


struct playerData{
	uint16_t posX;
	uint16_t posY;
	uint8_t radius;
	uint8_t rotation; //45 degrees steps 0 == right 2 == down 4 == left 6 == up
	uint8_t directionX; //direction where its looking at in case no input;
	int8_t speed;	//10 default L/R MODifier
	int8_t velX;
	int8_t velY;
	int16_t power;
	int16_t angle;
	uint8_t hp;
	int16_t score;
	bool inAir;

};

void playerOff();
void playerInit();
int playerMovement(uint8_t current_player);

extern int currentPlayer;
extern struct playerData player[2];



#endif /* INC_PLAYER_H_ */
