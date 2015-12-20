//
//  main.cpp
//  opengl hw1
//
//  Created by Mingyuan Wang on 3/11/15.
//  Copyright (c) 2015 Mingyuan Wang. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cfloat>
#include <cmath>
using namespace std;

#define MAX_MATERIAL_COUNT 10
static int big;
struct ThreeD_Vector
{
    float x;
    float y;
    float z;
}
ambient[MAX_MATERIAL_COUNT],
diffuse[MAX_MATERIAL_COUNT],
specular[MAX_MATERIAL_COUNT];

struct Tris_data
{
    struct ThreeD_Vector v0;
    struct ThreeD_Vector v1;
    struct ThreeD_Vector v2;
    struct ThreeD_Vector normal[3];
    struct ThreeD_Vector face_normal;
    float Color[3];
}*Tris;

GLfloat shine[MAX_MATERIAL_COUNT];
GLfloat x_min = 0, x_max = 0, y_min = 0, y_max = 0, z_min = 0, z_max = 0, x_center, y_center, z_center;
GLfloat x_rotate, y_rotate, z_rotate, x_move, y_move, z_move, near = 1.0, far = 5000.0, red_c = 1, green_c = 0, blue_c = 0;
GLint material_count, color_index[3], NumTris, model_switch = 0, i, j, display_type = 0;
GLbyte ch;
char str1[100] = "/Users/Stephanie/Documents/Xcode_projects/opengl\ hw1/opengl\ hw1/cube.txt";

void readTriangle(char *FileName)
{
    FILE* fp = fopen(FileName,"r");
    
    if (fp==NULL)
    {
        printf("ERROR: unable to open TriObj [%s]!\n",FileName);
        exit(1);
    }
    fscanf(fp, "%c", &ch);
    while(ch!= '\n') // skip the first line ®C object's name
        fscanf(fp, "%c", &ch);
    
    fscanf(fp,"# triangles = %d\n", &NumTris); // read # of triangles
    fscanf(fp,"Material count = %d\n", &material_count); // read material count
    
    for (i=0; i<material_count; i++) {
        fscanf(fp, "ambient color %f %f %f\n", &(ambient[i].x), &(ambient[i].y), &(ambient[i].z));
        fscanf(fp, "diffuse color %f %f %f\n", &(diffuse[i].x), &(diffuse[i].y), &(diffuse[i].z));
        fscanf(fp, "specular color %f %f %f\n", &(specular[i].x), &(specular[i].y), &(specular[i].z));
        fscanf(fp, "material shine %f\n", &(shine[i]));
    }
    fscanf(fp, "%c", &ch);
    while(ch!= '\n') // skip documentation line
        fscanf(fp, "%c", &ch);
    // allocate triangles for tri model
    printf ("Reading in %s (%d triangles). . .\n", FileName, NumTris);
    Tris = (struct Tris_data*)malloc(NumTris * sizeof(GLfloat) * 24);
    for (i=0; i<NumTris; i++) // read triangles
    {
        fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
               &(Tris[i].v0.x), &(Tris[i].v0.y), &(Tris[i].v0.z),
               &(Tris[i].normal[0].x), &(Tris[i].normal[0].y), &(Tris[i].normal[0].z),
               &(color_index[0]));
        fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
               &(Tris[i].v1.x), &(Tris[i].v1.y), &(Tris[i].v1.z),
               &(Tris[i].normal[1].x), &(Tris[i].normal[1].y), &(Tris[i].normal[1].z),
               &(color_index[1]));
        fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
               &(Tris[i].v2.x), &(Tris[i].v2.y), &(Tris[i].v2.z),
               &(Tris[i].normal[2].x), &(Tris[i].normal[2].y), &(Tris[i].normal[2].z),
               &(color_index[2]));
        fscanf(fp, "face normal %f %f %f\n", &(Tris[i].face_normal.x), &(Tris[i].face_normal.y),
               &(Tris[i].face_normal.z));
        x_min = x_min < Tris[i].v0.x ? x_min : Tris[i].v0.x;
        x_min = x_min < Tris[i].v1.x ? x_min : Tris[i].v1.x;
        x_min = x_min < Tris[i].v2.x ? x_min : Tris[i].v2.x;
        x_max = x_max > Tris[i].v0.x ? x_max : Tris[i].v0.x;
        x_max = x_max > Tris[i].v1.x ? x_max : Tris[i].v1.x;
        x_max = x_max > Tris[i].v2.x ? x_max : Tris[i].v2.x;
        
        y_min = y_min < Tris[i].v0.y ? y_min : Tris[i].v0.y;
        y_min = y_min < Tris[i].v1.y ? y_min : Tris[i].v1.y;
        y_min = y_min < Tris[i].v2.y ? y_min : Tris[i].v2.y;
        y_max = y_max > Tris[i].v0.y ? y_max : Tris[i].v0.y;
        y_max = y_max > Tris[i].v1.y ? y_max : Tris[i].v1.y;
        y_max = y_max > Tris[i].v2.y ? y_max : Tris[i].v2.y;
        
        z_min = z_min < Tris[i].v0.z ? z_min : Tris[i].v0.z;
        z_min = z_min < Tris[i].v1.z ? z_min : Tris[i].v1.z;
        z_min = z_min < Tris[i].v2.z ? z_min : Tris[i].v2.z;
        z_max = z_max > Tris[i].v0.z ? z_max : Tris[i].v0.z;
        z_max = z_max > Tris[i].v1.z ? z_max : Tris[i].v1.z;
        z_max = z_max > Tris[i].v2.z ? z_max : Tris[i].v2.z;
        
        Tris[i].Color[0] = (unsigned char)(int)(255*(diffuse[color_index[0]].x));
        Tris[i].Color[1] = (unsigned char)(int)(255*(diffuse[color_index[0]].y));
        Tris[i].Color[2] = (unsigned char)(int)(255*(diffuse[color_index[0]].z));
        
       
    }
    fclose(fp);
    
}

