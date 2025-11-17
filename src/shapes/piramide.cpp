/**
 * Demonstração: Pirâmide 3D - Pipeline MVP
 *
 * Este programa demonstra os conceitos básicos de transformações 3D usando
 * as três matrizes fundamentais do pipeline gráfico.
 *
 * Matrizes MVP:
 * - PROJECTION: Define como o mundo é projetado na tela
 * - VIEW: Define onde o observador está
 * - MODEL: Move/rotaciona/escalona o objeto (model = translate * rotate * scale)
 *
 * Compile com:
 * g++ src/piramide.cpp -o piramide -lGL -lGLU -lglut
 *
 * Execute:
 * ./piramide
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>

// Configurações da janela
int windowWidth = 1024;
int windowHeight = 768;

// Ângulo de rotação para animação
float pyramidRotation = 0.0f;

/**
 * Desenha uma pirâmide usando glBegin/glEnd
 */
void drawPyramid() {
    glBegin(GL_TRIANGLES);

    // Face frontal (vermelha)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);

    // Face direita (verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    // Face traseira (azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Face esquerda (amarela)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  0.5f,  0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    glEnd();

    // Base (magenta)
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();
}

/**
 * Inicialização do OpenGL
 */
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    printf("\n=== DEMONSTRAÇÃO PIRÂMIDE 3D - Pipeline MVP ===\n\n");
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
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ==========================================
    // PROJECTION: Define como o mundo é projetado na tela
    // ==========================================
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)windowWidth / (float)windowHeight;
    gluPerspective(45.0f,    // FOV (campo de visão)
                   aspect,    // Aspect ratio
                   0.1f,      // Near plane
                   100.0f);   // Far plane

    // ==========================================
    // VIEW: Define onde o observador está
    // ==========================================
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0f, 2.0f, 8.0f,   // Posição da câmera (eye)
              0.0f, 0.0f, 0.0f,   // Para onde olha (center)
              0.0f, 1.0f, 0.0f);  // Vetor "para cima" (up)

    // ==========================================
    // MODEL: Move/rotaciona/escalona o objeto
    // model = translate * rotate * scale
    // ==========================================
    glPushMatrix();

    // Transformações do objeto (aplicadas da direita para esquerda)
    glRotatef(pyramidRotation, 1.0f, 0.0f, 0.0f);  // Rotação no eixo X
    glScalef(1.2f, 1.2f, 1.2f);                    // Escala 1.2x

    drawPyramid();

    glPopMatrix();

    glutSwapBuffers();
}

/**
 * Atualização da animação
 */
void update(int value) {
    pyramidRotation += 1.5f;

    if (pyramidRotation > 360.0f) pyramidRotation -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // ~60 FPS
}

/**
 * Redimensionamento da janela
 */
void reshape(int w, int h) {
    if (h == 0) h = 1;
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

/**
 * Tratamento de teclas
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC
            exit(0);
            break;
        case 'r':
        case 'R':
            pyramidRotation = 0.0f;
            printf("Rotação resetada\n");
            break;
    }
}

/**
 * Main
 */
int main(int argc, char** argv) {
    // Inicializa GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pirâmide 3D");

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
