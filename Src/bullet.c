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

uint8_t other_player;

void bulletInit(){
	bullet.posX = 50;
	bullet.posY = 550;
	bullet.radius = 4;
	bullet.velX = 0;
	bullet.velY = 0;
	bullet.hit = false;
	bullet.power = 20;
	bullet.bombradius = 2;
	stateBullet = GAME_BULLET_INIT_CALC;
	pos_x = bullet.posX + VGABUFFERBREEDTE;
	sendData(BULLET_X,pos_x);
	pos_y = bullet.posY + VGABUFFERHOOGTE ;
	sendData(BULLET_Y,pos_y);
}

int bulletPower(int power, int dirX){
	power = power + dirX;
	if(power > MAX_POWER){
		power = MAX_POWER;
	} else if(power < 0){
		power = 0;
	}
	return power;
}
int bulletAngle(int angle, int dirX){

	angle = angle + dirX;
	if(angle < MIN_ANGLE){
		angle = MIN_ANGLE;
	} else if(angle > MAX_ANGLE){
		angle = MAX_ANGLE;
	}
	return angle;

}

void bulletSetShot(int power, int angle){

	bullet.velX = power * angle ;
//	if(bullet.velX == 0){
//		bullet.velX = -1000;
//	}
	if(angle > 0){
		bullet.velY = power * (MAX_ANGLE - angle);
	} else {
		bullet.velY = power * (MAX_ANGLE + angle);
	}

	bullet.posX = player[currentPlayer].posX;
	bullet.posY = player[currentPlayer].posY;

}

int bulletUpdatePos()
{
	bullet.posX = bullet.posX + (bullet.velX / MAPSIZE );
	bullet.posY = bullet.posY - (bullet.velY / MAPSIZE );
	if(bullet.velY > MAX_FALLGRAVITY)	{
		bullet.velY = bullet.velY - 3;
	}
	int hit = 0;
	 //zwaartekracht
	//bulletHit functie //vervangen door bordercheck
	if(bullet.posY > SCHERMHOOGTE * MAPSIZE){
		return 4;

	}

	if(bullet.posX < SCHERMBREEDTE * MAPSIZE && bullet.posX > 0){
		if(bullet.posY > 0){
			hit = bulletHitDetect();
		}
	} else {
		return 4;
	}

	return hit; // 1 = other // 2 = self // 3 = ground // 4 = screenboundaries
}
void bulletUpdateBarrel(uint8_t current_player,int8_t old_angle, int8_t directionX){
	int8_t current_angle = player[current_player].angle;
	int8_t rotation_barrel = player[current_player].rotation;
	if(current_angle % 18 == 0 && (current_angle != MAX_ANGLE || current_angle != MIN_ANGLE)  ){ //|| player[current_player].angle % 18  == old_angle + directionX % 18

		player[current_player].rotation -= directionX;
	}


	if(current_angle == 0){
		rotation_barrel = 5;
	} else if(current_angle > 0){
		for(int8_t i = 0; i < BARREL_STEPS;i++){
			if(current_angle > i * STEP_SIZE && current_angle <= (i+1)*STEP_SIZE){
				rotation_barrel = BARREL_STEPS - (i+1);
			}
		}
	} else if(current_angle < 0){
		for(int8_t i = 0; i < BARREL_STEPS;i++){
			if(current_angle < i * -STEP_SIZE && current_angle >= (i+1)*-STEP_SIZE){
				rotation_barrel = BARREL_STEPS + (i+1);
			}
		}
	}
	player[current_player].rotation = rotation_barrel;
	if(current_player == 0){
		sendData(BARREL1,player[current_player].rotation);
	} else {
		sendData(BARREL2,player[current_player].rotation);
	}
}

