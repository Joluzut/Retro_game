/*
 * bullet.c
 *
 *  Created on: Mar 28, 2024
 *      Author: frenk
 */
#include "bullet.h"

struct bulletData bullet;
uint16_t pos_x; //0b0000 0001 0011 0110
uint16_t pos_y;
static int stateBullet;
int8_t directionX = 0; // -1 = L || 1 == R
int8_t directionY = 0; // -1 = D || 1 == U
int hitReturn = 0;

void bulletInit(){
//	bullet.posX = player[currentPlayer].posX;
//	bullet.posY = player[currentPlayer].posY;
	bullet.radius = 4;
	bullet.velX = 0;
	bullet.velY = 0;
	bullet.hit = false;
	bullet.power = 20;
	bullet.bombradius = 2;
	stateBullet = GAME_BULLET_INIT_CALC;
}

int bulletPower(int power, int dirX){
	power = power + dirX;
	if(power > 70){
		power = 70;
	} else if(power < 0){
		power = 0;
	}
	return power;
}
int bulletAngle(int angle, int dirX){

	if(angle > -50 && angle < 50){
		angle = angle + dirX;
	}
	return angle;

}

void bulletSetShot(int power, int angle){

	bullet.velX = power * angle ;
//	if(bullet.velX == 0){
//		bullet.velX = -1000;
//	}
	if(angle > 0){
		bullet.velY = power * (100 - angle);
	} else {
		bullet.velY = power * (100 + angle);
	}

	bullet.posX = player[currentPlayer].posX;
	bullet.posY = player[currentPlayer].posY;

}

int bulletPos()
{
	bullet.posX = bullet.posX + (bullet.velX / 100 );
	bullet.posY = bullet.posY - (bullet.velY / 100 );
	if(bullet.velY > -3000)	{
		bullet.velY = bullet.velY - 3;
	}
	 //zwaartekracht
	//bulletHit functie //vervangen door bordercheck
//	if(bullet.posY < SCHERMHOOGTE / 2 * MAPSIZE){
		if(bulletHit() == 1){
			return 1;
		}

	//}

	if(bullet.posX >= 64000 || bullet.posX < 0){
		return 1;
	}

	return 0; // if hit 1
}

int bulletSchiet(int current_player){

		//bulletInit();
	directionX = buttonDPAD[BUTTON_LEFT] + buttonDPAD[BUTTON_RIGHT];
	directionY = buttonDPAD[BUTTON_DOWN] + buttonDPAD[BUTTON_UP];

//		if(buttonDPAD[2] != previousButton[2] ){
//
//		pos_x = bullet.posX + VGABUFFERBREEDTE ;
//		//sendData(0b01000000,pos_x/100);
//		pos_x = bullet.posX;
//
//
//		pos_y = bullet.posY + VGABUFFERHOOGTE;
//		//sendData(0b00000000,pos_y/100);
//		pos_y = bullet.posY;
//
//
//			}
	if(stateBullet == GAME_BULLET_INIT_CALC){
		stateBullet = GAME_BULLET_CALC;
	} else if(stateBullet == GAME_BULLET_CALC){ //powerCalc
		if(directionX != 0 && (buttonDPAD[BUTTON_LEFT] != previousButton[BUTTON_LEFT] || buttonDPAD[BUTTON_RIGHT] != previousButton[BUTTON_RIGHT])){
			player[current_player].angle = bulletAngle(player[current_player].angle,directionX);
			//sendData(0b11111111,bullet.power);
		}
		if(directionY != 0 && (buttonDPAD[BUTTON_DOWN] != previousButton[BUTTON_DOWN] || buttonDPAD[BUTTON_UP] != previousButton[BUTTON_UP])){
			player[current_player].power = bulletPower(player[current_player].power,directionY);

					//sendData(0b11111111,bullet.angle);
		}
		if(buttonDPAD[BUTTON_CONFIRM] != 0 && previousButton[BUTTON_CONFIRM] != buttonDPAD[BUTTON_CONFIRM] ){
			stateBullet = GAME_BULLET_INIT;
		}


	} else if(stateBullet == GAME_BULLET_INIT){ //bulletInit
		bulletSetShot(player[current_player].power,player[current_player].angle);
		stateBullet = GAME_BULLET_SHOOT;
	} else if(stateBullet == GAME_BULLET_SHOOT){ //bulletFlight
		hitReturn = bulletPos();
		if(hitReturn != 1){
			if(hitReturn == 1){
				player[(currentPlayer+1)%2].hp = player[(currentPlayer+1)%2].hp - bullet.power;
			} else if(hitReturn == 2){
				player[currentPlayer].hp = player[currentPlayer].hp - bullet.power;
			}

			bullet.posX = 10;
			bullet.posY = 55000;
			stateBullet = GAME_BULLET_INIT_CALC;

			return 1;
		}

//klljklk
			// send data via SPI //adjust map size



	}
	if(pos_x != bullet.posX ){
		pos_x = bullet.posX + VGABUFFERBREEDTE * MAPSIZE ;
		sendData(BULLET_X,pos_x/MAPSIZE);
		pos_x = bullet.posX;
	}


	if(pos_y != bullet.posY && bullet.posY > 0)	{
		//asm('NOP');
		pos_y = bullet.posY + VGABUFFERHOOGTE * MAPSIZE;
		sendData(BULLET_Y,pos_y/MAPSIZE);
		pos_y = bullet.posY;
	}


	return 0;
}

