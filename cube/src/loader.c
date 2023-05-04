#include "loader.h"

#include <obj/load.h>
#include <bike.h>

void load_bike(Bike *bike)
{
    load_model(&(bike->model), "assets/models/bike2.obj");
    bike->texture_id = load_texture("assets/textures/white.png");

    bike->material.ambient.red = 0.5;
    bike->material.ambient.green = 0.5;
    bike->material.ambient.blue = 0.5;

    bike->material.diffuse.red = 1.0;
    bike->material.diffuse.green = 1.0;
    bike->material.diffuse.blue = 1.0;

    bike->material.specular.red = 1.0;
    bike->material.specular.green = 1.0;
    bike->material.specular.blue = 1.0;

    bike->material.shininess = 1.0;
}