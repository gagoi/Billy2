#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

using namespace std;

void updatePositionN (Ennemy * bot, int numPos, float * pos);
vector updatePositionS (Ennemy * bot, Player * player);
float normeOf(float x, float y);

vector updatePositionS (Ennemy * bot, Player * player){
    float x,y,n;
    x = player->getX() - bot->getX();
    y = player->getY() - bot->getY();
    n = normeOf(x, y);
    x = x / n;
    y = y / n;
    bot->setX(x);
    bot->setY(y);
    r = vector2f(x, y);
    return(r);
}

void updatePositionN(Ennemy * bot, int numPos, float * pos, Player * player)
{
    if( bot.see(player) ){
        back = vector2f();

    }
}

void updatePositionN (Ennemy * bot, int numPos, float * pos)
{
     float x,y,n;
     x = pos[2 * numPos] - bot->getX();
     y = pos[2 * numPos + 1] - bot->getY();
     n = normeOf(x, y);
     x = x / n;
     y = y / n;
     bot->setX(x);
     bot->setY(y);
}

float normeOf(float x, float y)
{
    float r;
    r = sqrt(x * x + y * y);
    return(r);
}


