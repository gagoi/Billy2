#include "mapLoader.h"

// methods

TMX::Parser* mapLoader::tmx = new TMX::Parser();
TSX::Parser* mapLoader::tsx = new TSX::Parser();

mapLoader::mapLoader(std::string filePath, std::string basicName, std::string extension):
    m_filePath(filePath), m_basicName(basicName), m_extension(extension), m_currentLevel(-1) {

}

mapLoader::~mapLoader() {

}

Map * mapLoader::nextLevel() {
    printf("nextLevel\n");
    m_currentLevel++;
    std::string currentLevelName = m_basicName + std::to_string(m_currentLevel) + m_extension;
    return readFromFile(m_filePath, currentLevelName);
}

// static functions

Map * mapLoader::readFromFile(std::string filePath, std::string fileName) {
    std::cout << "loading file: " << filePath + fileName << std::endl;
    tmx->load( (filePath + fileName).c_str() );
    int width = tmx->mapInfo.width;
    int height = tmx->mapInfo.height;
    int beginX = stoi(tmx->mapInfo.property.find("beginX")->second);
    int beginY = stoi(tmx->mapInfo.property.find("beginY")->second);
    int endX = stoi(tmx->mapInfo.property.find("endX")->second);
    int endY = stoi(tmx->mapInfo.property.find("endY")->second);
    float scl = (float) tmx->mapInfo.tileWidth;
    std::string mapStr = tmx->tileLayer[tmx->tileLayer.begin()->first].data.contents;
    std::string texStr = filePath + tmx->tilesetList[0].source;

    Map * map = new Map(width, height, mapStr.c_str(), scl, beginX, beginY, endX, endY);


    tsx->load( texStr.c_str() );

    GLuint *textures = (GLuint*) calloc('Z' - 'A' + 1, sizeof(GLuint));
    bool *isWall = (bool*) calloc('Z' - 'A' + 1, sizeof(bool));

    for(std::vector<TSX::Parser::Tile>::iterator it = tsx->tileList.begin(); it != tsx->tileList.end(); ++it) {
        textures[it->id] = loadTexture((filePath + it->image.source).c_str());
        isWall[it->id] = it->property.find(std::string("Wall"))->second == std::string("true");
    }
    map->initTextures(textures);

    return map;
}
