#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>

static float posx = 0.0f;
static float posy = 0.0f;
static bool trigger = false;

void display (void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLineWidth (0.2f);
   glBegin (GL_POLYGON);
   glColor3f (1.0f, 1.0f, 1.0f);
   glVertex2f (posx - 0.02f, posy - 0.02f);
   glVertex2f (posx + 0.02f, posy - 0.02f);
   glVertex2f (posx + 0.02f, posy + 0.02f);
   glVertex2f (posx - 0.02f, posy + 0.02f);
   glVertex2f (posx - 0.02f, posy - 0.02f);
   glEnd ();

   //Force display to be drawn now
   glFlush ();

   glutSwapBuffers ();
}

void idle (void)
{
}

void special (int key, int x, int y)
{
   if (!trigger) {
      // <: 100
      if (key == 100) posx -= 0.02f;

      // ^: 101
      if (key == 101) posy += 0.02f;

      // >: 102
      if (key == 102) posx += 0.02f;

      // v: 103
      if (key == 103) posy -= 0.02f;

      trigger = true;

      // Signal the callback to the display now that a change has occurred
      glutPostRedisplay ();
   }
}

void specialUp (int key, int x, int y)
{
   trigger = false;
}

int main (int argc, char** argv)
{

   const int windowsizex = 600;
   const int windowsizey = 600;

   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (windowsizex, windowsizey);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("example");

   glutSpecialUpFunc (specialUp);
   glutSpecialFunc (special);

   glutDisplayFunc (display);
   glutIdleFunc (idle);

   glutMainLoop ();

   return 0;
}
