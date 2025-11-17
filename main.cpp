#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

int larguraJanela = 800;
int alturaJanela = 600;

float anguloCubo = 0.0f;
float fov = 45.0f;

struct Vertex
{
  float x;
  float y;
  float z;
};

// 8 vértices de um cubo centrado na origem, lado 1.0
const Vertex cubePositions[8] = {
    {-0.5f, -0.5f, -0.5f}, // 0
    {0.5f, -0.5f, -0.5f},  // 1
    {0.5f, 0.5f, -0.5f},   // 2
    {-0.5f, 0.5f, -0.5f},  // 3
    {-0.5f, -0.5f, 0.5f},  // 4
    {0.5f, -0.5f, 0.5f},   // 5
    {0.5f, 0.5f, 0.5f},    // 6
    {-0.5f, 0.5f, 0.5f}    // 7
};

// 12 triângulos (36 índices) – 6 faces * 2 triângulos
const unsigned int cubeIndices[36] = {
    // Frente (z positivo): 4,5,6,7
    4, 5, 6, 4, 6, 7,
    // Trás (z negativo): 1,0,3,2
    1, 0, 3, 1, 3, 2,
    // Esquerda (x negativo): 0,4,7,3
    0, 4, 7, 0, 7, 3,
    // Direita (x positivo): 5,1,2,6
    5, 1, 2, 5, 2, 6,
    // Topo (y positivo): 3,7,6,2
    3, 7, 6, 3, 6, 2,
    // Base (y negativo): 0,1,5,4
    0, 1, 5, 0, 5, 4};

void desenharCuboManual(float tamanho)
{
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < 36; ++i)
  {
    const Vertex &v = cubePositions[cubeIndices[i]];
    glVertex3f(v.x * tamanho, v.y * tamanho, v.z * tamanho);
  }
  glEnd();
}

// ---------------------- Inicialização -----------------------------

void inicializar()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

// ---------------------- Projeção ---------------------------------

void configurarProjecao()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float aspecto = (float)larguraJanela / (float)alturaJanela;

  gluPerspective(fov, aspecto, 0.1f, 100.0f);
}

// ---------------------- Desenho ----------------------------------

void exibir()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  configurarProjecao();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0,  // posição da câmera (eye)
            0.0, 0.0, 0.0,  // para onde a câmera olha (center)
            0.0, 1.0, 0.0); // vetor "para cima" (up)

  glPushMatrix();
  glRotatef(anguloCubo, 0.0f, 0.0f, 1.0f);
  glColor3f(1.0f, 0.0f, 0.0f);
  desenharCuboManual(1.0f);
  glPopMatrix();

  glutSwapBuffers();
}

// ---------------------- Atualização ------------------------------

void atualizar(int valor)
{
  // aumenta o ângulo para girar o cubo
  anguloCubo += 0.5f;
  if (anguloCubo > 360.0f)
  {
    anguloCubo -= 360.0f;
  }

  glutPostRedisplay();             // pede para redesenhar
  glutTimerFunc(16, atualizar, 0); // ~60 FPS
}

// ---------------------- Redimensionamento ------------------------

void redimensionar(int w, int h)
{
  if (h == 0)
    h = 1;

  larguraJanela = w;
  alturaJanela = h;

  glViewport(0, 0, w, h);
}

// ---------------------- main -------------------------------------

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(larguraJanela, alturaJanela);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Exercicio 01 - Cubo Rotacionando");

  inicializar();

  glutDisplayFunc(exibir);
  glutReshapeFunc(redimensionar);
  glutTimerFunc(16, atualizar, 0);

  glutMainLoop();
  return 0;
}
