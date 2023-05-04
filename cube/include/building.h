#ifndef BUILDING_H
#define BUILDING_H

#include "utils.h"
#include "bike.h"
#include "map.h"

#include <stdbool.h>

typedef struct Building
{
    int id;
    Model *model;
    vec3 position;
    vec3 rotation;
    GLuint texture_id;
    Material material;
    int hidden;
} Building;

void init_buildings(Map *map, int building_count, Building *buildings);

void init_building(Building *building, int buildingType, vec3 *position, int zRot);
void hide_close_buildings(Map *map, Building *building, int buildingCount);

#endif /* BUILDING_H */