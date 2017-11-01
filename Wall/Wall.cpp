#include "Wall.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

Wall::Wall(){}

Wall::Wall(char type, float size, float x, float y):m_texID(type), m_size(size), m_xPos(x), m_yPos(y){
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
}

char Wall::getType(){
    return m_texID;
}

void Wall::draw(){
    glBindVertexArray(m_vaoID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texID);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}
