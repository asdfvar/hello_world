#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>

void display( void )
{

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   //Initialise VBO - do only once, at start of program
   //Create a variable to hold the VBO identifier
   GLuint triangleVBO;

   //Vertices of a triangle (counter-clockwise winding)
   float data[] = {0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0};

   //Create a new VBO and use the variable id to store the VBO id
   glGenBuffers(1, &triangleVBO);

   //Make the new VBO active
   glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

   //Upload vertex data to the video device
   glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

   //Make the new VBO active. Repeat here incase changed since initialisation
   glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

   //Draw Triangle from VBO - do each time window, view point or data changes
   //Establish its 3 coordinates per vertex with zero stride in this array; necessary here
   glVertexPointer(3, GL_FLOAT, 0, NULL);

   //Establish array contains vertices (not normals, colours, texture coords etc)
   glEnableClientState(GL_VERTEX_ARRAY);

   //Actually draw the triangle, giving the number of vertices provided
   glDrawArrays(GL_TRIANGLES, 0, sizeof(data) / sizeof(float) / 3);

   //Force display to be drawn now
   glFlush();

   glutSwapBuffers();

}

void idle( void )
{
}

int main( int argc, char** argv)
{

   const int windowsizex = 600;
   const int windowsizey = 600;

   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutInitWindowSize( windowsizex, windowsizey );
   glutInitWindowPosition( 50, 50 );
   glutCreateWindow( "vbo example" );

   glutDisplayFunc(       display      );
   glutIdleFunc(          idle         );

   glutMainLoop();

   return 0;
}
