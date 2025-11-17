/**
 * Jogo com Câmera FPS - Demonstração de Controles
 * - Classe Camera (free look FPS-style)
 * - Classe InputHandler (teclado + mouse)
 * - Cena simples com cubo 3D no centro
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>
#include <cstring>

// ============================================================================
// CLASSE CAMERA - Free Look FPS Style
// ============================================================================

class Camera
{
public:
    // Posição da câmera no espaço 3D
    float posX, posY, posZ;

    // Ângulos de rotação
    float yaw;   // Rotação horizontal (esquerda/direita)
    float pitch; // Rotação vertical (cima/baixo)

    // Configurações de movimento
    float moveSpeed;
    float mouseSensitivity;

    // Construtor - inicializa câmera em posição padrão
    Camera()
    {
        posX = 0.0f;
        posY = 2.0f;
        posZ = 8.0f;

        yaw = -90.0f;   // Olhando para frente (eixo -Z)
        pitch = 0.0f;   // Nível horizontal

        moveSpeed = 0.1f;
        mouseSensitivity = 0.1f;
    }

    // Aplica a transformação de view usando gluLookAt
    void applyView()
    {
        // Calcula vetor de direção baseado em yaw e pitch
        float dirX = cos(pitch * M_PI / 180.0f) * cos(yaw * M_PI / 180.0f);
        float dirY = sin(pitch * M_PI / 180.0f);
        float dirZ = cos(pitch * M_PI / 180.0f) * sin(yaw * M_PI / 180.0f);

        // Ponto para onde a câmera olha (posição + direção)
        float centerX = posX + dirX;
        float centerY = posY + dirY;
        float centerZ = posZ + dirZ;

        gluLookAt(posX, posY, posZ,         // Posição da câmera
                  centerX, centerY, centerZ, // Para onde olha
                  0.0f, 1.0f, 0.0f);         // Vetor "up"
    }

    // Move para frente (direção que a câmera está olhando)
    void moveForward(float delta)
    {
        float dirX = cos(pitch * M_PI / 180.0f) * cos(yaw * M_PI / 180.0f);
        float dirZ = cos(pitch * M_PI / 180.0f) * sin(yaw * M_PI / 180.0f);

        posX += dirX * moveSpeed * delta;
        posZ += dirZ * moveSpeed * delta;
    }

    // Move para trás
    void moveBackward(float delta)
    {
        float dirX = cos(pitch * M_PI / 180.0f) * cos(yaw * M_PI / 180.0f);
        float dirZ = cos(pitch * M_PI / 180.0f) * sin(yaw * M_PI / 180.0f);

        posX -= dirX * moveSpeed * delta;
        posZ -= dirZ * moveSpeed * delta;
    }

    // Move para esquerda (strafe)
    void moveLeft(float delta)
    {
        // Vetor perpendicular à direção (produto vetorial com up)
        float dirX = cos(yaw * M_PI / 180.0f);
        float dirZ = sin(yaw * M_PI / 180.0f);

        posX += dirZ * moveSpeed * delta;
        posZ -= dirX * moveSpeed * delta;
    }

    // Move para direita (strafe)
    void moveRight(float delta)
    {
        float dirX = cos(yaw * M_PI / 180.0f);
        float dirZ = sin(yaw * M_PI / 180.0f);

        posX -= dirZ * moveSpeed * delta;
        posZ += dirX * moveSpeed * delta;
    }

    // Rotaciona a câmera (mouse)
    void rotate(float deltaYaw, float deltaPitch)
    {
        yaw += deltaYaw * mouseSensitivity;
        pitch += deltaPitch * mouseSensitivity;

        // Limita pitch para evitar flip
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
};

// ============================================================================
// CLASSE INPUTHANDLER - Gerencia teclado e mouse
// ============================================================================

class InputHandler
{
public:
    bool keys[256];       // Estado de cada tecla
    int lastMouseX;       // Última posição X do mouse
    int lastMouseY;       // Última posição Y do mouse
    bool firstMouse;      // Primeira vez movendo o mouse?
    int centerX, centerY; // Centro da janela

    // Construtor - inicializa estados
    InputHandler()
    {
        memset(keys, false, sizeof(keys));
        lastMouseX = 0;
        lastMouseY = 0;
        firstMouse = true;
        centerX = 512;
        centerY = 384;
    }

    // Define centro da janela (para recentrar o mouse)
    void setWindowCenter(int x, int y)
    {
        centerX = x;
        centerY = y;
    }

    // Tecla pressionada
    void keyDown(unsigned char key)
    {
        keys[key] = true;
    }

    // Tecla solta
    void keyUp(unsigned char key)
    {
        keys[key] = false;
    }

    // Verifica se uma tecla está pressionada
    bool isKeyPressed(unsigned char key)
    {
        return keys[key];
    }

    // Movimento do mouse (passivo)
    void mouseMotion(int x, int y, Camera &camera)
    {
        if (firstMouse)
        {
            lastMouseX = x;
            lastMouseY = y;
            firstMouse = false;
            return;
        }

        // Calcula deslocamento do mouse
        int deltaX = x - lastMouseX;
        int deltaY = lastMouseY - y; // Invertido (Y cresce para baixo)

        lastMouseX = x;
        lastMouseY = y;

        // Rotaciona a câmera
        camera.rotate(deltaX, deltaY);

        // Recentra o mouse na janela (para movimento infinito)
        glutWarpPointer(centerX, centerY);
        lastMouseX = centerX;
        lastMouseY = centerY;
    }
};

// ============================================================================
// VARIÁVEIS GLOBAIS
// ============================================================================

Camera camera;
InputHandler input;

int windowWidth = 1024;
int windowHeight = 768;

// ============================================================================
// FUNÇÕES DE RENDERIZAÇÃO
// ============================================================================

/**
 * Desenha um cubo colorido (cada face uma cor diferente)
 */
