//
//  main.c
//  dotvec 1.0
//
//  Rotates dotvector object on the screen around three-axis
//
//
//  Created by Mangobanaani on 11/5/13.
//  Copyright (c) 2013 Mangobanaani. All rights reserved.
//

#ifdef __APPLE__            // includes for OS X platform
 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
 #include <GLUT/glut.h>
#else
 #ifdef _WIN32               // includes for Win32 platform
  #include <windows.h>
 #endif
  #include <GL/gl.h>         // win32/linux platform libraries
  #include <GL/glu.h>
  #include <GL/glut.h>
#endif
#include <stdlib.h>         // common C libraries used across platforms
#include <math.h>

#define count 217           // number of points
extern long point[count][3];// point data from pointdata.c

const double XSize = 800, YSize = 600;  // window size
const int refreshMills = 50;            // refresh interval in milliseconds
double angle=0.0;                       // rotation angle, initially zero
const int objectWidth=1900;             // object width in 'object-space'
const int objectHeight=500;             // object height in 'object-space'
double sintable[360];                   // old-fashioned lookup table for sine-values
double costable[360];                   // lookup table for cosine values

void centerObject(void){ // centers the object data to world origo
    int x;               // obj is defined where upper left corner is (x,y) 0,0
    long centerX=objectWidth/2;         // and we want to have origo at middle of obj
    long centerY=objectHeight/2;        // so it will rotate around it's center
    for(x=0;x<count;x++){               // so coordinates are translated
        long xtmp=point[x][0];
        long ytmp=point[x][1];
        point[x][0]=xtmp-centerX;
        point[x][1]=ytmp-centerY;
    }
    
}

void putPixel(int x,int y){             // 2D putpixel wrapper
    glVertex2d(x, y);
}

void Timer(int value) {
    glutPostRedisplay();                // repaint request
    glutTimerFunc(refreshMills, Timer, 0); // next timer call in refreshMillis
}

void roll(double x_deg,double y_deg,double z_deg){  // rotate along x,y,z axis n deg
 int curr=0; // current point counter
 
 while(curr<count) // go through all points
 {
     long x=point[curr][0]; // point coordinates into variables
     long y=point[curr][1];
     long z=point[curr][2];
 
     double vx=(cos(z_deg)*x+sin(z_deg)*y);
     double vy=(cos(z_deg)*y+sin(z_deg)*x);
     double vz=(cos(x_deg)*z+sin(x_deg)*vy);
 
     double lx=(cos(y_deg)*vx-sin(y_deg)*vz); // and projection
     double ly=(cos(x_deg)*vy-sin(x_deg)*z);
     double lz=(cos(y_deg)*vz-sin(y_deg)*vx)/512.0; // focal correction (perspective)
 
     int rx=(lx/(lz+8))+XSize/2;    // fit to screen
     int ry=(ly/(lz+8))+YSize/2;
     putPixel(rx,ry);               // write to backbuf
     curr++;                        // and next dot
 }
}

void display() {    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);           // Clear the color buffer (background)
    
    glMatrixMode (GL_PROJECTION);           // projection matrix
    glLoadIdentity ();                      // reset the matrix
    glOrtho (0, XSize, YSize, 0, 0, 1);     // no perspective
    glDisable(GL_DEPTH_TEST);               // depth buffer off, we're fine with 2D
    glTranslatef(0.375, 0.375, 0);          // some gl displacement trickery
    
    glBegin(GL_POINTS);                     // start of real drawing
    roll(angle,angle,angle);
    glEnd();
    
    glutSwapBuffers();                      // render (copy backbuffer to display)
    angle+=0.05;                            // increase rotation angle
    if(angle>360.0)angle=0.0;               // if we rotated 360 degrees, zero us back at start
}

int main(int argc, char** argv) {
    centerObject();                         // reset object data to origo at middle screen
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE);       // double buffering
    glutCreateWindow("Dotvec 1.0b by PS-L");// make a window
    glutInitWindowSize(XSize, YSize);       // initial size
    glutInitWindowPosition(150, 150);       // initial top-left corner
    glutDisplayFunc(display);               // regisger callback handler
    glutTimerFunc(0, Timer, 0);             // timer call
    glutMainLoop();                         // start event processing (display)
    return 0;                               // exit with no error
}