void init()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);

}




void Draw_picture(void)
{
    
    if(display_type == 0)
        glBegin(GL_POINTS);
    else if(display_type == 1)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_TRIANGLES);
    
    
    glColor3f(red_c, green_c, blue_c);
    for(i = 0; i < NumTris; i++)
    {
        if(display_type == 0) {
            glBegin(GL_POINTS);
            glVertex3f(Tris[i].v0.x, Tris[i].v0.y, Tris[i].v0.z);
            glEnd();
            glBegin(GL_POINTS);
            glVertex3f(Tris[i].v1.x, Tris[i].v1.y, Tris[i].v1.z);
            glEnd();
            glBegin(GL_POINTS);
            glVertex3f(Tris[i].v2.x, Tris[i].v2.y, Tris[i].v2.z);
            glEnd();
        }
        else if(display_type == 1){
            glBegin(GL_LINES);
            glVertex3f(Tris[i].v0.x, Tris[i].v0.y, Tris[i].v0.z);
            glVertex3f(Tris[i].v1.x, Tris[i].v1.y, Tris[i].v1.z);
            glEnd();
            
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINES);
            glVertex3f(Tris[i].v1.x, Tris[i].v1.y, Tris[i].v1.z);
            glVertex3f(Tris[i].v2.x, Tris[i].v2.y, Tris[i].v2.z);
            glEnd();
            glColor3f(1.0f, 0.0f, 1.0f);
            glBegin(GL_LINES);
            glVertex3f(Tris[i].v2.x, Tris[i].v2.y, Tris[i].v2.z);
            glVertex3f(Tris[i].v0.x, Tris[i].v0.y, Tris[i].v0.z);
            glEnd();
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(Tris[i].v0.x, Tris[i].v0.y, Tris[i].v0.z);
            //glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(Tris[i].v1.x, Tris[i].v1.y, Tris[i].v1.z);
            //glColor3f(1.0f, 0.0f, 1.0f);
            glVertex3f(Tris[i].v2.x, Tris[i].v2.y, Tris[i].v2.z);
            glEnd();
        }
        
        
    }
    
    glEnd();
    glFlush();
    
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    
    glRotatef(x_rotate, 1, 0, 0);
    glRotatef(y_rotate, 0, 1, 0);
    glRotatef(z_rotate, 0, 0, 1);
    
    glTranslatef(x_move, y_move, z_move);
    glColor3f(1, 0, 0);
    
    switch(model_switch)
    {
        case 0:
            gluLookAt(0,0,1,0,0,-2,0,1,0);
            break;
        case 1:
            gluLookAt(0,0,400 + z_center, x_center, y_center, z_center, 0, 1, 0);
            break;
        case 2:
            gluLookAt(400,0,800 + z_center, x_center, y_center, z_center, 0, 1, 0);
            break;
    }
    
    Draw_picture();
    
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat)w/(GLfloat)h, near, far);
    
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'a':
        case 'A':
            y_rotate += 5;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            y_rotate -= 5;
            glutPostRedisplay();
            break;
        case 'w':
        case 'W':
            x_rotate += 5;
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            x_rotate -= 5;
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            z_rotate -= 5;
            glutPostRedisplay();
            break;
        case 'e':
        case 'E':
            z_rotate += 5;
            glutPostRedisplay();
            break;
        case 't':
        case 'T':
            if(model_switch == 0)
                y_move += 0.3;
            else
                y_move += 30.0;
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            if(model_switch == 0)
                y_move -= 0.3;
            else
                y_move -= 30.0;
            glutPostRedisplay();
            break;
        case 'f':
        case 'F':
            if(model_switch == 0)
                x_move -= 0.3;
            else
                x_move -= 30.0;
            glutPostRedisplay();
            break;
        case 'h':
        case 'H':
            if(model_switch == 0)
                x_move += 0.3;
            else
                x_move += 30.0;
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            if(model_switch == 0)
                z_move -= 0.3;
            else
                z_move -= 30.0;
            glutPostRedisplay();
            break;
        case 'y':
        case 'Y':
            if(model_switch == 0)
                z_move += 0.3;
            else
                z_move += 30.0;
            glutPostRedisplay();
            break;
        case 'z':
        case 'Z':
            near += 1.0;
            //glMatrixMode(GL_PROJECTION);
            //glLoadIdentity();
            glutPostRedisplay();
            break;
        case 'x':
        case 'X':
            near -= 1.0;
            glutPostRedisplay();
            break;
        case 'c':
        case 'C':
            far += 1.0;
            glutPostRedisplay();
            break;
        case 'v':
        case 'V':
            far -= 1.0;
            glutPostRedisplay();
            break;
            
        case 'j':
        case 'J':
            red_c += 0.1;
            if(red_c >= 1)
                red_c = 0;
            glutPostRedisplay();
            break;
        case 'k':
        case 'K':
            green_c += 0.1;
            if(green_c >= 1)
                green_c = 0;
            glutPostRedisplay();
            break;
        case 'l':
        case 'L':
            blue_c += 0.1;
            if(blue_c >= 1)
                blue_c = 0;
            glutPostRedisplay();
            break;
            
        case 'b':
        case 'B':
            display_type ++;
            display_type = display_type % 3;
            glutPostRedisplay();
            break;
            
        case 'i':
        case 'I':
            strcpy(str1,"/Users/Stephanie/Documents/Xcode_projects/opengl\ hw1/opengl\ hw1/cube.txt");
            readTriangle(str1);
            model_switch = 0;
            glutPostRedisplay();
            break;
        case 'o':
        case 'O':
            strcpy(str1,"/Users/Stephanie/Documents/Xcode_projects/opengl\ hw1/opengl\ hw1/phone.txt");
            readTriangle(str1);
            x_center = (x_min+x_max)/2;
            y_center = (y_min+y_max)/2;
            z_center = (z_min+z_max)/2;
            printf("%4.2f", x_center);
            model_switch = 1;
            glutPostRedisplay();
            break;
        case 'p':
        case 'P':
            strcpy(str1,"/Users/Stephanie/Documents/Xcode_projects/opengl\ hw1/opengl\ hw1/cow_up.txt");
            readTriangle(str1);
            x_center = (x_min+x_max)/2;
            y_center = (y_min+y_max)/2;
            z_center = (z_min+z_max)/2;
            printf("%4.2f", x_center);
            model_switch = 2;
            glutPostRedisplay();
            break;
        case 'u':
        case 'U':
            glLoadIdentity();
            if(model_switch == 0)
            {
                
                gluLookAt(0,0,1,0,0,-2,0,1,0);
                
            }
            else if(model_switch == 1)
            {
                
                gluLookAt(0,0,400 + z_center, x_center, y_center, z_center, 0, 1, 0);
            }
            else
            {
                
                gluLookAt(400,0,800 + z_center, x_center, y_center, z_center, 0, 1, 0);
            }
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("OpenGL Assignment1");
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); 
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
    
}