void drawColoredCube(float size)
{
    float s = size / 2.0f;

    glBegin(GL_QUADS);

    // Face frontal (VERMELHA)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);

    // Face traseira (VERDE)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, -s, -s);

    // Face superior (AZUL)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glVertex3f(-s, s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, s, -s);

    // Face inferior (AMARELA)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s);
    glVertex3f(-s, -s, s);

    // Face direita (MAGENTA)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(s, -s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, s, s);
    glVertex3f(s, -s, s);

    // Face esquerda (CIANO)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, s, -s);

    glEnd();
}

/**
 * Desenha grid no chão para referência espacial
 */
void drawFloorGrid(float size, int divisions)
{
    float step = size / divisions;
    float halfSize = size / 2.0f;

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);

    // Linhas paralelas ao eixo Z
    for (int i = 0; i <= divisions; i++)
    {
        float x = -halfSize + i * step;
        glVertex3f(x, 0.0f, -halfSize);
        glVertex3f(x, 0.0f, halfSize);
    }

    // Linhas paralelas ao eixo X
    for (int i = 0; i <= divisions; i++)
    {
        float z = -halfSize + i * step;
        glVertex3f(-halfSize, 0.0f, z);
        glVertex3f(halfSize, 0.0f, z);
    }

    glEnd();
}

/**
 * Inicialização do OpenGL
 */
void init()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Fundo azul escuro
    glEnable(GL_DEPTH_TEST);

    // Esconde o cursor e centraliza
    glutSetCursor(GLUT_CURSOR_NONE);

    printf("\n=== JOGO COM CAMERA FPS ===\n\n");
    printf("Controles:\n");
    printf("  W, A, S, D - Movimento\n");
    printf("  Mouse      - Rotacao da camera\n");
    printf("  ESC        - Sair\n\n");
    printf("Camera iniciada em (%.1f, %.1f, %.1f)\n\n", camera.posX, camera.posY, camera.posZ);
}

/**
 * Renderização da cena
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ==========================================
    // PROJECTION
    // ==========================================
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)windowWidth / (float)windowHeight;
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    // ==========================================
    // VIEW - Aplica transformação da câmera
    // ==========================================
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.applyView();

    // ==========================================
    // MODEL - Desenha cena
    // ==========================================

    // Grid no chão
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f); // Abaixo do cubo
    drawFloorGrid(20.0f, 20);
    glPopMatrix();

    // Cubo no centro
    glPushMatrix();
    drawColoredCube(2.0f);
    glPopMatrix();

    glutSwapBuffers();
}

/**
 * Atualização contínua - processa input e atualiza câmera
 */
void update(int value)
{
    // Processa teclas pressionadas (movimento contínuo)
    if (input.isKeyPressed('w') || input.isKeyPressed('W'))
    {
        camera.moveForward(1.0f);
    }
    if (input.isKeyPressed('s') || input.isKeyPressed('S'))
    {
        camera.moveBackward(1.0f);
    }
    if (input.isKeyPressed('a') || input.isKeyPressed('A'))
    {
        camera.moveLeft(1.0f);
    }
    if (input.isKeyPressed('d') || input.isKeyPressed('D'))
    {
        camera.moveRight(1.0f);
    }

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

    // Atualiza centro da janela
    input.setWindowCenter(w / 2, h / 2);
}

/**
 * Callback - tecla pressionada
 */
void keyboardDown(unsigned char key, int x, int y)
{
    if (key == 27) // ESC
    {
        exit(0);
    }

    input.keyDown(key);
}

/**
 * Callback - tecla solta
 */
void keyboardUp(unsigned char key, int x, int y)
{
    input.keyUp(key);
}

/**
 * Callback - movimento passivo do mouse
 */
void passiveMotion(int x, int y)
{
    input.mouseMotion(x, y, camera);
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
    glutCreateWindow("Jogo - Camera FPS");

    // Inicializa OpenGL
    init();

    // Define centro da janela no InputHandler
    input.setWindowCenter(windowWidth / 2, windowHeight / 2);

    // Registra callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(16, update, 0);

    // Loop principal
    glutMainLoop();

    return 0;
}
