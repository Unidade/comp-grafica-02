#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include "drawlevel.h"
#include "scene.h"
#include "utils/levelmetrics.h"
#include "texture.h"

extern GLuint texLava;
extern GLuint texSangue;
extern GLuint progLava;
extern GLuint progSangue;
extern float tempoEsfera;

static GLuint texFloor[2];
static GLuint texWall[2];
static GLuint texCeiling;

static const float TILE = 4.0f;
static const float WALL_H = 4.0f;
static const float EPS_Y = 0.001f;

void initLevelTextures()
{
    texFloor[0] = carregaTextura("assets/181.png");
    texFloor[1] = carregaTextura("assets/185.png");
    texWall[0] = carregaTextura("assets/091.png");
    texWall[1] = carregaTextura("assets/082.png");
    texCeiling = carregaTextura("assets/150.png");
}

static void desenhaQuadChao(float x, float z, float tile, float tilesUV)
{
    float half = tile * 0.5f;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, EPS_Y, z + half);
    glTexCoord2f(tilesUV, 0.0f);
    glVertex3f(x + half, EPS_Y, z + half);
    glTexCoord2f(tilesUV, tilesUV);
    glVertex3f(x + half, EPS_Y, z - half);
    glTexCoord2f(0.0f, tilesUV);
    glVertex3f(x - half, EPS_Y, z - half);
    glEnd();
}

static void desenhaQuadTeto(float x, float z)
{
    float half = TILE * 0.5f;
    glUseProgram(0);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texCeiling);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, WALL_H - EPS_Y, z - half);
    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(x + half, WALL_H - EPS_Y, z - half);
    glTexCoord2f(2.0f, 2.0f);
    glVertex3f(x + half, WALL_H - EPS_Y, z + half);
    glTexCoord2f(0.0f, 2.0f);
    glVertex3f(x - half, WALL_H - EPS_Y, z + half);
    glEnd();
}

static void desenhaTileChao(float x, float z, int texIndex)
{
    glUseProgram(0);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texFloor[texIndex]);
    desenhaQuadChao(x, z, TILE, 2.0f);
}

static void desenhaParede(float x, float z, int texIndex)
{
    float half = TILE * 0.5f;
    glUseProgram(0);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texWall[texIndex]);

    float tilesX = 1.0f;
    float tilesY = 2.0f;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, 0.0f, z + half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x + half, 0.0f, z + half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x + half, WALL_H, z + half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x - half, WALL_H, z + half);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + half, 0.0f, z - half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x - half, 0.0f, z - half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x - half, WALL_H, z - half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x + half, WALL_H, z - half);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + half, 0.0f, z + half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x + half, 0.0f, z - half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x + half, WALL_H, z - half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x + half, WALL_H, z + half);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, 0.0f, z - half);
    glTexCoord2f(tilesX, 0.0f);
    glVertex3f(x - half, 0.0f, z + half);
    glTexCoord2f(tilesX, tilesY);
    glVertex3f(x - half, WALL_H, z + half);
    glTexCoord2f(0.0f, tilesY);
    glVertex3f(x - half, WALL_H, z - half);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, WALL_H, z + half);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + half, WALL_H, z + half);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + half, WALL_H, z - half);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x - half, WALL_H, z - half);
    glEnd();
}

static void desenhaTileLava(float x, float z)
{
    glUseProgram(progLava);
    glUniform1f(glGetUniformLocation(progLava, "uTime"), tempoEsfera);
    glUniform1f(glGetUniformLocation(progLava, "uStrength"), 1.0f);
    glUniform2f(glGetUniformLocation(progLava, "uScroll"), 0.1f, 0.0f);
    glUniform1f(glGetUniformLocation(progLava, "uHeat"), 0.6f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texLava);
    glUniform1i(glGetUniformLocation(progLava, "uTexture"), 0);
    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);
    glUseProgram(0);
}

static void desenhaTileSangue(float x, float z)
{
    glUseProgram(progSangue);
    glUniform1f(glGetUniformLocation(progSangue, "uTime"), tempoEsfera);
    glUniform1f(glGetUniformLocation(progSangue, "uStrength"), 1.0f);
    glUniform2f(glGetUniformLocation(progSangue, "uSpeed"), 2.0f, 1.3f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texSangue);
    glUniform1i(glGetUniformLocation(progSangue, "uTexture"), 0);
    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);
    glUseProgram(0);
}

void drawLevel(const MapLoader &map)
{
    const auto &data = map.data();
    int H = map.getHeight();
    LevelMetrics m = LevelMetrics::fromMap(map, TILE);

    for (int z = 0; z < H; z++)
    {
        for (int x = 0; x < (int)data[z].size(); x++)
        {
            float wx, wz;
            m.tileCenter(x, z, wx, wz);

            Tile tile = map.getTile(x, z);
            char c = tile.raw;

            if (c == '1' || c == '2')
            {
                desenhaParede(wx, wz, tile.wallTexIndex);
            }
            else if (c == '0' || c == '9' || c == '3' || c == '4' || c == '5')
            {
                desenhaTileChao(wx, wz, tile.floorTexIndex);
                if (tile.hasCeiling)
                {
                    desenhaQuadTeto(wx, wz);
                }
            }
            else if (c == 'L')
            {
                desenhaTileLava(wx, wz);
            }
            else if (c == 'B')
            {
                desenhaTileSangue(wx, wz);
            }
        }
    }
}
