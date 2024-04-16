/*
 * letterBox.h
 *
 *  Created on: Apr 3, 2024
 *      Author: frenk
 */

#ifndef INC_LETTERBOX_H_
#define INC_LETTERBOX_H_

#include "global.h"
#include "spiData.h"


#define CHAR_INVIS 0b00000000   //Data no character
#define CHAR_A 0b00000001   //Spi data A      
#define CHAR_B 0b00000010   //Spi data B
#define CHAR_C 0b00000011   //Spi data C
#define CHAR_D 0b00000100   //Spi data D
#define CHAR_E 0b00000101   //Spi data E
#define CHAR_F 0b00000110   //Spi data F
#define CHAR_G 0b00000111   //Spi data G
#define CHAR_H 0b00001000   //Spi data H
#define CHAR_I 0b00001001   //Spi data I
#define CHAR_J 0b00001010   //Spi data J
#define CHAR_K 0b00001011   //Spi data K
#define CHAR_L 0b00001100   //Spi data L
#define CHAR_M 0b00001101   //Spi data M
#define CHAR_N 0b00001110   //Spi data N
#define CHAR_O 0b00001111   //Spi data O
#define CHAR_P 0b00010000   //Spi data P
#define CHAR_Q 0b00010001   //Spi data Q
#define CHAR_R 0b00010010   //Spi data R
#define CHAR_S 0b00010011   //Spi data S
#define CHAR_T 0b00010100   //Spi data T
#define CHAR_U 0b00010101   //Spi data U
#define CHAR_V 0b00010110   //Spi data V
#define CHAR_W 0b00010111   //Spi data W
#define CHAR_X 0b00011000   //Spi data X
#define CHAR_Y 0b00011001   //Spi data Y
#define CHAR_Z 0b00011010   //Spi data Z
#define CHAR_1 0b00011011   //Spi data 1
#define CHAR_2 0b00011100   //Spi data 2
#define CHAR_3 0b00011101   //Spi data 3
#define CHAR_4 0b00011110   //Spi data 4
#define CHAR_5 0b00011111   //Spi data 5
#define CHAR_6 0b00100000   //Spi data 6
#define CHAR_7 0b00100001   //Spi data 7
#define CHAR_8 0b00100010   //Spi data 8
#define CHAR_9 0b00100011   //Spi data 9
#define CHAR_0 0b00100100   //Spi data 0


void writeLetter(uint8_t groep, uint8_t locatie, uint8_t letter);
void writeBox(uint8_t groep, char tekst[TEXT_SIZE]);

void updateWriteHP(uint8_t player_write, uint8_t new_hp);
void updateWriteShot(uint16_t current_power, int16_t current_angle);
void updatePlayer(uint8_t current_player);




extern char hp_player1[TEXT_SIZE];
extern char hp_player2[TEXT_SIZE];

extern char power_box[TEXT_SIZE];
extern char angle_box[TEXT_SIZE];

extern char turn_player[TEXT_SIZE];



#endif /* INC_LETTERBOX_H_ */





