#include "building.h"
#include "utils.h"

int id = 3;

void init_buildings(Map *map, int building_count, Building *buildings)
{
    int pointsPerBuilding = map->length / building_count;
    int buildingType = 0;
    for (int i = 0; i < building_count; i++)
    {
        int x = map->x[i * pointsPerBuilding];
        int y = map->y[i * pointsPerBuilding];
        int x_n = map->normal_x[i * pointsPerBuilding];
        int y_n = map->normal_y[i * pointsPerBuilding];

        vec3 position = {x + (y_n * 10), y - (x_n * 10), 0};
        init_building(buildings[i], buildingType, &position, 0);
    }
}

void init_building(Building building, int buildingType, vec3 *position, int zRot)
{
    if (buildingType = 0)
    {
        load_model(building.model, "assets/models/building1.obj");
        building.texture_id = load_texture("assets/textures/building1.png");
    }

    glBindTexture(GL_TEXTURE_2D, building.texture_id);

    building.material.ambient.red = 0.0;
    building.material.ambient.green = 0.0;
    building.material.ambient.blue = 0.0;

    building.material.diffuse.red = 100.0;
    building.material.diffuse.green = 100.0;
    building.material.diffuse.blue = 100.0;

    building.material.specular.red = 0.0;
    building.material.specular.green = 0.0;
    building.material.specular.blue = 0.0;

    building.material.shininess = 1.0;
    building.id = 8;
    printf("Building %d initialized\n", building.id);
    printf("Building %d initialized\n", &(building));
}