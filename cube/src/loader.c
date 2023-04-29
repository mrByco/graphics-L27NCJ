#include "loader.h"

#include <obj/load.h>
#include <bike.h>

void load_bike(Bike *bike)
{
    load_model(&(bike->model), "assets/models/bike1.obj");
    bike->texture_id = load_texture("assets/textures/white.png");

    glBindTexture(GL_TEXTURE_2D, bike->texture_id);

    bike->material.ambient.red = 0.0;
    bike->material.ambient.green = 0.0;
    bike->material.ambient.blue = 0.0;

    bike->material.diffuse.red = 100.0;
    bike->material.diffuse.green = 100.0;
    bike->material.diffuse.blue = 100.0;

    bike->material.specular.red = 0.0;
    bike->material.specular.green = 0.0;
    bike->material.specular.blue = 0.0;

    bike->material.shininess = 1.0;
}