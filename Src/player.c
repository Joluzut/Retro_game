/*
 * player.c
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */



#include "player.h"


struct playerData player[2];
int currentPlayer;

/**
  * @brief Generates random spawn for both players
  * @param none
  * @returns int
  * 
  * 
  */
int rndSpawn(){
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
	int seed = HAL_ADC_GetValue(&hadc);
	srand(seed);
	unsigned int startLocation = rand() % 81 + 20;
	return startLocation;
}

/**
  * @brief disable tank sprites of both players
  * @param none
  * @returns none
  * 
  * 
  */
void playerOff(){
	player[0].posX = 10; //0b0000 0001 0011 0110
	player[0].posY = 55000;
	sendData(PLAYER1_X,player[0].posX / MAPSIZE);
	sendData(PLAYER1_Y,player[0].posY / MAPSIZE);


	player[1].posX = 10; //0b0000 0001 0011 0110
	player[1].posY = 55000;
	sendData(PLAYER1_X,player[1].posX / MAPSIZE);
	sendData(PLAYER1_Y,player[1].posY / MAPSIZE);



}

/**
  * @brief Initalization of player data. 
  * @param none
  * @returns none
  * 
  * 
  */
void playerInit(){

	player[0].posX = rndSpawn() * MAPSIZE; //0b0000 0001 0011 0110
	player[0].posY = 20000;
	player[0].radius = 7;
	player[0].speed = 1;
	player[0].directionX = 1;
	player[0].rotation = 2;
	player[0].velX = 0;
	player[0].velY = 0;
	player[0].inAir = true;
	player[0].power = 25;
	player[0].angle = 45;
	player[0].hp = 100;
	player[0].score = 0;

	player[1].posX = (rndSpawn()+500) * MAPSIZE; //0b0000 0001 0011 0110
	player[1].posY = 20000;
	player[1].radius = 7;
	player[1].speed = 1;
	player[1].directionX = 1;
	player[1].rotation = 7;
	player[1].velX = 0;
	player[1].velY = 0;
	player[1].inAir = true;
	player[1].power = 25;
	player[1].angle = -45;
	player[1].hp = 100;
	player[1].score = 0;

	currentPlayer = 1;
	for(int i = 0; i < 2; i++){
		updateWriteHP( i,  player[i].hp);
	}





}

/**
  * @brief Movement function of player including collision with map. 
  * Return 0 = player is on the map
  * Return 1 = player 1 fell down the map
  * Return 2 = player 2 fell down the map
  * @param uint8_t current player
  * @returns int
  * 
  * 
  */
