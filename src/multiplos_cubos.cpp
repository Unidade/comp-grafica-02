/**
 * Múltiplos Cubos em Grid 3D
 * Demonstração de profundidade com matriz de cubos
 * Todos os cubos rotacionam juntos
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>

// Configurações da janela
int windowWidth = 1024;
int windowHeight = 768;
bool debugShowAxes = false;

// Grid parameters
const int GRID_SIZE = 3;      // 3x3x3 = 27 cubos
const float SPACING = 2.5f;   // Espaçamento entre cubos
const float CUBE_SIZE = 0.8f; // Tamanho de cada cubo

// Ângulo de rotação para animação
float gridRotation = 0.0f;

/**
 * Desenha eixos XYZ para referência
 */
void drawAxes(float length)
{
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // Eixo X (vermelho)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Eixo Y (verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Eixo Z (azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);

    glEnd();
    glLineWidth(1.0f);
}

/**
 * Desenha um cubo usando glBegin/glEnd
 */
void drawCube()
{
    float s = CUBE_SIZE / 2.0f; // Half size

    glBegin(GL_QUADS);

    // Face frontal
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);

    // Face traseira
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, -s, -s);

    // Face superior
    glVertex3f(-s, s, -s);
    glVertex3f(-s, s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, s, -s);

    // Face inferior
    glVertex3f(-s, -s, -s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s);
    glVertex3f(-s, -s, s);

    // Face direita
    glVertex3f(s, -s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, s, s);
    glVertex3f(s, -s, s);

    // Face esquerda
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, s, -s);

    glEnd();
}

/**
 * Inicialização do OpenGL
 */
void init()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Fundo azul escuro
    glEnable(GL_DEPTH_TEST);

    printf("\n=== MÚLTIPLOS CUBOS - Demonstração de Profundidade ===\n\n");
    printf("Grid: %dx%dx%d = %d cubos\n", GRID_SIZE, GRID_SIZE, GRID_SIZE,
           GRID_SIZE * GRID_SIZE * GRID_SIZE);
    printf("Espaçamento: %.1f unidades\n", SPACING);
    printf("Tamanho do cubo: %.1f unidades\n\n", CUBE_SIZE);
    printf("Os cubos rotacionam juntos demonstrando profundidade\n\n");
}

/**
 * Renderização da cena
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ==========================================
    // PROJECTION: Define projeção perspectiva
    // ==========================================
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)windowWidth / (float)windowHeight;
    gluPerspective(45.0f,   // FOV
                   aspect,  // Aspect ratio
                   0.1f,    // Near plane
                   100.0f); // Far plane

    // ==========================================
    // VIEW: Posição da câmera
    // ==========================================
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Câmera posicionada para ver toda a grade
    gluLookAt(10.0f, 8.0f, 18.0f, // Posição da câmera (eye)
              0.0f, 0.0f, 0.0f,   // Para onde olha (center)
              0.0f, 1.0f, 0.0f);  // Vetor "para cima" (up)

    // Eixos do mundo (opcional)
    if (debugShowAxes)
    {
        glPushMatrix();
        drawAxes(5.0f);
        glPopMatrix();
    }

    // ==========================================
    // MODEL: Grid de cubos com rotação
    // ==========================================
    glPushMatrix();

    // Rotação aplicada a TODOS os cubos juntos
    glRotatef(gridRotation, 0.0f, 1.0f, 0.0f);

    // Loop triplo para criar grid 3D
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            for (int z = 0; z < GRID_SIZE; z++)
            {
                glPushMatrix();

                // Calcular posição para centralizar o grid em (0,0,0)
                float px = (x - GRID_SIZE / 2.0f) * SPACING;
                float py = (y - GRID_SIZE / 2.0f) * SPACING;
                float pz = (z - GRID_SIZE / 2.0f) * SPACING;

                glTranslatef(px, py, pz);

                // Cor baseada na posição (gradiente)
                float r = (float)x / (GRID_SIZE - 1);
                float g = (float)y / (GRID_SIZE - 1);
                float b = (float)z / (GRID_SIZE - 1);
                glColor3f(r, g, b);

                drawCube();

                glPopMatrix();
            }
        }
    }

    glPopMatrix();

    glutSwapBuffers();
}

/**
 * Atualização da animação
 */
void update(int value)
{
    gridRotation += 0.5f;

    if (gridRotation > 360.0f)
        gridRotation -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

/**
 * Redimensionamento da janela
 */
void reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

/**
 * Tratamento de teclas
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;

    case 'r':
    case 'R':
        gridRotation = 0.0f;
        printf("Rotação resetada\n");
        break;

    case 'd':
    case 'D':
        debugShowAxes = !debugShowAxes;
        printf("Debug eixos: %s\n", debugShowAxes ? "ON" : "OFF");
        break;

    case '+':
    case '=':
        printf("Velocidade aumentada\n");
        break;

    case '-':
    case '_':
        printf("Velocidade diminuída\n");
        break;
    }
}

/**
 * Main
 */
int main(int argc, char **argv)
{
    // Inicializa GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Múltiplos Cubos - Grid 3D");

    // Inicializa OpenGL
    init();

    // Registra callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    printf("Controles:\n");
    printf("  R - Resetar rotação\n");
    printf("  D - Toggle eixos debug\n");
    printf("  ESC - Sair\n\n");

    // Loop principal
    glutMainLoop();

    return 0;
}
