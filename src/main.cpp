//
//  main.cpp
//  Schoenberg
//
//  Created by work on 4/19/19.
//

#include <iostream>
#include <cmath>
#include <GLUT/glut.h>
#include <unistd.h>
#include <list>

int width = 500;
int height = 500;

using namespace std;

class Vertex
{
public:
    const double x, y;
    Vertex(double xx, double yy): x(xx), y(yy) {};
};

list<Vertex> vertices;
int nthVertex = 1;
int N = 4;
double speedBase = 100;
double speedMultiplier = 1.0;
//bool computed;

inline int sgn(double x)
{   return (x > 0) - (x < 0);   }

inline double u(double t, int a)
{
    return sgn(sgn(t - a) + 1);
}

inline double fnD(double t)
{
    double a = (3*t - 1) * (u(3*t, 1) - u(3*t, 2));
    double b = u(3*t, 2) - u(3*t, 4);
    double c = (5 - 3*t) * ( u(3*t, 4) - u(3*t, 5) );
    return a + b + c;
}

void computeVertices()
{

    double vX = 0;
    double vY = 0;
    vertices.push_back(Vertex(vX, vY));
    
    for (int i = 1; i <= pow(3, N); i++)
    {
        double x = 0, y = 0;
        double t = i / pow(3, N);
        int k = 0;
        for (; k <= floor(N / 2); k++)
        {
            x += fnD(t) / pow(2, (k + 1));
            t *= 3;
            t -= 2 * floor(t / 2);
            y += fnD(t) / pow(2, (k + 1));
            t *= 3;
            t -= 2 * floor(t / 2);
        }
        x += (i - 2 * (i / 2)) / pow(2, k);
        y += (i - 2 * (i / 2)) / pow(2, k);
        vX = x;
        vY = y;
        vertices.push_back(Vertex(vX, vY));
    }
}

void draw()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    long size = vertices.size();
    if (nthVertex <= size)
    {
        int i = 0;
        glBegin(GL_LINE_STRIP);
        for (auto it = vertices.begin(); i < nthVertex; it++, i++)
        {
            double totalPerc = 1.0 * (i+1) / size;
            double localPerc = 1 - 1.0 * (i+1) / nthVertex;
            double k = pow(0.1, localPerc);
            double r = k * 1;
            double g = k * (1-totalPerc);
            double b = k * totalPerc;
            glColor3d(r, g, b);
//            glLineWidth(2 * localPerc);
            glVertex2d(it->x, it->y);
        }
        glEnd();
        glFlush();
        usleep(1000 * speedBase / speedMultiplier);
        if (nthVertex <= size)
        {
            nthVertex++;
            glutPostRedisplay();
        }
    }
    else
    {
        usleep(1000 * 500);
        glBegin(GL_LINE_STRIP);
        int i = 0;
        for (auto it = vertices.begin(); it != vertices.end(); it++, i++)
        {
            double totalPerc = 1.0 * (i+1) / size;
            double k = 1;
            double r = k * 1;
            double g = k * (1-totalPerc);
            double b = k * totalPerc;
            
            glColor3d(r, g, b);
            glVertex2d(it->x, it->y);
        }
        glEnd();
        glFlush();
    }
}

// Callback for processing ASCII keyboard events.
void keyboard(GLubyte key, int, int)
{
    switch (key)
    {
        case 27: exit(0); break;
        case 'a': speedMultiplier *= 1.5; break;
        case 'd': speedMultiplier /= 1.5; break;
    }
    if (key >= '1' && key <= '9')
    {
        N = key - '1' + 1;
        nthVertex = 1;
        vertices.clear();
        computeVertices();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glutPostRedisplay();
    }
}

void reshape(int w, int h)
// Callback for processing reshape events.
{
    width = w;
    height = h;
    glViewport(0,0, (GLsizei)w, (GLsizei)h);
}

void mouse( int button, int state, int, int )
// Routine for processing mouse events.
{
    if ( button == GLUT_LEFT_BUTTON )
    {
        N = max(N-1, 1);
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        N = N+1;
    }
    nthVertex = 1;
    vertices.clear();
    computeVertices();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
}

int main(int argc, const char * argv[]) {
    int _argc = 1;
    char* _argv[1] = {(char*)""};
    glutInit( &_argc, _argv );
    
    glutInitDisplayMode(GLUT_RGB);
    
    // Set initial window size, position, and title.
    glutInitWindowSize( width, height );
    glutInitWindowPosition( 0, 0 );
    glutCreateWindow("Schoenberg Curve");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPointSize(1.0f);
    glEnable(GL_LINE_SMOOTH);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glViewport(0, 0, width, height);
    
    
    // Set the callbacks for the screen window.
    glutDisplayFunc( draw );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    
    
    computeVertices();
//    usleep(1000 * 1000 * 10);
    glutMainLoop();
    return 0;
}



