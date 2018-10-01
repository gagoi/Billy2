#include "Map.h"
#include <stdio.h>

Map::Map(int x, int y):m_width(x), m_height(y){
    m_layout.reserve(x*y);
    for(int i = 0; i < x*y; i++){
        m_layout[i] = new Wall();
    }
}

Map::Map(int x, int y, const char * str, float cellSize):m_width(x), m_height(y), m_cellSize(cellSize){
    for(int j = 0; j < y; j++){
        for(int i = 0; i < x; i++){
            m_layout.push_back(new Wall(str[j*x + i] - 'A', &m_textures, cellSize, i*cellSize, j*cellSize));
        }
    }
}

Map::~Map(){
}

void Map::initTextures(GLuint * textures){
    m_textures = textures;
}

int Map::getWidth(){
    return m_width;
}

int Map::getHeight(){
    return m_height;
}

char Map::getType(float x, float y){
    return m_layout[ ((int)(y*m_width/m_cellSize)) + (int)(x/m_cellSize) ]->getType();
}

Wall * Map::getWall(float x, float y){
    return m_layout[ ((int)(y*m_width/m_cellSize)) + (int)(x/m_cellSize) ];
}

void Map::draw(){
    for(Wall *wall: m_layout)
        wall->draw();
}