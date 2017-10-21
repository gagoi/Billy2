#ifndef DEF_PLAYER
#define DEF_PLAYER

class Player{
private:
    float m_x;
    float m_y;
    float m_angle;

    float m_endX;
    float m_endY;
    
    GLuint m_textureID;
    GLuint m_visionID;
    GLuint m_vboID;
    GLuint m_vaoID;

public:
    Player();
    Player(Player& playerSource);
    Player(float x, float y, float angle, float endX, float endY,
        const char * textureFile, const char * visionFile);

    void getX();
    void getY();
    void getAngle();
    void move(float x, float y);
    void tp(float x, float y);
    void turn(float angle);

    void draw(glm::mat4 projection, glm::mat4 modelview);

};


#endif