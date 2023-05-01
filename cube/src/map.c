#include "map.h";
#include <stdio.h>
#include <math.h>
#define _USE_MATH_DEFINES
#define _E 2.71828182845904523536
#define _PI 3.14159265358979323846
#define MAP_SCALE 18

Map *map;

Map *map_init(int length)
{
    float generator_direction = 0;

    map = malloc(sizeof(Map));
    map->length = length;
    map->x = malloc(sizeof(float) * length);
    map->y = malloc(sizeof(float) * length);
    map->normal_x = malloc(sizeof(float) * length);
    map->normal_y = malloc(sizeof(float) * length);

    for (int i = 0; i < length; i++)
    {
        double x = (float)i / MAP_SCALE;
        map->x[i] = x * MAP_SCALE;
        map->y[i] = 0.3 * (-3.2 * sin(x) - 1 * sin((-1.7) * _E * x) + 2.1 * sin(0.7 * _PI * x)) * MAP_SCALE;
    }

    for (int i = 1; i < length; i++)
    {
        map->normal_x[i] = map->x[i] - map->x[i - 1];
        map->normal_y[i] = map->y[i] - map->y[i - 1];
        float normal_length = sqrt(map->normal_x[i] * map->normal_x[i] + map->normal_y[i] * map->normal_y[i]);
        map->normal_x[i] /= normal_length;
        map->normal_y[i] /= normal_length;
    }
    // print map
    for (int i = 0; i < length; i++)
    {
        printf("%f %f %f %f\n", map->x[i], map->y[i], map->normal_x[i], map->normal_y[i]);
    }
    return map;
}