/**
  ******************************************************************************
  * @file           : player.h
  * @brief          : Header for player.c file.
  *                   This file contains the public variables and functions of the playable character.
  ******************************************************************************
  */

#ifndef INC_PLAYER_H_
#define INC_PLAYER_H_

#include "global.h"
#include "button.h"
#include "grondMap.h"
#include "letterBox.h"
#include "spiData.h"

/**
  * @brief Struct of player data
  * @param uint16_t	location x-axis
  * @param uint16_t	location x-axis
  * @param uint8_t	radius of player
  * @param uint8_t	position of tank barrel
  * @param uint8_t	direction tank is moving
  * @param int8_t	speed of tank
  * @param int8_t	velocity of tank X-axis
  * @param int8_t	velocity of tank Y-axis
  * @param int16_t	shot power
  * @param int16_t	shot able
  * @param uint8_t	health points of player
  * @param int16_t	score of player
  * @param bool		player in air = 1
  */
struct playerData{
	uint16_t posX;
	uint16_t posY;
	uint8_t radius;
	uint8_t rotation;
	uint8_t directionX; //direction where its looking at in case no input;
	int8_t speed;	//1 default L/R MODifier
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
