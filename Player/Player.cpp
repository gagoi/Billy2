#include "Player.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

Player::Player(){}

Player::Player(Player& playerSource){}

Player::Player(float x, float y, float angle, float endX, float endY,
    const char * textureFile, const char * visionFile): m_x(x), m_y(y), m_angle(angle),
    m_endX(endX), m_endY(endY)
{

    if(glIsBuffer(m_vboID))
        glDeleteBuffers(1, &m_vboID);
    if(glIsVertexArray(m_vaoID))
        glDeleteVertexArrays(1, &m_vaoID);

    float bufferInfos[16 * 2] = {-32, -32, 
                                32, -32,
                                32, 32,
                                -32, 32,
                                -6*64, -6*64, 
                                6*64, -6*64,
                                6*64, 6*64,
                                -6*64, 6*64,
                                0, 0, 
                                1, 0, 
                                1, 1,
                                0, 1,
                                0, 0, 
                                1, 0, 
                                1, 1,
                                0, 1};

    m_textureID = loadTexture(textureFile);
    m_visionID = loadTexture(visionFile);
    
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

        glBufferData(GL_ARRAY_BUFFER, 16*2 * sizeof(float), bufferInfos, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(8));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Player::tp(float x, float y){
    m_x = x;
    m_y = y;
}


void Player::draw(glm::mat4 projection, glm::mat4 modelview){

    // recup occlusion texture // DONE
    //draw light rays distances tex
    //draw texture of light beams
    //draw scene multiplied with light beams texture

    glBindVertexArray(m_vaoID);

        glBindTexture(GL_TEXTURE_2D, m_textureID);
            
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        glBindTexture(GL_TEXTURE_2D, m_visionID);
        
            glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

        glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
}