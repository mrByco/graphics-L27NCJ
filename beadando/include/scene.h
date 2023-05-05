#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "bike.h"
#include "loader.h"
#include "map.h"
#include "building.h"

#include <obj/model.h>

typedef enum GameState
{
    Running = 0,
    Paused = 1,
    GameOver = 2
} GameState;

typedef struct Scene
{
    Bike *bike;
    Map *map;
    Building *buildings;
    int building_count;

    GameState state;

    GLuint pause_texture;
    GLuint game_over_texture;

    float lighting;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(Scene *scene);

/**
 * Set the current material.
 */
void set_material(const Material *material);

/**
 * Update the scene.
 */
void update_scene(Scene *scene, double time_delta);

/**
 * Render the scene objects.
 */
void render_scene(const Scene *scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();
void draw_point2D(float x, float y);
void draw_road_segment(float x0, float y0, float x0_normal, float y0_normal, float x1, float y1, float x1_normal, float y1_normal, float width);
void draw_skybox(vec3 center_pos, float distance);
void toggle_pause(Scene *scene);
void check_for_loose(Scene *scene);
void restart_game(Scene *scene);
double distance_to_line_segment(vec2 point, vec2 segmentStart, vec2 segmentEnd);
double dot(vec2 v1, vec2 v2);
double distance(vec2 v1, vec2 v2);

#endif /* SCENE_H */
