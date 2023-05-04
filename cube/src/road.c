
#include "utils.h"

void init_material(Material *material)
{
    material->ambient.red = 0.2;
    material->ambient.green = 0.2;
    material->ambient.blue = 0.3;

    material->diffuse.red = 0.3;
    material->diffuse.green = 0.3;
    material->diffuse.blue = 0.3;

    material->specular.red = 0.0;
    material->specular.green = 0.0;
    material->specular.blue = 0.0;

    material->shininess = 1.0;
}