int bulletLoop(uint8_t current_player){

	other_player = (current_player+1)%2;
	directionX = buttonDPAD[BUTTON_LEFT] + buttonDPAD[BUTTON_RIGHT];
	directionY = buttonDPAD[BUTTON_DOWN] + buttonDPAD[BUTTON_UP];
	int8_t oldAngle = player[current_player].angle;

	if(stateBullet == GAME_BULLET_INIT_CALC){
		stateBullet = GAME_BULLET_CALC;
	} else if(stateBullet == GAME_BULLET_CALC){ //powerCalc
		if(directionX != 0 && (buttonDPAD[BUTTON_LEFT] != previousButton[BUTTON_LEFT] || buttonDPAD[BUTTON_RIGHT] != previousButton[BUTTON_RIGHT])){
			player[current_player].angle = bulletAngle(player[current_player].angle,directionX);
			updateWriteShot(player[current_player].power,player[current_player].angle);
			bulletUpdateBarrel(current_player, oldAngle, directionX);
		}
		if(directionY != 0 && (buttonDPAD[BUTTON_DOWN] != previousButton[BUTTON_DOWN] || buttonDPAD[BUTTON_UP] != previousButton[BUTTON_UP])){
			player[current_player].power = bulletPower(player[current_player].power,directionY);
			updateWriteShot(player[current_player].power,player[current_player].angle);
		}
		if(buttonDPAD[BUTTON_CONFIRM] != 0 && previousButton[BUTTON_CONFIRM] != buttonDPAD[BUTTON_CONFIRM] ){
			stateBullet = GAME_BULLET_INIT;
		}


	} else if(stateBullet == GAME_BULLET_INIT){ //bulletInit
		bulletSetShot(player[current_player].power,player[current_player].angle);
		stateBullet = GAME_BULLET_SHOOT;
		sendData(PLAYSOUND,SOUND_SHOOT);
		sendData(PLAYSOUND,NO_DATA);
	} else if(stateBullet == GAME_BULLET_SHOOT){ //bulletFlight
		if(bullet.posX < SCHERMBREEDTE * MAPSIZE && bullet.posX > 0){
			//if(bullet.posY > 0){
				hitReturn = bulletUpdatePos();
			//}
		} else {
			//stateBullet = GAME_BULLET_INIT_CALC;
			hitReturn = 1;
		}


		if(hitReturn != 0){
			player[current_player].score = changeScorePlayer( hitReturn,player[current_player].score);
			player[other_player].score = changeScoreOpponent( hitReturn,player[other_player].score);
			sendData(PLAYSOUND,SOUND_EXPLOSION);
			sendData(PLAYSOUND,NO_DATA);

			if(hitReturn != 4 && hitReturn != 3){
				writeBox(CONT_REGEL2,hit_text);
			}

				if(player[0].hp <= 0){
					return 2;
				}
				if(player[1].hp <= 0){
					return 3;
				}
				//player[currentPlayer].hp = player[currentPlayer].hp - bullet.power;


			bullet.posX = 10;
			bullet.posY = 55000;

			pos_x = bullet.posX + VGABUFFERBREEDTE * MAPSIZE ;
			sendData(BULLET_X,pos_x/MAPSIZE);
			pos_y = bullet.posY + VGABUFFERHOOGTE * MAPSIZE;
			sendData(BULLET_Y,pos_y/MAPSIZE);
			stateBullet = GAME_BULLET_INIT_CALC;
			return 1;
		}



	}
	//if(hitReturn == )
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

int bulletHitPlayer(uint8_t otherPlayer){
	int radiusTotaal;
	int zijdeX;
	int zijdeY;

	radiusTotaal = bullet.radius + player[otherPlayer].radius ;
	if(bullet.posX > player[otherPlayer].posX){
		zijdeX = bullet.posX - player[otherPlayer].posX;
	} else {
		zijdeX = player[otherPlayer].posX - bullet.posX;
	}
	zijdeX = zijdeX / MAPSIZE;
	if(bullet.posY > player[otherPlayer].posY){
		zijdeY = bullet.posY - player[otherPlayer].posY;
	} else {
		zijdeY = player[otherPlayer].posY - bullet.posY;
	}
	zijdeY = zijdeY / MAPSIZE;
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
void playerHealthUpdate(uint8_t playerHit, uint8_t bullet_power){
	player[playerHit].hp = player[playerHit].hp - bullet_power;
	if(player[playerHit].hp < 0){
		player[playerHit].hp = 100;
	}
	updateWriteHP( playerHit,  player[playerHit].hp);
	//SHOW ANIMATION // SOUND



}

int bulletHitDetect(){

	if(bullet.posX > 0 && bullet.posX < SCHERMBREEDTE * MAPSIZE){
		if(bullet.posY > 0){

			if(bulletHitPlayer(other_player) == 1){
				playerHealthUpdate(other_player, bullet.power);
				return 1;
			} else if(bulletHitPlayer(currentPlayer) == 1 && bullet.velY < 0){
				playerHealthUpdate(currentPlayer, bullet.power);
				return 2;
			} else if(bulletHitGround() == 1){
				return 3;
			}
		}
	} else {
		return 4;
	}




	if(bullet.posY > SCHERMHOOGTE * MAPSIZE){
		//return 1;
	}


	return 0;
}




