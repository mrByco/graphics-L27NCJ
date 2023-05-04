#include "scene.h"
#include "loader.h"
#include "bike.h"
#include "utils.h"
#include "road.h"
#include "map.h"

#include <obj/draw.h>
#include <math.h>

Material *road_material;

void init_scene(Scene *scene)
{
    scene->bike = malloc(sizeof(Bike));
    scene->map = map_init(400);
    load_bike(scene->bike);
    init_bike(scene->bike);
    scene->building_count = 100;
    road_material = malloc(sizeof(Material));
    init_material(road_material);

    scene->buildings = malloc(scene->building_count * sizeof(Building));
    init_buildings(scene->map, scene->building_count, scene->buildings);
}

void set_lighting(Scene *scene)
{
    float ambient_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float diffuse_light[] = {0.5f, 0.5f, 0.5, 1.0f};
    float specular_light[] = {0.0f, 0.0f, 0.0f, 0.0f};
    float position[] = {scene->bike->position.x + 5, scene->bike->position.y, scene->bike->position.z + 5, 1.0f};

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
    set_lighting(scene);
    draw_origin();
    set_material(&(scene->bike->material));
    draw_model_scene_placed(scene->bike->position, scene->bike->rotation, &(scene->bike->model), scene->bike->texture_id);
    draw_map(scene->map);
    draw_buildings(scene);
    draw_skybox(scene->bike->position, 60);
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

void draw_model_scene_placed(vec3 offset, vec3 rotation, Model *model, GLuint texture_id)
{
    glPushMatrix();
    glTranslatef(offset.x, offset.y, offset.z);
    glRotatef(rotation.z, 0, 0, 1);
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    draw_model(model);
    glPopMatrix();
}

void draw_map(Map *map)
{

    for (int i = 1; i < map->length; i++)
    {
        float width = i < 10 ? 5 : 1;
        draw_road_segment(map->x[i - 1], map->y[i - 1], map->normal_x[i - 1], map->normal_y[i - 1], map->x[i], map->y[i], map->normal_x[i], map->normal_y[i], width);
        // draw_point2D(map->x[i], map->y[i]);
        //  glVertex3f(map->x[i - 1], map->y[i - 1], 0);
        //  glVertex3f(map->x[i], map->y[i], 0);
    }
}

void draw_point2D(float x, float y)
{
    float f = 0.1;

    glVertex3f(x + f, y + f, 0);
    glVertex3f(x - f, y - f, 0);

    glVertex3f(x + f, y - f, 0);
    glVertex3f(x - f, y + f, 0);
}

void draw_buildings(Scene *scene)
{
    for (int i = 0; i < scene->building_count; i++)
    {
        Building currentBuilting = scene->buildings[i];
        // printf("building id8: %d\n", scene->buildings);
        // printf("building id: %d\n", &(currentBuilting));
        draw_building(currentBuilting);
    }
}

void draw_building(Building building)
{
    // printf("position: %f, %f, %f\n", building.position.x, building.position.y, building.position.z);
    if (building.hidden == 1)
    {
        return;
    }
    set_material(&(building.material));
    draw_model_scene_placed(building.position, building.rotation, building.model, building.texture_id);
}

void draw_road_segment(float x0, float y0, float x0_normal, float y0_normal, float x1, float y1, float x1_normal, float y1_normal, float width)
{
    set_material(road_material);
    vec2 p1 = {x0 + y0_normal * width, y0 - x0_normal * width};
    vec2 p2 = {x0 - y0_normal * width, y0 + x0_normal * width};
    vec2 p3 = {x1 + y1_normal * width, y1 - x1_normal * width};
    vec2 p4 = {x1 - y1_normal * width, y1 + x1_normal * width};

    glBegin(GL_QUADS);

    glVertex3f(p2.x, p2.y, 0);
    glVertex3f(p4.x, p4.y, 0);
    glVertex3f(p3.x, p3.y, 0);
    glVertex3f(p1.x, p1.y, 0);

    glEnd();
}

void draw_skybox(vec3 center_pos, float distance)
{

    // set_material(road_material);

    float ambient_material_color[] = {0.2196, 0.549, 1.0, 1.0};
    float zeros[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zeros);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeros);

    float shininess = 0.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
    printf("center_pos, distance: %f, %f, %f, %f\n", center_pos.x, center_pos.y, center_pos.z, distance);

    glBegin(GL_QUADS);

    printf("center_pos, distance: %f, %f, %f, %f\n", center_pos.x, center_pos.y, center_pos.z, distance);

    // Draw the left face
    glVertex3f(center_pos.x - distance, center_pos.y - distance, center_pos.z + distance);
    glVertex3f(center_pos.x - distance, center_pos.y - distance, center_pos.z - distance);
    glVertex3f(center_pos.x - distance, center_pos.y + distance, center_pos.z - distance);
    glVertex3f(center_pos.x - distance, center_pos.y + distance, center_pos.z + distance);

    // Draw the back face
    glVertex3f(center_pos.x + distance, center_pos.y - distance, center_pos.z + distance);
    glVertex3f(center_pos.x - distance, center_pos.y - distance, center_pos.z + distance);
    glVertex3f(center_pos.x - distance, center_pos.y + distance, center_pos.z + distance);
    glVertex3f(center_pos.x + distance, center_pos.y + distance, center_pos.z + distance);

    // Draw the bottom face
    glVertex3f(center_pos.x + distance, center_pos.y - distance, center_pos.z - distance);
    glVertex3f(center_pos.x - distance, center_pos.y - distance, center_pos.z - distance);
    glVertex3f(center_pos.x - distance, center_pos.y - distance, center_pos.z + distance);
    glVertex3f(center_pos.x + distance, center_pos.y - distance, center_pos.z + distance);

    // Draw the top face
    glVertex3f(center_pos.x + distance, center_pos.y + distance, center_pos.z - distance);
    glVertex3f(center_pos.x - distance, center_pos.y + distance, center_pos.z - distance);
    glVertex3f(center_pos.x - distance, center_pos.y + distance, center_pos.z + distance);
    glVertex3f(center_pos.x + distance, center_pos.y + distance, center_pos.z + distance);

    // Draw the right face
    glVertex3f(center_pos.x + distance, center_pos.y - distance, center_pos.z + distance);
    glVertex3f(center_pos.x + distance, center_pos.y - distance, center_pos.z - distance);
    glVertex3f(center_pos.x + distance, center_pos.y + distance, center_pos.z - distance);
    glVertex3f(center_pos.x + distance, center_pos.y + distance, center_pos.z + distance);

    float ambient_gray_material_color[] = {0.3f, 0.3f, 0.4f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_gray_material_color);

    // Draw the front face
    glVertex3f(center_pos.x + distance, center_pos.y - distance, center_pos.z - 0.1f);
    glVertex3f(center_pos.x - distance, center_pos.y - distance, center_pos.z - 0.1f);
    glVertex3f(center_pos.x - distance, center_pos.y + distance, center_pos.z - 0.1f);
    glVertex3f(center_pos.x + distance, center_pos.y + distance, center_pos.z - 0.1f);

    glEnd();
}