int bulletHitPlayer(int otherPlayer){
	int radiusTotaal;
	int zijdeX;
	int zijdeY;

	radiusTotaal = bullet.radius + player[otherPlayer].radius * MAPSIZE;
	if(bullet.posX > player[otherPlayer].posX){
		zijdeX = bullet.posX - player[otherPlayer].posX;
	} else {
		zijdeX = player[otherPlayer].posX - bullet.posX;
	}

	if(bullet.posY > player[otherPlayer].posY){
		zijdeY = bullet.posY - player[otherPlayer].posY;
	} else {
		zijdeY = player[otherPlayer].posY - bullet.posY;
	}

	int cKwadraat = radiusTotaal*radiusTotaal;
	int bKwadraat = zijdeX * zijdeX;
	int aKwadraat = zijdeY * zijdeY;
	if(cKwadraat > bKwadraat + aKwadraat ){
		return 1;
	} else{
		return 0;
	}

}

int bulletHitGround(){

	uint16_t tileColX;
	uint16_t tileColY;

	// remaining space between grid and exact
	uint8_t hit = 0;

	int tempPosY = -1 * ((SCHERMHOOGTE * MAPSIZE / 2) - bullet.posY + bullet.radius * MAPSIZE); //onderkant tank
	if(tempPosY < 0){
		tempPosY = 0;
	}
	tileColY = ( ( tempPosY / MAPSIZE) + bullet.radius) / TILESIZE;

	tileColX = ( (  bullet.posX / MAPSIZE ) + bullet.radius ) / TILESIZE;
	if(tileMap[tileColY][tileColX] == 1 && tileColX < TILEWIDTH)
	{
		tileMap[tileColY][tileColX] = 0;
		mapDataUpdate(tileColX, tileColY);
		hit = 1;
	}
//	modTileX = ( bullet.posX + ( MAPSIZE * bullet.radius ) ) % (TILESIZE * MAPSIZE);

	tileColX = ( ( bullet.posX / MAPSIZE ) - bullet.radius ) / TILESIZE;
	if(tileMap[tileColY][tileColX] == 1 && tileColX < TILEWIDTH)
	{
		tileMap[tileColY][tileColX] = 0;
		mapDataUpdate(tileColX, tileColY);
		hit = 1;
	}
//	modTileX = ( player[current_player].posX - ( MAPSIZE * player[current_player].radius ) ) % (TILESIZE * MAPSIZE);

	return hit;
}

int bulletHit(){
	int otherPlayer;
	if(bullet.posY > 0){
		if( currentPlayer == 0){
			otherPlayer = 1;
		} else {
			otherPlayer = 0;
		}
		if(bulletHitPlayer(otherPlayer) == 1){
			return 1;
		} else if(bulletHitPlayer(currentPlayer) == 1 && bullet.velY < 0){
			return 2;
		} else if(bulletHitGround() == 1){

			return 3;
		}
	}





	if(bullet.posY > SCHERMHOOGTE * MAPSIZE){
		//return 1;
	}


	return 0;
}




