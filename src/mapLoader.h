#ifndef MAPLOADER_DEF
#define MAPLOADER_DEF

#include <stdLib.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include "Map.h"
#include "TMXParser.h"
#include "TSXParser.h"
#include "sdlglutils.h"

class mapLoader {
private:
    std::string m_filePath;
    std::string m_basicName;
    std::string m_extension;
    int m_currentLevel;

public:
    static TMX::Parser* tmx;
    static TSX::Parser* tsx;

    mapLoader(std::string filePath, std::string basicName, std::string extension);
    ~mapLoader();
    Map * nextLevel();
    static Map * readFromFile(std::string filePath, std::string fileName);
};

#endif
