/**
  ******************************************************************************
  * @file           : global.h
  * @brief          : Header for global.c file.
  *                   This file contains global defines and variables. 
  ******************************************************************************
  */


#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

//Define for spi data adresses
#define PLAYER1_X 0b00000000    //Address for player 1 X-axis
#define PLAYER1_Y 0b00000001    //Address for player 1 Y-axis
#define PLAYER2_X 0b00010000    //Address for player 2 X-axis
#define PLAYER2_Y 0b00010001    //Address for player 2 Y-axis
#define BULLET_X 0b00100000     //Address for bullet X-axis
#define BULLET_Y 0b00100001     //Address for bullet Y-axis
#define GROUND_OFF 0b00110100   //Address to disable one ground sprite
#define GROUND_ON 0b00110101    //Address to enable one ground sprite
#define WRITE_LETTER 0b01110001 //Address to write a character
#define SWITCH_SCREEN 0b11100011//Address to call screen transition
#define BARREL1 0b01010101      //Address for barrel sprite player 1
#define BARREL2 0b01100110      //Address for barrel sprite player 2
#define PLAYSOUND 0b11000011    //Address to play sound effect

//defines for spi data transitions and sounds
#define NO_DATA 0b0000000000000000      //Empty 16-bit data
#define GAMESCREEN 0b1011011000000000   //Transition data main -> game
#define MAINSCREEN 0b0000000001001001   //Transition data game -> main

#define SOUND_EXPLOSION 0b0000000000001111  //Sound data Explosion
#define SOUND_SHOOT 0b0000000011110000      //Sound data Shot
#define SOUND_WIN 0b0000111100000000        //Sound win screen



//Defines for letterboxes
#define CONT_HP1 0b0000     //Address of top left corner letter box
#define CONT_HP2 0b0001     //Address of top right corner letter box
#define CONT_POWER 0b0010   //Address of bottom left corner letter box
#define CONT_ANGLE 0b0011   //Address of bottom right corner letter box
#define CONT_REGEL1 0b0100  //Address of 1st middle row letter box 
#define CONT_REGEL2 0b0101  //Address of 2nd middle row letter box 
#define CONT_REGEL3 0b0110  //Address of 3rd middle row letter box 
#define CONT_REGEL4 0b0111  //Address of 4th middle row letter box 
#define CONT_REGEL5 0b1000  //Address of 5th middle row letter box 
#define CONT_REGEL6 0b1001  //Address of 6th middle row letter box 

//Defines for State Machine
#define INIT_MAIN 0                 //State initalization main menu
#define HOOFDMENU 1                 //State main menu
#define GAME_INIT 2                 //State initalization game
#define GAME_MOVE 3                 //State game player can move
#define GAME_BULLET_INIT 4          //State bullet initalization
#define GAME_BULLET_CALC 5          //State bullet power and angle input
#define GAME_BULLET_INIT_CALC 6     //State bullet set-up shot
#define GAME_IDLE 7                 //State game idle
#define GAME_BULLET_SHOOT 8         //State game bullet in flight
#define GAME_HIT_DETECT 9           //State bullet collision
#define GAME_FINISHED 10            //State game finished
#define GAME_PLAYER_SWITCH 11       //State switching player
#define HIGHSCORES 12               //State highscore menu
#define GAME_END_INIT 13            //State initalization of game over screen
#define GAME_TEST 20                //State for testing purposes

//defines for settings screen
#define SCHERMHOOGTE 480    //Screen height in pixels
#define SCHERMBREEDTE 640   //Screen width in pixels
#define VGABUFFERBREEDTE 0  //VGA offset width
#define VGABUFFERHOOGTE 0   //VGA offset height
#define TILESIZE 8          //Size of ground tiles in pixels
#define MAPSIZE 100         //Map size modifier
#define TILEWIDTH 80        //Amount of ground tiles X-axis
#define TILEHEIGHT 30       //Amount of ground tiles Y-axis
#define TEXT_SIZE 16        //Max amount of characters in letter box

//defines for game settings
#define GRAVITY 1           //Gravity
#define BARREL_STEPS 5      //Barrel sprites of 90 degrees

//defines for physical buttons
#define BUTTON_LEFT 0       //Array number of left in buttonDPAD[]
#define BUTTON_RIGHT 1      //Array number of right in buttonDPAD[]
#define BUTTON_DOWN 2       //Array number of down in buttonDPAD[]
#define BUTTON_UP 3         //Array number of up in buttonDPAD[]
#define BUTTON_SELECT 5     //Array number of select in buttonDPAD[]
#define BUTTON_CONFIRM 4    //Array number of confirm in buttonDPAD[]


#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f0xx_hal.h"

#include <stdio.h>


//empty char array
extern char emptyChar[TEXT_SIZE];

//main menu text chars
extern char start_game_text[TEXT_SIZE];     //char array main menu text line 3
extern char highscore_main_text[TEXT_SIZE]; //char array main menu text line 5
extern char press_confirm_text[TEXT_SIZE];  //char array main menu text line 4
extern char press_select_text[TEXT_SIZE];   //char array main menu text line 6
extern char game_name[TEXT_SIZE];           //char array of game name

//win text for both players
extern char player1_wins_text[TEXT_SIZE];   //char array player 1 win
extern char player2_wins_text[TEXT_SIZE];   //char array player 2 win

//other text chars
extern char hit_text[TEXT_SIZE];            //char array text if hit
extern char highscore_text[TEXT_SIZE];      //char array highscore menu top text

//credits text chars
extern char credits[TEXT_SIZE];             //char array credits text
extern char frenk_text[TEXT_SIZE];          //char array credits person 1
extern char luuk_text[TEXT_SIZE];           //char array credits person 2
extern char tjebbe_text[TEXT_SIZE];         //char array credits person 3

//player restrictions
extern int8_t maxVelY; //max fall speed of player


#endif /* INC_GLOBAL_H_ */
