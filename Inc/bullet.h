/*
 * bullet.h
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */

#ifndef INC_BULLET_H_
#define INC_BULLET_H_

#include "global.h"
#include "player.h"
#include "grondMap.h"
#include "spiData.h"

struct bulletData{
	uint32_t posX;
	uint32_t posY;
	//45 degrees steps 0 == right 2 == down 4 == left 6 == up
	bool hit; //direction where its looking at in case no input;
	int16_t velX;
	int16_t velY;
	uint8_t radius;
	uint8_t power;
	uint8_t bombradius;
};


void bulletInit();
int bulletSchiet(int current_player);
int bulletHit(); //return 0 no hit// return 1 if player hit// return 2 if ground hit



extern struct bulletData bullet;






#endif /* INC_BULLET_H_ */
