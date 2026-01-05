#include "maploader.h"
#include <fstream>
#include <iostream>

bool MapLoader::load(const char *filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "ERRO CRITICO: Nao foi possivel abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string line;
    mapData.clear();
    height = 0;
    width = 0;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty() || line[0] == '#')
            continue;

        for (int x = 0; x < (int)line.size(); x++)
        {
            if (line[x] == '9')
            {
                playerStartX = (float)x;
                playerStartZ = (float)height;
            }
        }

        mapData.push_back(line);
        if ((int)line.size() > width)
            width = (int)line.size();
        height++;
    }

    file.close();
    return (height > 0 && width > 0);
}

Tile MapLoader::getTile(int x, int y) const
{
    Tile t;
    t.isWall = false;
    t.hasCeiling = true;
    t.floorTexIndex = 0;
    t.wallTexIndex = 0;
    t.raw = ' ';

    if (x < 0 || x >= width || y < 0 || y >= height)
        return t;
    if (x >= (int)mapData[y].size())
        return t;

    char c = mapData[y][x];
    t.raw = c;

    switch (c)
    {
    case '1':
        t.isWall = true;
        t.wallTexIndex = 0;
        break;
    case '2':
        t.isWall = true;
        t.wallTexIndex = 1;
        break;
    case '3':
        t.hasCeiling = true;
        t.floorTexIndex = 1;
        break;
    case '4':
        t.hasCeiling = false;
        t.floorTexIndex = 0;
        break;
    case '5':
        t.hasCeiling = false;
        t.floorTexIndex = 1;
        break;
    case 'L':
    case 'B':
        t.hasCeiling = false;
        break;
    default:
        break;
    }
    return t;
}
