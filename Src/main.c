/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "global.h"
#include "button.h"
#include "grondMap.h"
#include "bullet.h"
#include "player.h"
#include "letterBox.h"
#include "mainmenu.h"
#include "highscore.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/*
 * De resolutie bedraagt 640 : 480. De FPGA 800:600 tekenen maar 640:480 zichtbaar. Beginnend linksboven. Lege ruimte is 144breed en 31 hoog.
 * Voor precieze locaties moet deze waardes opgeteld worden. Voor game logica is 0,0 locatie linksonder. Dus voor y-as een inversie
 * Voor x-as: x-positie + vga buffer breedte.
 * Voor y-as: schermhoogte - y-positie + vga buffer hoogte
 */




//1left 2right 3down 4up




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC_Init(void);
/* USER CODE BEGIN PFP */
//static int8_t buttonDPAD[] = {0,0,0,0};


void sendData(uint8_t, uint16_t);
void mapDataStart();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
  // correct byte for led control
  uint16_t pos_x; //0b0000 0001 0011 0110
  uint16_t pos_y;


	static int state = INIT_MAIN;
	uint8_t game_over = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);


	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

	void buttonInit();
	int hit;
	uint16_t turn = 0;
	int16_t score = 0;



	while (1)
	{
	  	if(state != HOOFDMENU && state != INIT_MAIN){
	  		buttonRead();
	  	}
		if(state == GAME_TEST){

		}

		if(state == INIT_MAIN){
			sendData(SWITCH_SCREEN,MAINSCREEN);
			sendData(SWITCH_SCREEN,NO_DATA);

			for(int j = 0; j < 6; j++){
				buttonDPAD[j] = 0;
				previousButton[j] = 0;
			}
			buttonRead();
			mainMenu();
			playerOff();
			mapDataOff();
			HAL_Delay(3000);

			state = HOOFDMENU;
		}else if(state == HOOFDMENU){
			buttonRead();
			if( buttonDPAD[BUTTON_SELECT] != 0 && previousButton[BUTTON_SELECT] != buttonDPAD[BUTTON_SELECT] )
			{
				state = HIGHSCORES;
				highscoreMenu();
			}
			if( buttonDPAD[BUTTON_CONFIRM] != 0 && previousButton[BUTTON_CONFIRM] != buttonDPAD[BUTTON_CONFIRM] )
			{
				state = GAME_INIT;
			}
		} else if(state == HIGHSCORES){

			if( buttonDPAD[BUTTON_SELECT] != 0 && previousButton[BUTTON_SELECT] != buttonDPAD[BUTTON_SELECT] )
			{
				state = HOOFDMENU;
				mainMenu();
			}
		} else if(state == GAME_INIT){
			genereateRandomMap();
			sendData(SWITCH_SCREEN,GAMESCREEN);
			playerInit();
			mapDataStart();
			bulletInit();
			writeBox(CONT_REGEL2,emptyChar);
			writeBox(CONT_REGEL3,emptyChar);
			writeBox(CONT_REGEL4,emptyChar);
			writeBox(CONT_REGEL5,emptyChar);
			writeBox(CONT_REGEL6,emptyChar);
			sendData(SWITCH_SCREEN,NO_DATA);
			turn = -1;
			state = GAME_PLAYER_SWITCH;
			for(int j = 0; j < 6; j++){
				buttonDPAD[j] = 0;
				previousButton[j] = 0;
			}

			sendData(BARREL1,player[0].rotation);

			sendData(BARREL2,player[1].rotation);
			HAL_Delay(1000);

		} else	if(state == GAME_MOVE){
			game_over = playerMovement(currentPlayer);
			if(buttonDPAD[BUTTON_SELECT] != 0 && previousButton[BUTTON_SELECT] != buttonDPAD[BUTTON_SELECT] )
			{
				//game_over = 1;
			}
			if(game_over != 0){
				state = GAME_END_INIT;

			}
			if(buttonDPAD[BUTTON_CONFIRM] != 0 && previousButton[BUTTON_CONFIRM] != buttonDPAD[BUTTON_CONFIRM] )
			{
				writeBox(CONT_REGEL2,emptyChar);
				state = GAME_BULLET_INIT;
			}
		} else if(state == GAME_BULLET_INIT){
			bulletInit();
			updateWriteShot(player[currentPlayer].power,player[currentPlayer].angle);
			state = GAME_BULLET_CALC;
		} else if(state == GAME_BULLET_CALC){
			hit = bulletLoop(currentPlayer);
			if(hit == 1){
				state = GAME_PLAYER_SWITCH;
			}
			else if(hit == 2){ //player 1 death
				game_over = 1;
			} else if(hit == 3){ //player 2 deatch
				game_over = 2;
			}
			if(game_over != 0){
				state = GAME_END_INIT;
			}
		} else if(state == GAME_PLAYER_SWITCH){

			for(uint8_t i = 0; i < 2; i++)
			{
				game_over = playerMovement(i);

				pos_x = player[i].posX + VGABUFFERBREEDTE * MAPSIZE;
				if(i == 0){
					sendData(PLAYER1_X,pos_x/MAPSIZE - player[i].radius);
				} else {
					sendData(PLAYER2_X,pos_x/MAPSIZE - player[i].radius);
				}
				pos_x = player[i].posX;

				pos_y = player[i].posY + VGABUFFERBREEDTE * MAPSIZE;
				if(i == 0){
					sendData(PLAYER1_Y,pos_y/MAPSIZE - player[i].radius);
				} else {
					sendData(PLAYER2_Y,pos_y/MAPSIZE - player[i].radius);
				}
				pos_y = player[i].posY;

			}

			if(player[0].inAir == false && player[1].inAir == false ){
				turn++;
				state = GAME_MOVE;
				writeBox(CONT_REGEL3,"player switch   ");
				HAL_Delay(1500);
				writeBox(CONT_REGEL3,emptyChar);
				writeBox(CONT_REGEL2,emptyChar);

				if(currentPlayer == 0){
					currentPlayer = 1;
				} else {
					currentPlayer = 0;
				}
				updatePlayer(currentPlayer);
				writeBox(CONT_POWER,emptyChar);
				writeBox(CONT_ANGLE,emptyChar);

			}
			if(game_over != 0){
				state = GAME_END_INIT;

			}
		} else if(state == GAME_END_INIT){
			turn = turn / 2;
			sendData(PLAYSOUND,SOUND_WIN);

			if(game_over == 2){
				writeBox(CONT_REGEL2,"player 1 wins  ");
				score = player[0].score;
			} else if(game_over == 1){
				writeBox(CONT_REGEL2,"player 2 wins  ");
				score = player[1].score;
			}
			score = addTurnScore(turn,score);

			char scoreChar[16] = "score 000";
			scoreChar[6] = (score/100) + '0';
			scoreChar[7] = ((score/10)%10) + '0';
			scoreChar[8] = (score%10) + '0';

			writeBox(CONT_REGEL5,scoreChar);

			//game_over = 0;
			if(score >= 0){
				compareScores(score);
			}

			state = GAME_FINISHED;
		} else if(state == GAME_FINISHED){



			if(buttonDPAD[BUTTON_SELECT] != 0 && previousButton[BUTTON_SELECT] != buttonDPAD[BUTTON_SELECT] )
			{
				sendData(PLAYSOUND,NO_DATA);
				game_over = 0;
				turn = 0;
				score = 0;
				state = INIT_MAIN;
			}

		}



		// send data via SPI //adjust map size
		if(pos_x != player[currentPlayer].posX ){
			pos_x = player[currentPlayer].posX + VGABUFFERBREEDTE * MAPSIZE;
			if(currentPlayer == 0){
				sendData(PLAYER1_X,pos_x/MAPSIZE - player[currentPlayer].radius);
			} else {
				sendData(PLAYER2_X,pos_x/MAPSIZE - player[currentPlayer].radius);
			}
			pos_x = player[currentPlayer].posX;
		}
		if(player[currentPlayer].velY != 0 ){
			pos_y = player[currentPlayer].posY + VGABUFFERHOOGTE * MAPSIZE;
			if(currentPlayer == 0){
				sendData(PLAYER1_Y,pos_y/MAPSIZE - player[currentPlayer].radius);
			} else {
				sendData(PLAYER2_Y,pos_y/MAPSIZE - player[currentPlayer].radius);
			}
			pos_y = player[currentPlayer].posY;
		}




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

  /*Configure GPIO pin : PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */











/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
