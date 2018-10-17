#include <stdLib.h>
#include <string>
#include <stdexcept>
#include "Map.h"

class mapLoader {
private:
    std::string m_basicName;
    int m_currentLevel;
public:
    mapLoader(std::string basicName);
    ~mapLoader();
    Map * nextLevel();
    static Map * readFromFile(std::string fileName);
};
