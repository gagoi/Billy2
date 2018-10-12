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
            m_layout.push_back(new Wall(str[j*x + i] - 'A', &m_textures, m_cellSize, i*m_cellSize, j*m_cellSize));
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
    if(x >= m_width*m_cellSize || x < 0 || y >= m_height*m_cellSize || y < 0)
        return 0;

    return m_layout[ ((int)(y/m_cellSize))*m_width + (int)(x/m_cellSize) ]->getType();
}

bool Map::isWall(float x, float y) {
    return getType(x, y) != 0;
}

bool Map::isFloor(float x, float y) {
    return getType(x, y) == 0;
}

Wall * Map::getWall(float x, float y){
    return m_layout[ ((int)(y/m_cellSize))*m_width + (int)(x/m_cellSize) ];
}

void Map::draw(){
    for(Wall *wall: m_layout)
        wall->draw();
}

void Map::drawOcc(float viewX, float viewY){
    int neighbors[4] = {0};
    //inner
    for(int x = 1; x < m_width-1; x++)
        for(int y = 1; y < m_height-1; y++) {
            neighbors[0] = m_layout[y*m_width + x+1]->getType();
            neighbors[1] = m_layout[(y+1)*m_width + x]->getType();
            neighbors[2] = m_layout[y*m_width + x-1]->getType();
            neighbors[3] = m_layout[(y-1)*m_width + x]->getType();
            m_layout[y*m_width + x]->drawOcc(viewX, viewY, neighbors);
        }
    // sides
    for(int x = 1; x < m_width - 1; x++){ // y 0
        neighbors[0] = m_layout[x+1]->getType();
        neighbors[1] = m_layout[1*m_width + x]->getType();
        neighbors[2] = m_layout[x-1]->getType();
        neighbors[3] = 0;
        m_layout[x]->drawOcc(viewX, viewY, neighbors);
    }
    for(int x = 1; x < m_width - 1; x++){ // y height-1
        neighbors[0] = m_layout[(m_height-1)*m_width + x+1]->getType();
        neighbors[1] = 0;
        neighbors[2] = m_layout[(m_height-1)*m_width + x-1]->getType();
        neighbors[3] = m_layout[(m_height-2)*m_width + x]->getType();
        m_layout[(m_height-1)*m_width + x]->drawOcc(viewX, viewY, neighbors);
    }
    for(int y = 1; y < m_height - 1; y++){ // x 0
        neighbors[0] = m_layout[y*m_width + 1]->getType();
        neighbors[1] = m_layout[(y+1)*m_width]->getType();
        neighbors[2] = 0;
        neighbors[3] = m_layout[(y-1)*m_width]->getType();
        m_layout[y*m_width]->drawOcc(viewX, viewY, neighbors);
    }
    for(int y = 1; y < m_height - 1; y++){ // x width-1
        neighbors[0] = 0;
        neighbors[1] = m_layout[(y+1)*m_width + m_width-1]->getType();
        neighbors[2] = m_layout[y*m_width + m_width-2]->getType();
        neighbors[3] = m_layout[(y-1)*m_width + m_width-1]->getType();
        m_layout[y*m_width + m_width-1]->drawOcc(viewX, viewY, neighbors);
    }
    // corners
    { // x 0 y 0
        neighbors[0] = m_layout[1]->getType();
        neighbors[1] = m_layout[1*m_width]->getType();
        neighbors[2] = 0;
        neighbors[3] = 0;
        m_layout[0]->drawOcc(viewX, viewY, neighbors);
    }
    { // x 0 y height-1
        neighbors[0] = m_layout[(m_height-1)*m_width + 1]->getType();
        neighbors[1] = 0;
        neighbors[2] = 0;
        neighbors[3] = m_layout[(m_height-2)*m_width]->getType();
        m_layout[(m_height-1)*m_width]->drawOcc(viewX, viewY, neighbors);
    }
    { // x width-1 y 0
        neighbors[0] = 0;
        neighbors[1] = m_layout[1*m_width + m_width-1]->getType();
        neighbors[2] = m_layout[m_width-2]->getType();
        neighbors[3] = 0;
        m_layout[m_width-1]->drawOcc(viewX, viewY, neighbors);
    }
    { // x width-1 y height-1
        neighbors[0] = 0;
        neighbors[1] = 0;
        neighbors[2] = m_layout[(m_height-1)*m_width + m_width-2]->getType();
        neighbors[3] = m_layout[(m_height-2)*m_width + m_width-1]->getType();
        m_layout[(m_height-1)*m_width + m_width-1]->drawOcc(viewX, viewY, neighbors);
    }
}