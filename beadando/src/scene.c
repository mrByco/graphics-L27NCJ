#include "scene.h"
#include "loader.h"
#include "bike.h"
#include "utils.h"
#include "road.h"
#include "map.h"

#include <obj/draw.h>
#include <math.h>

#define MAP_WIDTH 1

Material *road_material;

void init_scene(Scene *scene)
{
    scene->bike = malloc(sizeof(Bike));
    scene->map = map_init(400);
    load_bike(scene->bike);
    init_bike(scene->bike);
    scene->bike->position.x = scene->map->x[0];
    scene->bike->position.y = scene->map->y[0];
    scene->building_count = 100;
    road_material = malloc(sizeof(Material));
    init_material(road_material);

    scene->lighting = 1.0f;
    scene->state = Paused;
    scene->game_over_texture = load_texture("assets/textures/game_over_texture.png");
    scene->pause_texture = load_texture("assets/textures/help_texture.png");
    scene->progress_pointer_texture = load_texture("assets/textures/pointer.jpg");
    scene->progress_bar_texture = load_texture("assets/textures/progress.png");

    scene->buildings = malloc(scene->building_count * sizeof(Building));
    init_buildings(scene->map, scene->building_count, scene->buildings);

    glEnable(GL_FOG);
    glFogf(GL_FOG_DENSITY, 0.02f);
    glFogfv(GL_FOG_COLOR, (GLfloat[]){0.8f, 0.8f, 0.8f, 1.0f});
}

void set_lighting(Scene *scene)
{
    float ambient_light[] = {1.0f * scene->lighting, 1.0f * scene->lighting, 1.0f * scene->lighting, 1.0f * scene->lighting};
    float diffuse_light[] = {0.5f * scene->lighting, 0.5f * scene->lighting, 0.5f * scene->lighting, 1.0f * scene->lighting};
    float specular_light[] = {0.0f * scene->lighting, 0.0f * scene->lighting, 0.0f * scene->lighting, 0.0f * scene->lighting};
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
    if (scene->state != Running)
        return;
    update_bike_control(scene->bike);
    move_bike(scene->bike, elapsed_time);
    check_for_loose(scene);
}

void render_scene(const Scene *scene)
{
    set_lighting(scene);
    if (scene->state == Paused)
    {
        draw_gui(scene->pause_texture);
        return;
    }
    else if (scene->state == GameOver)
    {
        draw_gui(scene->game_over_texture);
        return;
    }
    glEnable(GL_FOG);
    draw_origin();
    set_material(&(scene->bike->material));
    draw_model_scene_placed(scene->bike->position, scene->bike->rotation, &(scene->bike->model), scene->bike->texture_id);
    draw_map(scene->map);
    draw_buildings(scene);
    draw_skybox(scene->bike->position, 60);

    vec2 hudPos1 = {2.2f, 0.0f};
    vec2 hudPos2 = {2.4f, 2.0f};
    vec2 size = {30.0f, 50.0f};
    float pointerBottom = scene->progress * hudPos2.y;
    vec2 pointerPos1 = {2.2f, pointerBottom + 0.02f};
    vec2 pointerPos2 = {2.4f, pointerBottom};
    draw_hud(scene->progress_pointer_texture, pointerPos1, pointerPos2, size);
    draw_hud(scene->progress_bar_texture, hudPos1, hudPos2, size);
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
    bike->speed += bike->speed * 0.01 * time_delta;
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
        float width = i < 10 ? 5 : MAP_WIDTH;
        draw_road_segment(map->x[i - 1], map->y[i - 1], map->normal_x[i - 1], map->normal_y[i - 1], map->x[i], map->y[i], map->normal_x[i], map->normal_y[i], width);
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
        draw_building(currentBuilting);
    }
}

void draw_building(Building building)
{
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

    glBegin(GL_QUADS);

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

void check_for_loose(Scene *scene)
{
    Bike *bike = scene->bike;
    float minDistanceToMap = 9.0f;

    for (int i = 0; i < scene->map->length - 1; i++)
    {
        vec2 checkpoint1 = {scene->map->x[i], scene->map->y[i]};
        vec2 checkpoint2 = {scene->map->x[i + 1], scene->map->y[i + 1]};

        vec2 bikePos2d = {bike->position.x, bike->position.y};

        // calculate the distance to the line segment
        double distance = distance_to_line_segment(bikePos2d, checkpoint1, checkpoint2);

        if (distance < minDistanceToMap)
        {
            minDistanceToMap = distance;
            scene->progress = (float)i / (float)scene->map->length;
        }
    }
    float distanceToFirstPoint = sqrt(pow(bike->position.x - scene->map->x[0], 2) + pow(bike->position.y - scene->map->y[0], 2));
    if (minDistanceToMap > MAP_WIDTH + 0.1f && distanceToFirstPoint > 15)
    {
        printf("You loose\n");
        scene->state = GameOver;
    }
}

void draw_gui(GLuint texture)
{
    glDisable(GL_FOG);

    float ambient_material_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float zeros[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zeros);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeros);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3d(-2.5, 1.5, -3);
    glTexCoord2f(1, 0);
    glVertex3d(2.5, 1.5, -3);
    glTexCoord2f(1, 1);
    glVertex3d(2.5, -1.5, -3);
    glTexCoord2f(0, 1);
    glVertex3d(-2.5, -1.5, -3);
    glEnd();
}

void draw_hud(GLuint texture, vec2 pos1, vec2 pos2, vec2 imageSize)
{
    float ambient_material_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float zeros[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zeros);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeros);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(pos1.x, pos2.y, -3); // Top-left corner
    glTexCoord2f(1, 0);
    glVertex3f(pos2.x, pos2.y, -3); // Top-right corner
    glTexCoord2f(1, 1);
    glVertex3f(pos2.x, pos1.y, -3); // Bottom-right corner
    glTexCoord2f(0, 1);
    glVertex3f(pos1.x, pos1.y, -3); // Bottom-left corner
    glEnd();
}

void toggle_pause(Scene *scene)
{
    scene->state = scene->state == Paused ? Running : Paused;
}

void restart_game(Scene *scene)
{

    free(scene->bike);
    free(scene->map);

    free(road_material);

    free(scene->buildings);

    init_scene(scene);
}

double distance_to_line_segment(vec2 point, vec2 segmentStart, vec2 segmentEnd)
{
    vec2 v = {
        segmentEnd.x - segmentStart.x,
        segmentEnd.y - segmentStart.y,
    };
    vec2 w = {
        point.x - segmentStart.x,
        point.y - segmentStart.y,
    };

    double c1 = dot(w, v);
    if (c1 <= 0)
        return distance(point, segmentStart);

    double c2 = dot(v, v);
    if (c2 <= c1)
        return distance(point, segmentEnd);

    double b = c1 / c2;
    vec2 pb = {segmentStart.x + b * v.x, segmentStart.y + b * v.y};
    return distance(point, pb);
}

double dot(vec2 v1, vec2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

double distance(vec2 v1, vec2 v2)
{
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}