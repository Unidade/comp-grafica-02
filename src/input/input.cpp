#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

#include "input/input.h"
#include "input/keystate.h"
#include "core/window.h"

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
    case 'w': case 'W': keyW = true; break;
    case 's': case 'S': keyS = true; break;
    case 'a': case 'A': keyA = true; break;
    case 'd': case 'D': keyD = true; break;

    case 27: // ESC
        std::exit(0);
        break;
    }
}

void keyboardUp(unsigned char key, int, int)
{
    switch (key)
    {
    case 'w': case 'W': keyW = false; break;
    case 's': case 'S': keyS = false; break;
    case 'a': case 'A': keyA = false; break;
    case 'd': case 'D': keyD = false; break;
    }

    if ((key == 13 || key == '\r') && (glutGetModifiers() & GLUT_ACTIVE_ALT))
    {
        altFullScreen();
    }
}


