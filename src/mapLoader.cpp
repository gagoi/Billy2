#include "mapLoader.h"

// methods

TMX::Parser* mapLoader::tmx = new TMX::Parser();

mapLoader::mapLoader(std::string basicName, std::string extension):m_basicName(basicName), m_extension(extension),
    m_currentLevel(-1) {

}

mapLoader::~mapLoader() {

}

Map * mapLoader::nextLevel() {
    printf("nextLevel\n");
    m_currentLevel++;
    std::string currentLevelName = m_basicName;
    return readFromFile(currentLevelName.append(std::to_string(m_currentLevel).append(m_extension)));
}

// static functions

Map * mapLoader::readFromFile(std::string fileName) {
    std::cout << "loading file: " << fileName << std::endl;
    tmx->load( fileName.c_str() );
    int width = tmx->mapInfo.width;
    int height = tmx->mapInfo.height;
    int beginX = stoi(tmx->mapInfo.property.find("beginX")->second);
    int beginY = stoi(tmx->mapInfo.property.find("beginY")->second);
    int endX = stoi(tmx->mapInfo.property.find("endX")->second);
    int endY = stoi(tmx->mapInfo.property.find("endY")->second);
    float scl = (float) tmx->mapInfo.tileWidth;
    const char * mapStr;
    Map * map;

    mapStr = tmx->tileLayer[tmx->tileLayer.begin()->first].data.contents.c_str();

    map = new Map(width, height, mapStr, scl, beginX, beginY, endX, endY);

    return map;
}

/*Map * mapLoader::readFromFile(std::string fileName) {
    int width = 0, height = 0;
    int beginX, beginY, endX, endY;
    float scl = 75.f;
    char * mapStr;
    Map * map;

    FILE * f = fopen64(fileName.c_str(), "r");
    if(f == NULL) {
        char * errStr = (char*) calloc(30, sizeof(char));
        sprintf(errStr, "could not open file \"%s\"", fileName.c_str());
        throw std::runtime_error(errStr);
    }


    char * line = (char*) calloc(20, sizeof(char));

    fgets(line, 20, f); width = atoi(line);
    fgets(line, 20, f); height = atoi(line);
    fgets(line, 20, f); scl = atof(line);
    fgets(line, 20, f); beginX = atoi(line);
    fgets(line, 20, f); beginY = atoi(line);
    fgets(line, 20, f); endX = atoi(line);
    fgets(line, 20, f); endY = atoi(line);

    printf("%d, %d, %f\n", width, height, scl);

    mapStr = (char*) calloc(width * height, sizeof(char));

    for(int i = 0; i < width*height; i++) {
        int c;
        do {
            c = fgetc(f);
        } while( c != EOF && ( c < 'A' || c > 'Z'));
        mapStr[i] = c;
    }

    map = new Map(width, height, mapStr, scl, beginX, beginY, endX, endY);

    return map;
}
*/
