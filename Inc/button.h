/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : button.h
  * @brief          : Header for button.c file.
  *                   This file contains external variables of input
  ******************************************************************************
  */


#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "global.h"

#define BUTTON_GPIOA 1 
#define BUTTON_GPIOB 2
#define BUTTON_GPIOC 3


void buttonRead();

extern uint16_t buttons[][2]; //contains STM32 addresses of input pins

extern int8_t buttonDPAD[]; //latest input read
extern int previousButton[]; //previous input read

#endif /* INC_BUTTON_H_ */
