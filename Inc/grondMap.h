/*
 * grondMap.h
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */

#ifndef INC_GRONDMAP_H_
#define INC_GRONDMAP_H_

#include "global.h"
#include "spiData.h"
#include "main.h"

void mapDataStart();
void mapDataOff();
void mapDataUpdate(int x_data, int y_data);
void genereateRandomMap();
extern uint8_t tileMap[][TILEWIDTH];

#endif /* INC_GRONDMAP_H_ */
