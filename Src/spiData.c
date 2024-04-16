/*
 * spiData.c
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */


#include "spiData.h"


/**
  * @brief SPI data transfer write function of 3 bytes 
  * @param uint8_t address 
  * @param uint16_t data
  * @retval None
  * 
  * 
  */
void sendData(uint8_t address, uint16_t data)
{
			uint8_t bitData[3];
			bitData[2] = data & 0xff;
			bitData[1] = (data >> 8);
			bitData[0] = address; // first byte is address



			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_SPI_Transmit(&hspi1, bitData, 3, 100);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

}
