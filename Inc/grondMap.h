/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : grondMap.h
  * @brief          : Header for grondMap.c file.
  *                   This file contains the public funtions regarding the ground
  ******************************************************************************
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

extern uint8_t tileMap[][TILEWIDTH]; //data of the ground in tiles

#endif /* INC_GRONDMAP_H_ */
