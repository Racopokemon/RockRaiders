#ifndef MAP_DATA_H
#define MAP_DATA_H

struct MapData {
    std::string textureName;

    std::string winText;
    std::string startText;

    int crystalsToWin;
    int oresToWin;
    int artifactsToWin;
    int basesToWin;
};

#endif