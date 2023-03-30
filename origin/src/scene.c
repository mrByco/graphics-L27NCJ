#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/gl.h>

float geometry[25][25];

void init_scene(Scene* scene)
{
   srand(time(0));

   for (int i = 0; i < 25; i++){
    for (int j = 0; j < 25; i++){
        geometry[i][j] = rand();
    }
   }

   return 0;
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    draw_origin();
}

void draw_origin()
{
    printf("HELLO");
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glColor3f(1, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(3, 3, 1);

    glEnd();

    draw_color_terrain();

}


void draw_color_terrain(){
    //float arr[4] = {0.0, 0.0, 0.0, 0.0};
    //arr
    draw_square();
    
   /*for (int i = 0; i < 25; i++){
    for (int j = 0; j < 25; i++){
        geometry[i][j] = rand();
    }
   }*/
}

void draw_square(){ //float points[4]
    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 1);
    // 00
    glVertex3f(0, 0, 0);
    glColor3f(1, 0, 1);
    // 01
    glVertex3f(0, 0, 1);
    glColor3f(1, 1, 0);
    // 10
    glVertex3f(1, 1, 0);

    
    glColor3f(0, 1, 1);
    // 10
    glVertex3f(1, 0, 0);
    glColor3f(1, 0, 1);
    // 01
    glVertex3f(0, 0, 1);
    glColor3f(1, 1, 0);
    // 11
    glVertex3f(1, 1, 1);


    glEnd();
}
