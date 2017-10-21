#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "Vector2f"

using namespace std;

void updatePositionN (Ennemy * bot, int numPos, float * pos, float mvt);
void updatePositionS (Ennemy * bot, Player * player, float mvt);
float normeOf(float x, float y);
void updatePosition(Ennemy * bot, int numPos, float * pos, Player * player, float mvt);


void updatePosition(vector<vector2f> back, Ennemy * bot, int numPos, float * pos, Player * player, float mvt)
{
    if( notEmpty(back) ){

    }
}

void updatePositionS (Ennemy * bot, Player * player, float mvt){
    float x,y,n;
    x = player->getX() - bot->getX();
    y = player->getY() - bot->getY();
    n = normeOf(x, y);
    x = x / n;
    y = y / n;
    bot->setX(bot->getX() + x * mvt);
    bot->setY(bot->getY() + y * mvt);
    Vector2f r(5, 6);

    return(r);
}

void updatePositionN (Ennemy * bot, int numPos, float * pos, float mvt)
{
     float x,y,n;
     x = pos[2 * numPos] - bot->getX();
     y = pos[2 * numPos + 1] - bot->getY();
     n = normeOf(x, y);
     x = x / n;
     y = y / n;
     bot->setX(bot->getX() + x * mvt);
     bot->setY(bot->getY() + y * mvt);
}

float normeOf(float x, float y)
{
    float r;
    r = sqrt(x * x + y * y);
    return(r);
}


