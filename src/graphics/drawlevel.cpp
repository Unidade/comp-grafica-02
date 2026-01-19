#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include "graphics/drawlevel.h"
#include "core/camera.h"
#include "level/levelmetrics.h"
#include <cstdio>

extern GLuint texParede;
extern GLuint texParedeInterna;
extern GLuint texLava;
extern GLuint texSangue;
extern GLuint texChao;
extern GLuint texChaoInterno;
extern GLuint texTeto;

extern GLuint progLava;
extern GLuint progSangue;

// Controle de tempo
extern float tempo;

// Config do grid
static const float TILE = 4.0f;      // tamanho do tile no mundo (ajuste)
static const float CEILING_H = 4.0f; // altura do teto
static const float WALL_H = 4.0f;    // altura da parede
static const float EPS_Y = 0.001f;   // evita z-fighting

static const GLfloat kAmbientOutdoor[] = {0.45f, 0.30f, 0.25f, 1.0f}; // quente (seu atual)
static const GLfloat kAmbientIndoor[] = {0.12f, 0.12f, 0.18f, 1.0f};  // frio/azulado

static void vinculaTextura0(GLuint tex)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
}

static float hash01(float x)
{
    float s = sinf(x * 12.9898f) * 43758.5453f;
    return s - floorf(s);
}

static float flickerFluorescente(float t)
{
    const float rate = 4.0f; // era 9.0
    float block = floorf(t * rate);
    float r = hash01(block);

    if (r < 0.22f) // era 0.12
    {
        float phase = t * rate - block;

        // apagão mais longo
        if (phase > 0.35f && phase < 0.55f)
            return 0.12f; // quase apaga

        // as vezes um segundo tranco
        if (r < 0.06f && phase > 0.65f && phase < 0.78f)
            return 0.40f;
    }

    return 0.96f + 0.04f * sinf(t * 5.0f);
}

static void configuraLampadaInternaEm(float x, float z, float intensidade)
{
    // posição da lâmpada (pontual)
    GLfloat pos[] = {x, CEILING_H - 0.05f, z, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, pos);

    GLfloat diff[] = {
        1.20f * intensidade,
        1.22f * intensidade,
        1.28f * intensidade,
        1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);

    GLfloat amb[] = {
        1.10f * intensidade,
        1.10f * intensidade,
        1.12f * intensidade,
        1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
}

static void iniciaInterno(float wx, float wz)
{
    // sol NÃO entra
    glDisable(GL_LIGHT0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, kAmbientIndoor); // esfria o ambiente

    // lâmpada fria entra
    glEnable(GL_LIGHT1);

    float f = flickerFluorescente(tempo);
    float intensidade = 1.2f * f;

    configuraLampadaInternaEm(wx, wz, intensidade);
}

static void finalizaInterno()
{
    glDisable(GL_LIGHT1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, kAmbientOutdoor);

    glEnable(GL_LIGHT0);
}

static void desenhaQuadTeto(float x, float z, float tile, float tilesUV)
{
    float half = tile * 0.5f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f); // NORMAL DO TETO

    // note a ordem invertida
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - half, CEILING_H, z - half);
    glTexCoord2f(tilesUV, 0.0f);
    glVertex3f(x + half, CEILING_H, z - half);
    glTexCoord2f(tilesUV, tilesUV);
    glVertex3f(x + half, CEILING_H, z + half);
    glTexCoord2f(0.0f, tilesUV);
    glVertex3f(x - half, CEILING_H, z + half);
    glEnd();
}

static void desenhaQuadChao(float x, float z, float tile, float tilesUV)
{
    float half = tile * 0.5f;

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f); // NORMAL DO CHÃO

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

static void desenhaTileChao(float x, float z, GLuint texChaoX, bool temTeto)
{
    glUseProgram(0); // sem shader
    glColor3f(1, 1, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texChaoX);

    // chão
    desenhaQuadChao(x, z, TILE, 2.0f);

    // teto
    if (temTeto)
    {
        glBindTexture(GL_TEXTURE_2D, texTeto);
        desenhaQuadTeto(x, z, TILE, 2.0f);
    }
}

static bool ehInterno(char c)
{
    return (c == '2' || c == '3');
}

static void aplicaIluminacaoPorVizinho(char nb, float wx, float wz, float nx, float nz)
{
    if (ehInterno(nb))
    {
        // Se o vizinho é indoor, a face da parede está voltada para dentro.
        // Posicionamos a lâmpada levemente deslocada para o centro do vizinho.
        float nwx = wx + nx * TILE;
        float nwz = wz + nz * TILE;
        iniciaInterno(nwx, nwz);
    }
    else
    {
        // Se o vizinho é outdoor, a face da parede deve receber a luz do sol.
        finalizaInterno();
    }
}

