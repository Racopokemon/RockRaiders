#include "MapLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include "CharBlockLookup.h"

static int const MAX_LINE_SIZE = 1024;
static char const COMMENT_CHAR = ';';

void readNextNotCommentedLine(std::ifstream & fin, char* buffer) {
    char & next = buffer[0]; //sum pointer magic
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

std::vector<std::string> readChunk(std::ifstream & fin) {
    std::vector<std::string> lines;
    char buffer[MAX_LINE_SIZE];
    while (true) {
        readNextNotCommentedLine(fin, buffer);
        std::string nextLine = std::string(buffer);
        if (nextLine.empty()) {
            break;
        } else {
            lines.push_back(nextLine);
        }
    }
    return lines;
}

void fillInMeta(std::vector<std::string> & lines, Block ** map, int width, int height, bool metaA) {
    int i = 0;
    if (lines.size() < height) {
        throw std::exception("Metadata has less lines than the level itself.");
    }
    while (true) {
        std::string line = lines[i];
        int j = 0;
        while (line[j] != '\0') {
            if (j >= width) {
                std::cout << "WARNING: In the map file for meta there is a line that exceeds the level bounds\n";
                break;
            }
            int metaValue = line[j] - '0';//some stackoverflow magic to get the actual number value from the char
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

std::string combineStrings(std::vector<std::string> lines) {
    std::string s = lines[0];
    for (int i = 1; i < lines.size(); i++) {
        s = s + "\n" + lines[i];
    }
    return s;
}

std::shared_ptr<GameMap> loadFromFile(std::string file, MapData & mapDataReturn) {


    std::ifstream fin;
    fin.open(file.c_str());
    if (!fin.is_open()) {
        throw std::exception("Could not open map file :(");
    }

    mapDataReturn.textureName = readChunk(fin)[0];
    mapDataReturn.crystalsToWin = std::stoi(readChunk(fin)[0]);
    mapDataReturn.oresToWin = std::stoi(readChunk(fin)[0]);
    mapDataReturn.artifactsToWin = std::stoi(readChunk(fin)[0]);
    mapDataReturn.basesToWin = std::stoi(readChunk(fin)[0]);
    mapDataReturn.startText = combineStrings(readChunk(fin));
    mapDataReturn.winText = combineStrings(readChunk(fin));

    std::vector<std::string> lines = readChunk(fin);

    int width = lines[0].length();
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
    
    fillInMeta(readChunk(fin), map, width, height, true);
    fillInMeta(readChunk(fin), map, width, height, false);

    GameMap * gameMap = new GameMap(map, width, height, mapDataReturn.textureName);

    return std::dynamic_pointer_cast<GameMap>(gameMap->ref());
}