#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <vector>
#include <string>

struct Tile
{
    bool isWall;
    bool hasCeiling;
    int floorTexIndex;
    int wallTexIndex;
    char raw;
};

class MapLoader
{
public:
    bool load(const char *filename);

    const std::vector<std::string> &data() const { return mapData; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    float getPlayerStartX() const { return playerStartX; }
    float getPlayerStartZ() const { return playerStartZ; }

    Tile getTile(int x, int y) const;

private:
    std::vector<std::string> mapData;
    int width = 0;
    int height = 0;

    float playerStartX = 1.0f;
    float playerStartZ = 1.0f;
};

#endif
