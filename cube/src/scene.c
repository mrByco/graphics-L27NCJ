#include "scene.h"
#include "loader.h"
#include "bike.h"
#include "utils.h"

#include <obj/draw.h>

void init_scene(Scene *scene)
{
    scene->bike = malloc(sizeof(Bike));
    load_bike(scene->bike);
    init_bike(scene->bike);
}

void set_lighting()
{
    float ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float diffuse_light[] = {1.0f, 1.0f, 1.0, 0.0f};
    float specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float position[] = {0.0f, 0.0f, 10.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material *material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue};

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue};

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene *scene, double elapsed_time)
{
    update_bike_control(scene->bike);
    move_bike(scene->bike, elapsed_time);
}

void render_scene(const Scene *scene)
{
    set_material(&(scene->bike->material));
    set_lighting();
    draw_origin();
    draw_model_scene_placed(scene->bike->position, scene->bike->rotation, &(scene->bike->model));
    // draw_model(&(scene->bike->model));
}

void draw_origin()
{
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

    glEnd();
}

void move_bike(Bike *bike, double time_delta)
{
    bike->position.x += bike->speed * sin(degree_to_radian(-bike->rotation.z)) * time_delta;
    bike->position.y += bike->speed * cos(degree_to_radian(-bike->rotation.z)) * time_delta;
    bike->rotation.z += bike->speed * bike->steering * 50 * time_delta;
    bike->rotation.y = -bike->steering * 50;
}

void draw_model_scene_placed(vec3 offset, vec3 rotation, Model *model)
{
    glPushMatrix();
    glTranslatef(offset.x, offset.y, offset.z);
    glRotatef(rotation.z, 0, 0, 1);
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    draw_model(model);
    glPopMatrix();
}
