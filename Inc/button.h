/*
 * button.h
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "global.h"

#define BUTTON_GPIOA 1
#define BUTTON_GPIOB 2
#define BUTTON_GPIOC 3


void buttonRead();

extern uint16_t buttons[][2];



extern int8_t buttonDPAD[];
extern int previousButton[];

#endif /* INC_BUTTON_H_ */
