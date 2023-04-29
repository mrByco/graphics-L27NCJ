#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "bike.h"
#include "loader.h"

#include <obj/model.h>

typedef struct Scene
{
    Bike *bike;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

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

#endif /* SCENE_H */
