/*
 * letterBox.c
 *
 *  Created on: Apr 3, 2024
 *      Author: frenk
 */

#include "letterBox.h"
#include "spiData.h"

void writeBox(uint8_t groep, uint8_t locatie, uint8_t letter){
	uint16_t totale_data = (groep << 12 | locatie << 8 | letter);
	sendData(WRITE_LETTER,totale_data);
}

//void writeLetter(uint8_t groep, char tekst[16]){
//	char temp_tekst[16];
//	uint8_t letter;
//	int i,j;
//	for(i=0;i<16;i++){
//		temp_tekst[i] = tekst[i];
//		}
//
//
//	if(groep == CONT_HP2 || groep == CONT_ANGLE){
//		for(i=0;i<16;i++){
//			tekst[i+7] = temp_tekst[i];
//		}
//	}
//	//memset(temp_tekst, '\0', sizeof(temp_tekst));
//	for(i=0;i<16;i++){
//
//		for(j = 48;j<57;j++){
//			if(tekst[i] == 48){
//				letter = CHAR_0;
//			} else {
//				if(tekst[i] == j){
//					letter = j-22;
//				}
//			}
//		}
//		for(j=97;j<122;j++){
//			if(tekst[i] == j){
//				letter = j-1;
//			}
//		}
//	writeBox(groep,i,letter);
//	}
//
//
//}


