/**
 * Matrizes MVP:
 * - PROJECTION: Define como o mundo é projetado na tela
 * - VIEW: Define onde o observador está
 * - MODEL: Move/rotaciona/escalona o objeto (model = translate * rotate * scale)
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>

// Configurações da janela
int windowWidth = 1024;
int windowHeight = 768;
bool debugShowAxes = true; // Mostrar eixos XYZ

// Ângulo de rotação para animação
float cubeRotation = 0.0f;

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
    glBegin(GL_QUADS);

    // Face frontal (vermelha)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Face traseira (verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Face superior (azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Face inferior (amarela)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Face direita (ciano)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Face esquerda (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();
}

/**
 * Inicialização do OpenGL
 */
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    printf("\n=== DEMONSTRAÇÃO CUBO 3D - Pipeline MVP ===\n\n");
    printf("Este programa demonstra as três matrizes fundamentais:\n\n");
    printf("1. PROJECTION: Define como o mundo é projetado na tela\n");
    printf("   - gluPerspective(fov, aspect, near, far)\n\n");
    printf("2. VIEW: Define onde o observador está\n");
    printf("   - gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)\n\n");
    printf("3. MODEL: Move/rotaciona/escalona o objeto\n");
    printf("   - model = translate * rotate * scale\n");
    printf("   - glTranslatef(x, y, z)\n");
    printf("   - glRotatef(angle, x, y, z)\n");
    printf("   - glScalef(x, y, z)\n\n");
}

/**
 * Renderização da cena
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ==========================================
    // PROJECTION: Define como o mundo é projetado na tela
    // ==========================================
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)windowWidth / (float)windowHeight;
    gluPerspective(45.0f,   // FOV (campo de visão)
                   aspect,  // Aspect ratio
                   0.1f,    // Near plane
                   100.0f); // Far plane

    // ==========================================
    // VIEW: Define onde o observador está
    // ==========================================
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 8.0f,  // Posição da câmera (eye)
              0.0f, 0.0f, 0.0f,  // Para onde olha (center)
              0.0f, 1.0f, 0.0f); // Vetor "para cima" (up)

    // --- Eixos do mundo (no centro, sem transformações de modelo) ---
    if (debugShowAxes)
    {
        glPushMatrix();
        drawAxes(2.0f); // eixos do mundo com comprimento 2
        glPopMatrix();
    }

    // ==========================================
    // MODEL: Move/rotaciona/escalona o objeto
    // model = translate * rotate * scale
    // ==========================================
    glPushMatrix();

    // Transformações do objeto (aplicadas da direita para esquerda)
    glRotatef(cubeRotation, 0.0f, 1.0f, 0.0f); // Rotação no eixo Y
    glScalef(1.0f, 1.0f, 1.0f);                // Escala uniforme

    drawCube();

    glPopMatrix();

    glutSwapBuffers();
}

/**
 * Atualização da animação
 */
void update(int value)
{
    cubeRotation += 1.0f;

    if (cubeRotation > 360.0f)
        cubeRotation -= 360.0f;

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
        cubeRotation = 0.0f;
        printf("Rotação resetada\n");
        break;

    case 'd':
    case 'D':
        debugShowAxes = !debugShowAxes;
        printf("Debug eixos: %s\n", debugShowAxes ? "ON" : "OFF");
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
    glutCreateWindow("Cubo 3D");

    // Inicializa OpenGL
    init();

    // Registra callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    printf("Controles:\n");
    printf("  R - Resetar rotação\n");
    printf("  ESC - Sair\n\n");

    // Loop principal
    glutMainLoop();

    return 0;
}
