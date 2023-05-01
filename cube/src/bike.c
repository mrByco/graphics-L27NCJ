#include "bike.h"
#include "game_control.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_bike(Bike *bike)
{
    bike->position.x = 1;
    bike->position.y = 0;
    bike->position.z = 0;

    bike->rotation.x = 0.0;
    bike->rotation.y = -5.0;
    bike->rotation.z = 0.0;

    bike->speed = 3;
    bike->steering = 1;
}

void bind_control(Bike *bike, Control *control)
{
    bike->control = control;
    printf("Control binded\n");
}

void update_bike_control(Bike *bike)
{
    bike->steering = bike->control->target_steering;
}