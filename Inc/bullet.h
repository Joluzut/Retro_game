/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : bullet.h
  * @brief          : Header for bullet.c file.
  *                   This file contains the variables and defines of the bullet. 
  ******************************************************************************
  */

#ifndef INC_BULLET_H_
#define INC_BULLET_H_

#include "global.h"
#include "player.h"
#include "grondMap.h"
#include "spiData.h"
#include "letterbox.h"
#include "highscore.h"

#define MAX_ANGLE 90	//max angle to the right
#define MIN_ANGLE -90	//max angle to the left
#define MAX_POWER 80	// max power of bullet
#define MAX_FALLGRAVITY -3000 //max velocity of the bullet Y-axis
#define STEP_SIZE 18 	//90deg/5 = 18. 5 sprites per 90 degrees 

struct bulletData{
	int32_t posX; 
	int32_t posY;
	bool hit; //not used
	int16_t velX;
	int16_t velY;
	uint8_t radius;
	uint8_t power;
	uint8_t bombradius; //not used
};


void bulletInit();
int bulletLoop(uint8_t current_player);
int bulletHitDetect(); 


extern struct bulletData bullet;






#endif /* INC_BULLET_H_ */
