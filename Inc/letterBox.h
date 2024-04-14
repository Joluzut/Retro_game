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


#define CHAR_INVIS 0b00000000
#define CHAR_A 0b00000001
#define CHAR_B 0b00000010
#define CHAR_C 0b00000011
#define CHAR_D 0b00000100
#define CHAR_E 0b00000101
#define CHAR_F 0b00000110
#define CHAR_G 0b00000111
#define CHAR_H 0b00001000
#define CHAR_I 0b00001001
#define CHAR_J 0b00001010
#define CHAR_K 0b00001011
#define CHAR_L 0b00001100
#define CHAR_M 0b00001101
#define CHAR_N 0b00001110
#define CHAR_O 0b00001111
#define CHAR_P 0b00010000
#define CHAR_Q 0b00010001
#define CHAR_R 0b00010010
#define CHAR_S 0b00010011
#define CHAR_T 0b00010100
#define CHAR_U 0b00010101
#define CHAR_V 0b00010110
#define CHAR_W 0b00010111
#define CHAR_X 0b00011000
#define CHAR_Y 0b00011001
#define CHAR_Z 0b00011010
#define CHAR_1 0b00011011
#define CHAR_2 0b00011100
#define CHAR_3 0b00011101
#define CHAR_4 0b00011110
#define CHAR_5 0b00011111
#define CHAR_6 0b00100000
#define CHAR_7 0b00100001
#define CHAR_8 0b00100010
#define CHAR_9 0b00100011
#define CHAR_0 0b00100100


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