static void desenhaParede(float wx, float wz, GLuint texParedeX, char n, char s, char e, char w, char self)
{
    float half = TILE * 0.5f;

    float tilesX = 1.0f;
    float tilesY = 2.0f;

    // Se a própria parede é '2' (Indoor), o topo e a base geralmente são internos/escuros.
    // Mas as faces laterais dependem do vizinho.
    
    // Frente (z+) -> Vizinho SUL
    aplicaIluminacaoPorVizinho(s, wx, wz, 0, 1);
    glBindTexture(GL_TEXTURE_2D, texParedeX);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(wx - half, 0.0f, wz + half);
    glTexCoord2f(tilesX, 0.0f); glVertex3f(wx + half, 0.0f, wz + half);
    glTexCoord2f(tilesX, tilesY); glVertex3f(wx + half, WALL_H, wz + half);
    glTexCoord2f(0.0f, tilesY); glVertex3f(wx - half, WALL_H, wz + half);
    glEnd();

    // Trás (z-) -> Vizinho NORTE
    aplicaIluminacaoPorVizinho(n, wx, wz, 0, -1);
    glBindTexture(GL_TEXTURE_2D, texParedeX);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(wx + half, 0.0f, wz - half);
    glTexCoord2f(tilesX, 0.0f); glVertex3f(wx - half, 0.0f, wz - half);
    glTexCoord2f(tilesX, tilesY); glVertex3f(wx - half, WALL_H, wz - half);
    glTexCoord2f(0.0f, tilesY); glVertex3f(wx + half, WALL_H, wz - half);
    glEnd();

    // Direita (x+) -> Vizinho LESTE
    aplicaIluminacaoPorVizinho(e, wx, wz, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texParedeX);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(wx + half, 0.0f, wz + half);
    glTexCoord2f(tilesX, 0.0f); glVertex3f(wx + half, 0.0f, wz - half);
    glTexCoord2f(tilesX, tilesY); glVertex3f(wx + half, WALL_H, wz - half);
    glTexCoord2f(0.0f, tilesY); glVertex3f(wx + half, WALL_H, wz + half);
    glEnd();

    // Esquerda (x-) -> Vizinho OESTE
    aplicaIluminacaoPorVizinho(w, wx, wz, -1, 0);
    glBindTexture(GL_TEXTURE_2D, texParedeX);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(wx - half, 0.0f, wz - half);
    glTexCoord2f(tilesX, 0.0f); glVertex3f(wx - half, 0.0f, wz + half);
    glTexCoord2f(tilesX, tilesY); glVertex3f(wx - half, WALL_H, wz + half);
    glTexCoord2f(0.0f, tilesY); glVertex3f(wx - half, WALL_H, wz - half);
    glEnd();

    // Topo - Segue o tipo da própria parede
    if (ehInterno(self)) iniciaInterno(wx, wz); else finalizaInterno();
    glBindTexture(GL_TEXTURE_2D, texParedeX);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(wx - half, WALL_H, wz + half);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(wx + half, WALL_H, wz + half);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(wx + half, WALL_H, wz - half);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(wx - half, WALL_H, wz - half);
    glEnd();

    // Reset para o padrão outdoor ao sair
    finalizaInterno();
}

static void desenhaTileLava(float x, float z)
{
    glUseProgram(progLava);

    GLint locTime = glGetUniformLocation(progLava, "uTime");
    GLint locStr = glGetUniformLocation(progLava, "uStrength");
    GLint locScr = glGetUniformLocation(progLava, "uScroll");
    GLint locHeat = glGetUniformLocation(progLava, "uHeat");
    GLint locTex = glGetUniformLocation(progLava, "uTexture");

    glUniform1f(locTime, tempo);
    glUniform1f(locStr, 1.0f);
    glUniform2f(locScr, 0.1f, 0.0f);
    glUniform1f(locHeat, 0.6f);

    vinculaTextura0(texLava);
    glUniform1i(locTex, 0);

    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);

    glUseProgram(0);
}

static void desenhaTileSangue(float x, float z)
{
    glUseProgram(progSangue);

    GLint locTime = glGetUniformLocation(progSangue, "uTime");
    GLint locStr = glGetUniformLocation(progSangue, "uStrength");
    GLint locSpd = glGetUniformLocation(progSangue, "uSpeed");
    GLint locTex = glGetUniformLocation(progSangue, "uTexture");

    glUniform1f(locTime, tempo);
    glUniform1f(locStr, 1.0f);
    glUniform2f(locSpd, 2.0f, 1.3f);

    vinculaTextura0(texSangue);
    glUniform1i(locTex, 0);

    glColor3f(1, 1, 1);
    desenhaQuadChao(x, z, TILE, 2.0f);

    glUseProgram(0);
}

void desenhaNivel(const MapLoader &map)
{
    const auto &data = map.data();
    int H = map.getHeight();

    LevelMetrics m = LevelMetrics::fromMap(map, TILE);

    for (int z = 0; z < H; z++)
    {
        int W = (int)data[z].size();
        for (int x = 0; x < W; x++)
        {
            float wx, wz;
            m.tileCenter(x, z, wx, wz);

            char c = data[z][x];

            // Vizinhos para iluminação de parede
            char n = (z > 0) ? data[z - 1][x] : '0';
            char s = (z < H - 1) ? data[z + 1][x] : '0';
            char w = (x > 0) ? data[z][x - 1] : '0';
            char e = (x < W - 1) ? data[z][x + 1] : '0';

            if (c == '0')
                desenhaTileChao(wx, wz, texChao, false);
            else if (c == '3')
            {
                iniciaInterno(wx, wz);
                desenhaTileChao(wx, wz, texChaoInterno, true);
                finalizaInterno();
            }
            else if (c == '1' || c == '2')
            {
                GLuint tex = (c == '1') ? texParede : texParedeInterna;
                desenhaParede(wx, wz, tex, n, s, e, w, c);
            }
            else if (c == 'L')
                desenhaTileLava(wx, wz);
            else if (c == 'B')
                desenhaTileSangue(wx, wz);
        }
    }
}
