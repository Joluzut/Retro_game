#include "highscore.h"

char highscore[15] = "500200100050025";			//highscore save
char spelernamen[15] = "abcdefghijklmno";		//highscore player names save
uint16_t intHighscores[5] = {500,200,100,50,25};//highscore conversion to int 

/**
  * @brief shows current highscores on screen of certain line
  * @param uint8_t line number
  * @returns none
  * 
  * 
  */
void showScore(uint8_t regel){
	uint8_t regel_offset = regel * 3;
	char data[16] = "";
	for(uint8_t i = 0; i < 6; i++){
		if(i < 3){
			data[i] = spelernamen[i + regel_offset];
		} else {
			data[i] = highscore[i - 3 + regel_offset];
		}

		}
	uint8_t container_regel = CONT_REGEL2 + regel;
	writeBox(container_regel,data);
}
/**
  * @brief Initalization of highscore menu screen
  * @param none
  * @returns none
  * 
  * 
  */
void highscoreMenu(){

	writeBox(CONT_HP1,credits);
	writeBox(CONT_HP2,frenk_text);
	writeBox(CONT_POWER,luuk_text);
	writeBox(CONT_ANGLE,tjebbe_text);
	writeBox(CONT_REGEL1,highscore_text);

	for(int i = 0; i < 5; i++){
		showScore(i);
	}



}
/**
  * @brief Update highscores with new entry
  * @param char name of new entry
  * @param uint8_t place of new entr
  * @returns none
  * 
  * 
  */
void updateScore(char new_player[TEXT_SIZE], uint8_t place){
	uint8_t hundreds ;
	uint8_t tens ;
	uint8_t ones ;

	for(uint8_t i = 0; i < 5;i++){
		hundreds = intHighscores[i]/100;
		tens = (intHighscores[i]/10)%10;
		ones = intHighscores[i]%10;
		highscore[0 + (i*3)] = hundreds + '0';
		highscore[1 + (i*3)] = tens + '0';
		highscore[2 + (i*3)] = ones + '0';
	}
	uint8_t new_place;
	for(new_place = 14;new_place >= place*3;new_place--){
		spelernamen[new_place] = spelernamen[new_place-3];
	}
	spelernamen[new_place] = new_player[2];
	spelernamen[new_place-1] = new_player[1];
	spelernamen[new_place-2] = new_player[0];
}
/**
  * @brief creates screen to input name of new score
  * @param uint8_t place
  * @returns none
  * 
  * 
  */
void inputName(uint8_t place){
	char new_player[16] = "aaa";
	writeBox(CONT_REGEL3,"enter name      "); // max 36
	int8_t id = 0;

	buttonRead();
	uint8_t directionX = buttonDPAD[BUTTON_LEFT] + buttonDPAD[BUTTON_RIGHT];
	uint8_t directionY = buttonDPAD[BUTTON_DOWN] + buttonDPAD[BUTTON_UP];
	while(buttonDPAD[BUTTON_SELECT] == 0 || previousButton[BUTTON_SELECT] == buttonDPAD[BUTTON_SELECT]){
		writeBox(CONT_REGEL4,new_player);
		buttonRead();
		directionX = buttonDPAD[BUTTON_LEFT] + buttonDPAD[BUTTON_RIGHT];
		directionY = buttonDPAD[BUTTON_DOWN] + buttonDPAD[BUTTON_UP];
		if(directionX != 0 && (buttonDPAD[BUTTON_LEFT] != previousButton[BUTTON_LEFT] || buttonDPAD[BUTTON_RIGHT] != previousButton[BUTTON_RIGHT])){
			id += directionX;
			if(id < 0){
				id = 2;
			} else if(id > 2){
				id = 0;
			}
		}
		if(directionY != 0 && (buttonDPAD[BUTTON_DOWN] != previousButton[BUTTON_DOWN] || buttonDPAD[BUTTON_UP] != previousButton[BUTTON_UP])){
			new_player[id] += directionY;
			if(new_player[id] < 97){
				new_player[id] = 122;
			} else if(new_player[id] > 122){
				new_player[id] = 97;
			}
		}



	}


	updateScore(new_player, place);
}
/**
  * @brief Compares new score with the highscores
  * @param uint16_t new score
  * @returns none
  * 
  * 
  */
void compareScores(uint16_t new_score){
	uint16_t hundreds;
	uint8_t tens;
	uint8_t ones;

	for(uint8_t i = 0; i < 15; i+=3){
		hundreds = (highscore[0 + i] - '0');
		tens = (highscore[1 + i] - '0');
		ones = highscore[2 + i] - '0';
		intHighscores[i/3] = hundreds * 100 + tens * 10 + ones;
	}
	uint8_t scoreSlot;
	for(scoreSlot = 0;scoreSlot < 5;scoreSlot++){
		if(intHighscores[scoreSlot] <= new_score){
			for(int8_t j = 4; j > scoreSlot; j--){
				intHighscores[j] = intHighscores[j-1];
			}
			intHighscores[scoreSlot] = new_score;
			//writeBox(CONT_REGEL5,score);
			inputName(scoreSlot+1);

			break;
		}
	}

}

/**
  * @brief Change score of current player in the ongoing game
  * @param uint8_t type of hit
  * @param int16_t current score of player
  * @returns int
  * 
  * 
  */
int changeScorePlayer(uint8_t hit_type,int16_t current_score){
	int8_t scoreMod; // 1 = other // 2 = self // 3 = ground // 4 = screenboundaries
    if(hit_type == 1){
    	scoreMod = 50;
    } else if(hit_type == 2){
    	scoreMod = -25;
    } else if(hit_type == 3){
    	scoreMod = -3;
    } else if(hit_type == 4){
    	scoreMod = -6;
    }
    return current_score + scoreMod;
}
/**
  * @brief Change score of oponent in the ongoing game
  * @param uint8_t type of hit
  * @param int16_t current score of oponent
  * @returns int
  * 
  * 
  */
int changeScoreOpponent(uint8_t hit_type,int16_t current_score){
	int8_t scoreMod; // 1 = other // 2 = self // 3 = ground // 4 = screenboundaries
    if(hit_type == 1){
    	scoreMod = -25;
    } else if(hit_type == 2){
    	scoreMod = 0;
    } else if(hit_type == 3){
    	scoreMod = 0;
    } else if(hit_type == 4){
    	scoreMod = 0;
    }
    return current_score + scoreMod;
}
/**
  * @brief Add bonus score at end of game based in played turns
  * @param uint16_t turn game ended
  * @param int16_t current score of winning player
  * @returns int
  * 
  * 
  */
int addTurnScore(uint16_t turn_n,int16_t current_score){

	return current_score + (600 - (turn_n * 20));
}
