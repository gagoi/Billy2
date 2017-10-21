#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

using namespace std;

void updatePosition (int numPos, float * pos, float * bot);
float normeOf(float x, float y);

int main()
{
    /*float * pos;
    pos = (float *) malloc(2 * sizeof(float));
    float * bot;
    bot = (float *) malloc(2 * sizeof(float));
    pos[0] = 1;
    pos[1] = -3;
    bot[0] = -5;
    bot[1] = 2;
    updatePosition(0, pos, bot);
    printf("%f,%f\n",bot[0],bot[1]);
    */
    vector<float> back;
    back.push_back(2);
    back.push_back(2);
    back.push_back(2);
    back.push_back(2);        
    printf("%d\n",back.capacity());
    return(0);
}

void updatePosition (int numPos, float * pos, float * bot)
{
     float x,y,n;
     x = pos[2 * numPos] - bot[0];
     y = pos[2 * numPos + 1] - bot[1];
     n = normeOf(x, y);
     x = x / n;
     y = y / n;
     bot[0]=x;
     bot[1]=y;
}

float normeOf(float x, float y)
{
    float r;
    r = sqrt(x * x + y * y);
    return(r);
}
 



