#include "bullet.h"
#include <math.h>
#include <stdlib.h>

int VelocityX;
int VelocityY;
int zwaarteKracht = 30;

void set(int power, int angle){
    VelocityX = power * angle;
   if(angle > 0){
       VelocityY = power * (100 - angle);
    }else{
        VelocityY = power * (100 + angle);
    }
}
int berekenX(int Xout)
{
    return Xout + VelocityX;
}

int berekenY(int Youd)
{
    VelocityY = VelocityY + zwaarteKracht;
    return Youd - VelocityY;
}