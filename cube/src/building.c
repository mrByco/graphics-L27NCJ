#include "building.h"
#include "utils.h"
#include "math.h"
#include <stdlib.h>
#include <obj/load.h>
#include <stdio.h>
#include <time.h>

int id = 0;
int discance = 8;
int mindistance = 7;

Model building_models[4];
GLuint building_textures[4];

void init_buildings(Map *map, int building_count, Building *buildings)
{
    load_model(&(building_models[0]), "assets/models/building1.obj");
    building_textures[0] = load_texture("assets/textures/building1.png");
    load_model(&(building_models[1]), "assets/models/building2.obj");
    building_textures[1] = load_texture("assets/textures/building2.png");
    load_model(&(building_models[2]), "assets/models/building3.obj");
    building_textures[2] = load_texture("assets/textures/building3.png");
    load_model(&(building_models[3]), "assets/models/building4.obj");
    building_textures[3] = load_texture("assets/textures/building4.png");

    srand(time(NULL));
    int pointsPerBuilding = map->length / building_count;
    for (int i = 0; i < building_count; i++)
    {
        int pointIndex = i * pointsPerBuilding;
        float x = map->x[pointIndex];
        float y = map->y[pointIndex];
        float x_n = map->normal_x[pointIndex];
        float y_n = map->normal_y[pointIndex];

        printf("Building %d: %f %f %f %f\n", i, x, y, x_n, y_n);

        int direction = (rand() % 2) * 2 - 1;
        vec3 position = {x + (y_n * discance * direction), y - (x_n * discance * direction), 0};
        int buildingType = rand() % 4;
        init_building(&(buildings[i]), buildingType, &position, 0);
    }
    hide_close_buildings(map, buildings, building_count);
}

void hide_close_buildings(Map *map, Building *buildings, int buildingCount)
{
    for (int i = 0; i < buildingCount; i++)
    {
        float bx = buildings[i].position.x;
        float by = buildings[i].position.y;

        for (int j = 0; j < map->length; j++)
        {
            float px = map->x[j];
            float py = map->y[j];

            float distance = sqrt(pow(bx - px, 2) + pow(by - py, 2));
            if (distance < mindistance)
            {
                printf("Distance: %f\n", distance);
                buildings[i].hidden = 1;
            }
        }
        printf("Building %d hidden: %d\n", i, buildings[i].hidden);
    }
}

void init_building(Building *building, int buildingType, vec3 *position, int zRot)
{
    printf("Building type: %d\n", buildingType);
    switch (buildingType)
    {
    case 0:
        building->model = &(building_models[0]);
        building->texture_id = building_textures[0];
        break;
    case 1:
        building->model = &(building_models[1]);
        building->texture_id = building_textures[1];
        break;
    case 2:
        building->model = &(building_models[2]);
        building->texture_id = building_textures[2];
        break;
    case 3:
        building->model = &(building_models[3]);
        building->texture_id = building_textures[3];
        break;
    default:
        load_model(&(building->model), "assets/models/building1.obj");
        building->texture_id = load_texture("assets/textures/building1.png");
        break;
    }

    building->position.x = position->x;
    building->position.y = position->y;
    building->position.z = position->z;

    building->rotation.x = 0;
    building->rotation.y = 0;
    building->rotation.z = zRot;

    building->material.ambient.red = 0.4;
    building->material.ambient.green = 0.4;
    building->material.ambient.blue = 0.4;

    building->material.diffuse.red = 1.0;
    building->material.diffuse.green = 1.0;
    building->material.diffuse.blue = 1.0;

    building->material.specular.red = 0.0;
    building->material.specular.green = 0.0;
    building->material.specular.blue = 0.0;

    building->material.shininess = 1.0;
    building->hidden = 0;
    building->id = id++;
}