#include "Vector2f.h"

Vector2f::Vector2f(float x, float y) : m_x(x), m_y(y){

}

float Vector2f::getX(){
    return m_x;
}

float Vector2f::getY(){
    return m_y;
}

void Vector2f::setX(float x){
    m_x = x;
}

void Vector2f::setY(float y){
    m_y = y;
}
