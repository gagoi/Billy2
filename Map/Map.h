#ifndef DEF_MAP
#define DEF_MAP

#include "../Wall/Wall.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Map{
private:
    int m_width;
    int m_height;
    float m_cellSize;
    std::vector<Wall*> m_layout;

    GLuint m_textures[26];
    GLuint m_vboID;
    GLuint m_vaoID;

public:
    Map(int x, int y);
    Map(int x, int y, const char* str, float cellSize);
    ~Map();

    void initTextures(GLuint textures[]);

    char getType(float x, float y);
    Wall * getWall(float x, float y);
    int getWidth();
    int getHeight();
    void draw();
};

#endif