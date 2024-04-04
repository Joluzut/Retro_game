/*
 * player.c
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */



#include "player.h"


struct playerData player[2];
int currentPlayer;
void playerInit(){

	player[0].posX = 32000; //0b0000 0001 0011 0110
	player[0].posY = 10000;
	player[0].radius = 7;
	player[0].speed = 1;
	player[0].directionX = 1;
	player[0].rotation = 8;
	player[0].velX = 0;
	player[0].velY = 0;
	player[0].inAir = true;
	player[0].power = 25;
	player[0].angle = 25;
	player[0].hp = 100;

	player[1].posX = 55000; //0b0000 0001 0011 0110
	player[1].posY = 10000;
	player[1].radius = 7;
	player[1].speed = 1;
	player[1].directionX = 1;
	player[1].rotation = 8;
	player[1].velX = 0;
	player[1].velY = 0;
	player[1].inAir = true;
	player[1].power = 25;
	player[1].angle = -25;
	player[1].hp = 100;

	currentPlayer = 1;

}
void playerMovement(int current_player)
{
	int16_t tempPosY;
	int8_t directionX = 0; // -1 = L || 1 == R
	int8_t directionY = 0; // -1 = D || 1 == U

	uint8_t i,j;
	uint8_t rotation = 0; // 0-7
	//temp var for testing


	// direction calc
	directionX = buttonDPAD[BUTTON_LEFT] + buttonDPAD[BUTTON_RIGHT];
	directionY = buttonDPAD[BUTTON_DOWN] + buttonDPAD[BUTTON_UP];
	if(directionY == -1)
	{
		//sendData(0b00110001, 0b0010101101010000); //0b0010 1011 0101 0000
		//mapDataStart();

		//player[currentPlayer].posY = 15000;
		//player[currentPlayer].inAir = true;
	} else if(directionY == 1) {
		//sendData(0b00000000,player[currentPlayer].posX/100);
		//sendData(0b00110000, 0b0010101101010000);

	}

	//rotation calc
	for(i = -1; i < 2;i++)
	{
		for(j = -1; j < 2; j++)
		{
			if(directionX == i)
			{
				if(directionY == j)
				{
					if(i != 0 && j != 0) //dont update when player idle
					{
						player[current_player].rotation = rotation;
					}
				}
			}
		rotation++;
		}
	}
	//direction calc
	if(directionX != 0) //update direction if player is not idle
	{
		player[current_player].directionX = directionX;
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
			}
			modTileX = ( player[current_player].posX + ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);
		}
		else if(directionX == -1)
		{
			tileColX = ( ( player[current_player].posX / MAPSIZE ) - player[current_player].radius ) / TILESIZE;
			if(tileMap[tileColY][tileColX] == 1 && tileColX < TILEWIDTH)
			{
				directionX = 0; // NEW x set
			}
			modTileX = ( player[current_player].posX - ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);
		}
//		if(player[current_player].posX + player[current_player].radius < 63999 || player[current_player].posX - player[current_player].radius > 0){
//		player[current_player].posX = player[current_player].posX + (directionX * player[current_player].speed);
//		} else {
//
//		}
		if(player[current_player].posX + player[current_player].radius * MAPSIZE > 63999){
			player[current_player].posX--;
		} else if ( player[current_player].posX < (player[current_player].radius + 1) * MAPSIZE) {
			player[current_player].posX++;;
		} else {
			player[current_player].posX = player[current_player].posX + (directionX * player[current_player].speed);
		}

		tileColX = ( ( player[current_player].posX / MAPSIZE ) - player[current_player].radius ) / TILESIZE; //left border
		if(tileMap[tileColY+1][tileColX] == 0){
			tileColX = (( player[current_player].posX / MAPSIZE ) + player[current_player].radius ) / TILESIZE; //right border
			if(tileMap[tileColY+1][tileColX] == 0){
				player[current_player].inAir = true;
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




}
