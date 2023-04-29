#ifndef BIKE_H
#define BIKE_H

#include "texture.h"
#include "utils.h"
#include "game_control.h"

#include <obj/model.h>

typedef struct Bike
{
    Model model;
    Material material;
    GLuint texture_id;

    float direction;
    vec3 position;
    vec3 rotation;
    float speed;
    float steering;

    Control *control;

} Bike;

void bind_control(Bike *bike, Control *control);
void update_bike_control(Bike *bike);

#endif /* BIKE_H */