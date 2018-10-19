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

    int m_entryTileX;
    int m_entryTileY;
    int m_exitTileX;
    int m_exitTileY;

    GLuint * m_textures;
    GLuint m_vboID;
    GLuint m_vaoID;

public:
    Map(int width, int height);
    Map(int width, int height, const char* str, float cellSize,
        int entryTileX, int entryTileY, int exitTileX, int exitTileY);
    ~Map();

    void initTextures(GLuint textures[]);

    float getEntryCoordX();
    float getEntryCoordY();
    bool isOnExitTile(float x, float y);

    char getType(float x, float y);
    Wall * getWall(float x, float y);
    int getWidth();
    int getHeight();
    float getScale();
    void draw();
    void drawOcc(float viewX, float viewY);
};

#endif