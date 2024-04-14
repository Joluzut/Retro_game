/*
 * button.c
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */



#include "button.h"

int previousButton[] = {0,0,0,0,0,0};
int8_t buttonDPAD[] = {0,0,0,0,0,0};

uint16_t buttons[6][2] = {
		{BUTTON_GPIOB,GPIO_PIN_4},  	//left
		{BUTTON_GPIOB,GPIO_PIN_6},  	//right
		{BUTTON_GPIOC,GPIO_PIN_7},		//down
		{BUTTON_GPIOB,GPIO_PIN_10},  	//up
		{BUTTON_GPIOA,GPIO_PIN_6},		//rechterknop
		{BUTTON_GPIOB,GPIO_PIN_5}		//linkerknop

};

void buttonInit(){





}




void buttonRead()
{
//	uint16_t buttons[6][2] = {
//			{GPIOB,GPIO_PIN_4},  	//left
//			{GPIOB,GPIO_PIN_6},  	//right
//			{GPIOC,GPIO_PIN_7},		//omlaag
//			{GPIOB,GPIO_PIN_10},  	//omhoog
//			{GPIOA,GPIO_PIN_6},		//linkerknop
//			{GPIOB,GPIO_PIN_5}		//rechterknop
//	};



	for(int i = 0; i < 6; i++){
		previousButton[i] = buttonDPAD[i];
	}

	for(int i = 0; i < 6; i++)
	{
		if(buttons[i][0] == BUTTON_GPIOA){
				buttonDPAD[i] = HAL_GPIO_ReadPin(GPIOA, buttons[i][1]);
		} else if(buttons[i][0] == BUTTON_GPIOB){
			if((i+2) % 2 == 0){
				buttonDPAD[i] = HAL_GPIO_ReadPin(GPIOB, buttons[i][1]);
			} else {
				buttonDPAD[i] = HAL_GPIO_ReadPin(GPIOB, buttons[i][1]) * -1;
			}
		} else if(buttons[i][0] == BUTTON_GPIOC){
			buttonDPAD[i] = HAL_GPIO_ReadPin(GPIOC, buttons[i][1]);
		}
	}


}


