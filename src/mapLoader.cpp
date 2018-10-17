#include "mapLoader.h"

// methods

mapLoader::mapLoader(std::string basicName):m_basicName(basicName), m_currentLevel(-1) {

}

mapLoader::~mapLoader() {

}

Map * mapLoader::nextLevel() {
    printf("nextLevel\n");
    m_currentLevel++;
    std::string currentLevelName = m_basicName;
    return readFromFile(currentLevelName.append(std::to_string(m_currentLevel)));
}

// static functions
Map * mapLoader::readFromFile(std::string fileName) {
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
