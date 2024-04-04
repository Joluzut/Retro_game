/*
 * global.h
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

//Define for spi data adresses
#define PLAYER1_X 0b00000000
#define PLAYER1_Y 0b00000001
#define PLAYER2_X 0b00010000
#define PLAYER2_Y 0b00010001
#define BULLET_X 0b00100000
#define BULLET_Y 0b00100001
#define GROUND_OFF 0b00110000
#define GROUND_ON 0b00110001
#define WRITE_LETTER 0b01110001

//Defines for letterboxes
#define CONT_HP1 0b0000
#define CONT_HP2 0b0001
#define CONT_POWER 0b0010
#define CONT_ANGLE 0b0011
#define CONT_REGEL1 0b0100
#define CONT_REGEL2 0b0101
#define CONT_REGEL3 0b0110
#define CONT_REGEL4 0b0111
#define CONT_REGEL5 0b1000
#define CONT_REGEL6 0b1001

//Defines for State Machine
#define HOOFDMENU 0
#define HIGHSCOREMENU 1
#define GAME_INIT 2
#define GAME_MOVE 3
#define GAME_BULLET_INIT 4
#define GAME_BULLET_CALC 5
#define GAME_BULLET_INIT_CALC 6
#define GAME_IDLE 7
#define GAME_BULLET_SHOOT 8
#define GAME_HIT_DETECT 9
#define GAME_FINISHED 10
#define GAME_PLAYER_SWITCH 11

#define SCHERMHOOGTE 480
#define SCHERMBREEDTE 640
#define VGABUFFERBREEDTE 0 // 144
#define VGABUFFERHOOGTE 0 // 31
#define TILESIZE 8
#define MAPSIZE 100
#define TILEWIDTH 80
#define TILEHEIGHT 30

#define GRAVITY 1

#define BUTTON_LEFT 0
#define BUTTON_RIGHT 1
#define BUTTON_DOWN 2
#define BUTTON_UP 3
#define BUTTON_SELECT 4
#define BUTTON_CONFIRM 5






#include <stdbool.h>
#include <string.h>
#include "stm32f0xx_hal.h"




static int8_t maxVelX = 5;
static int8_t maxVelY = 10;





#endif /* INC_GLOBAL_H_ */