int playerMovement(uint8_t current_player)
{
	int16_t tempPosY;
	int8_t directionX = 0; // -1 = L || 1 == R


	// direction calc
	directionX = buttonDPAD[BUTTON_LEFT] + buttonDPAD[BUTTON_RIGHT];
	if(directionX != 0){
		//char hitText[16] = "";
		//writeBox(CONT_REGEL1,hitText);
	}


	//collision map x-axis

	//tile calc including radius and direction for background coliision

	uint16_t tileColX;
	uint16_t tileColY;

	// remaining space between grid and exact
	uint16_t modTileX;
	uint16_t modTileY;



	if(player[current_player].inAir == false)
	{
		tempPosY = -1 * ((SCHERMHOOGTE * MAPSIZE / 2) - player[current_player].posY + player[current_player].velY + player[current_player].radius * MAPSIZE); //onderkant tank
		if(tempPosY < 0){
			tempPosY = 0;
		}
		tileColY = ( ( tempPosY / MAPSIZE) + player[current_player].radius) / TILESIZE;
		if(directionX == 1)
		{
			tileColX = ( (  player[current_player].posX / MAPSIZE ) + player[current_player].radius ) / TILESIZE;
			if(tileMap[tileColY][tileColX] == 1 && tileColX < TILEWIDTH)
			{
				directionX = 0;
				if(tileMap[tileColY-1][tileColX] == 0 && tileColX < TILEWIDTH && tileMap[tileColY-1][tileColX-1] == 0 ){
					directionX = 1;
					player[current_player].posY -= (1000);
				}
			}
			modTileX = ( player[current_player].posX + ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);
		}
		else if(directionX == -1)
		{
			tileColX = ( ( player[current_player].posX / MAPSIZE ) - player[current_player].radius ) / TILESIZE;
			if(tileMap[tileColY][tileColX] == 1 && tileColX < TILEWIDTH)
			{

				directionX = 0; // NEW x set
				if(tileMap[tileColY-1][tileColX] == 0 && tileColX < TILEWIDTH && tileMap[tileColY-1][tileColX+1] == 0 ){
					directionX = -1;
					player[current_player].posY -= (1000);
				}
			}
			modTileX = ( player[current_player].posX - ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);
		}
		if(player[current_player].posX + player[current_player].radius * MAPSIZE > 63850){
			player[current_player].posX--;
		} else if ( player[current_player].posX < (player[current_player].radius + 1) * MAPSIZE) {
			player[current_player].posX++;;
		} else {
			player[current_player].posX = player[current_player].posX + (directionX * player[current_player].speed);
		}
		tempPosY = -1 * ((SCHERMHOOGTE * MAPSIZE / 2) - player[current_player].posY + player[current_player].velY + player[current_player].radius * MAPSIZE);
		tileColY = ( ( tempPosY / MAPSIZE) + player[current_player].radius) / TILESIZE;
		tileColX = ( ( player[current_player].posX / MAPSIZE ) - player[current_player].radius ) / TILESIZE; //left border
		if(tileMap[tileColY+1][tileColX] == 0){
			tileColX = (( player[current_player].posX / MAPSIZE ) + player[current_player].radius ) / TILESIZE; //right border
			if(tileMap[tileColY+1][tileColX] == 0){
				tileColX = (player[current_player].posX / MAPSIZE ) / TILESIZE; //right border
				if(tileMap[tileColY+1][tileColX] == 0){
					player[current_player].inAir = true;
				}

			} else {

			}
		} else {
		}
	}
	else //if in air different all borders have to be checked
	{
		//add falling speed
		if(player[current_player].velY < maxVelY){
			player[current_player].velY = player[current_player].velY + GRAVITY;
		} else {
			player[current_player].velY = maxVelY;
		}

		//check if player can still fall down // only check below half the size of screen --> no ground above that line
		if(player[current_player].posY + player[current_player].velY + player[current_player].radius > (SCHERMHOOGTE * MAPSIZE)/ 2) {
			tempPosY = -1 * (((SCHERMHOOGTE * MAPSIZE) / 2) - player[current_player].posY + player[current_player].velY);

			tileColY = ( ( tempPosY / MAPSIZE) + player[current_player].radius ) / TILESIZE;
			tileColX = ( ( player[current_player].posX / MAPSIZE) - player[current_player].radius ) / TILESIZE; //left border
			if(tileMap[tileColY][tileColX] == 1){
				modTileY = (tempPosY + ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);
				player[current_player].posY = player[current_player].posY + player[current_player].velY - modTileY;
				player[current_player].inAir = false;
				player[current_player].velY = 0;

			} else {
				tileColX = ( ( player[current_player].posX / MAPSIZE) + player[current_player].radius ) / TILESIZE; //right border
				if(tileMap[tileColY][tileColX] == 1){
					modTileY = (tempPosY + ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);
					player[current_player].posY = player[current_player].posY + player[current_player].velY - modTileY - 1;
					player[current_player].inAir = false;
					player[current_player].velY = 0;
				} else {
					player[current_player].posY = player[current_player].posY + player[current_player].velY;
				}
			}
		} else {
			player[current_player].posY = player[current_player].posY + player[current_player].velY;
		}
	}

	if(player[current_player].posY > SCHERMHOOGTE * MAPSIZE){
		return current_player + 1;
	} else {
		return 0;
	}
}


