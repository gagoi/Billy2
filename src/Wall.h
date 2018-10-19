#ifndef DEF_WALL
#define DEF_WALL

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <stdio.h>

class Wall{
private:
    GLuint m_texID;
    GLuint m_vboID;
    GLuint m_vboOccID;
    GLuint m_vaoID;
    GLuint m_vaoOccID;

    GLuint ** m_textures;

    float m_size;
    float m_xPos;
    float m_yPos;

public:
    Wall();
    Wall(char type, GLuint ** textures, float size, float xPos, float yPos);

    char getType();

    void draw();
    void drawOcc(float viewX, float viewY, int neighbors[4]);

    bool isWall();
    bool isFloor();
    static bool isWall(int i);
    static bool isFloor(int i);
};

#endif