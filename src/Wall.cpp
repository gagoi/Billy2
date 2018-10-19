#include "Wall.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

Wall::Wall(){}

Wall::Wall(char type, GLuint ** textures, float size, float x, float y):m_texID(type), m_textures(textures), m_size(size), m_xPos(x), m_yPos(y){
    float vboData[4*2 *2] = {  x, y,
                            x+size, y,
                            x+size, y+size,
                            x, y+size,
                            0, 0,
                            1, 0,
                            1, 1,
                            0, 1};
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);
        glGenBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
            glBufferData(GL_ARRAY_BUFFER, 4*2 *2 *sizeof(float), vboData, GL_STATIC_DRAW);
            
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(4*2 *1 * sizeof(float)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float vboOccData[10*3] = {  x+size, y, -1,      // right begin
                                x+size, y, 1,
                                x+size, y+size, -1, // bottom begin
                                x+size, y+size, 1,
                                x, y+size, -1,      // left begin
                                x, y+size, 1,       
                                x, y, -1,           // top begin
                                x, y, 1,
                                x+size, y, -1,      // loop
                                x+size, y, 1 };

    glGenVertexArrays(1, &m_vaoOccID);
    glBindVertexArray(m_vaoOccID);
        glGenBuffers(1, &m_vboOccID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboOccID);
            glBufferData(GL_ARRAY_BUFFER, 10*3 * sizeof(float), vboOccData, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

char Wall::getType(){
    return m_texID;
}

void Wall::draw(){
    glBindVertexArray(m_vaoID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (*m_textures)[m_texID]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Wall::drawOcc(float lightX, float lightY, int neighbors[4]){
    /*
    if(m_texID != 0) { // if not a wall #TODO need proper way to check it
        glBindVertexArray(m_vaoOccID);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
        glBindVertexArray(0);
    }
    */
    /*
    2 etapes: 
        - cotes recevant de la lumière
        - occlusion par un bloc
        - propagation des signaux si bloc plein
    */
    bool lighted[4] = {0};
    lighted[0] = lightX >= m_xPos + m_size   && isFloor(neighbors[0]);  // right side
    lighted[1] = lightY >= m_yPos + m_size   && isFloor(neighbors[1]);  // bottom side
    lighted[2] = lightX <= m_xPos            && isFloor(neighbors[2]);           // left side
    lighted[3] = lightY <= m_yPos            && isFloor(neighbors[3]);           // top side

    bool draw[4] = {true, true, true, true};

    if(lighted[0] || lighted[2]){
        draw[1] = draw[1] && isFloor(neighbors[1]);
        draw[3] = draw[3] && isFloor(neighbors[3]);
    }
    if(lighted[1] || lighted[3]){
        draw[0] = draw[0] && isFloor(neighbors[0]);
        draw[2] = draw[2] && isFloor(neighbors[2]);
    }

    if(isWall()) {
        glBindVertexArray(m_vaoOccID);
            if(draw[0])
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            if(draw[1])
                glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
            if(draw[2])
                glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            if(draw[3])
                glDrawArrays(GL_TRIANGLE_STRIP, 6, 4);
        glBindVertexArray(0);
    }
}

bool Wall::isWall() {
    return m_texID != 8;
}

bool Wall::isFloor() {
    return !isWall();
}

bool Wall::isWall(int i) {
    return i != 8;
}

bool Wall::isFloor(int i) {
    return !isWall(i);
}
