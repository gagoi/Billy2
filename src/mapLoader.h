#ifndef MAPLOADER_DEF
#define MAPLOADER_DEF

#include <stdLib.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Map.h"
#include "TMXParser.h"

class mapLoader {
private:
    std::string m_basicName;
    std::string m_extension;
    int m_currentLevel;

public:
    static TMX::Parser* tmx;

    mapLoader(std::string basicName, std::string extension);
    ~mapLoader();
    Map * nextLevel();
    static Map * readFromFile(std::string fileName);
};

#endif
