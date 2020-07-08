#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include "CharBlockLookup.h"

static int const MAX_LINE_SIZE = 1024;
static char const COMMENT_CHAR = ';';

void readNextNotCommentedLine(std::ifstream & fin, char* buffer) {
    char & next = buffer[0]; //ooooooh dat pointer magic * 
    while (true) {
        fin.getline(buffer, MAX_LINE_SIZE);
        if (next != COMMENT_CHAR) {
            break;
        }
        if (fin.eof()) {
            throw std::exception("Could not read next line in file. :(");
        }
    }
}

void fillInMeta(std::ifstream & fin, Block ** map, char * buffer, int width, int height, bool metaA) {
    int i = 0;
    while (true) {
        readNextNotCommentedLine(fin, buffer);
        if (!buffer[0] == '\0') {
            int j = 0;
            while (buffer[j] != '\0') {
                if (j >= width) {
                    std::cout << "WARNING: In the map file for meta there is a line that exceeds the level bounds\n";
                    break;
                }
                int metaValue = buffer[j] - '0';//some stackoverflow magic to get the actual number value from the char
                if (metaA) {
                    map[j][i].setMetaA(metaValue); 
                } else {
                    map[j][i].setMetaB(metaValue); 
                }
                ++j;
            }
            if (++i >= height) {
                std::cout << "WARNING: In the map file for meta there are more lines than in the level itself\n";
                break;
            }
        }
    }
}

std::shared_ptr<GameMap> loadFromFile(std::string file) {

    std::ifstream fin;
    fin.open(file.c_str());
    if (!fin.is_open()) {
        throw std::exception("Could not open map file :(");
    }

    char buffer[MAX_LINE_SIZE];
    readNextNotCommentedLine(fin, buffer);
    std::string textureName(buffer);

    std::vector<std::string> lines;
    while (true) {
        readNextNotCommentedLine(fin, buffer);
        std::string nextLine = std::string(buffer);
        if (nextLine.empty()) {
            break;
        }
        lines.push_back(nextLine);
    }
    int width = lines[0].size();
    int height = lines.size();
    
    Block ** map = new Block*[width];
    for (int i = 0; i < width; i++) {
        map[i] = new Block[height];
    }

    for (int i = 0; i < height; i++) {
        std::string line = lines[i];
        if (line.size() != width) {
            throw std::exception("Invalid level file, not uniform line lengths in the main map part (with the characters)");
        }
        for  (int j = 0; j < width; j++) {
            map[j][i] = Block(getBlockFromCharacter(line[j]));
        }
    }
    
    fillInMeta(fin, map, buffer, width, height, true);
    fillInMeta(fin, map, buffer, width, height, false);

    GameMap * gameMap = new GameMap(map, width, height, textureName);

    return std::dynamic_pointer_cast<GameMap>(gameMap->ref());
}