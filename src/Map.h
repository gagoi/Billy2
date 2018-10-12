#ifndef DEF_MAP
#define DEF_MAP

#include "Wall.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Map{
private:
    int m_width;
    int m_height;
    float m_cellSize;
    std::vector<Wall*> m_layout;

    GLuint * m_textures;
    GLuint m_vboID;
    GLuint m_vaoID;

public:
    Map(int x, int y);
    Map(int x, int y, const char* str, float cellSize);
    ~Map();

    void initTextures(GLuint textures[]);

    char getType(float x, float y);
    Wall * getWall(float x, float y);
    bool isWall(float x, float y);
    bool isFloor(float x, float y);
    int getWidth();
    int getHeight();
    void draw();
    void drawOcc(float viewX, float viewY);
};

#endif