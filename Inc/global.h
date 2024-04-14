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
#define GROUND_OFF 0b00110100
#define GROUND_ON 0b00110101
#define WRITE_LETTER 0b01110001
#define SWITCH_SCREEN 0b11100011
#define BARREL1 0b01010101
#define BARREL2 0b01100110
#define PLAYSOUND 0b11000011

//defines for spi data transitions and sounds
#define NO_DATA 0b0000000000000000
#define GAMESCREEN 0b1011011000000000
#define MAINSCREEN 0b0000000001001001

#define SOUND_EXPLOSION 0b0000000000001111
#define SOUND_SHOOT 0b0000000011110000
#define SOUND_WIN 0b0000111100000000



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
#define INIT_MAIN 0
#define HOOFDMENU 1
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
#define HIGHSCORES 12
#define GAME_END_INIT 13
#define GAME_TEST 20

//defines for settings screen
#define SCHERMHOOGTE 480
#define SCHERMBREEDTE 640
#define VGABUFFERBREEDTE 0 // 144
#define VGABUFFERHOOGTE 0 // 31
#define TILESIZE 8
#define MAPSIZE 100
#define TILEWIDTH 80
#define TILEHEIGHT 30
#define TEXT_SIZE 16

//defines for game settings
#define GRAVITY 1
#define BARREL_STEPS 5

//defines for physical buttons
#define BUTTON_LEFT 0
#define BUTTON_RIGHT 1
#define BUTTON_DOWN 2
#define BUTTON_UP 3
#define BUTTON_SELECT 5
#define BUTTON_CONFIRM 4


#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f0xx_hal.h"

#include <stdio.h>


//empty char array
extern char emptyChar[TEXT_SIZE];

//main menu text chars
extern char start_game_text[TEXT_SIZE];
extern char highscore_main_text[TEXT_SIZE];
extern char press_confirm_text[TEXT_SIZE];
extern char press_select_text[TEXT_SIZE];
extern char game_name[TEXT_SIZE];

//win text for both players
extern char player1_wins_text[TEXT_SIZE];
extern char player2_wins_text[TEXT_SIZE];

//other text chars
extern char hit_text[TEXT_SIZE];
extern char highscore_text[TEXT_SIZE];

//credits text chars
extern char credits[TEXT_SIZE];
extern char frenk_text[TEXT_SIZE];
extern char luuk_text[TEXT_SIZE];
extern char tjebbe_text[TEXT_SIZE];

//player restrictions
extern int8_t maxVelY; //max fall speed of player


#endif /* INC_GLOBAL_H_ */
