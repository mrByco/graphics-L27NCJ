#include "map.h";
#include <stdio.h>
#include <math.h>
#include <time.h>
#define _USE_MATH_DEFINES
#define _E 2.71828182845904523536
#define _PI 3.14159265358979323846
#define MAP_SCALE 18

Map *map;

Map *map_init(int length)
{
    srand(time(NULL));
    int seed = rand();
    float generator_direction = 0;

    map = malloc(sizeof(Map));
    map->length = length;
    map->x = malloc(sizeof(float) * length);
    map->y = malloc(sizeof(float) * length);
    map->normal_x = malloc(sizeof(float) * length);
    map->normal_y = malloc(sizeof(float) * length);

    for (int i = 0; i < length; i++)
    {
        double x = (float)i / MAP_SCALE + seed;
        map->x[i] = x * MAP_SCALE;
        map->y[i] = 0.3 * (-3.2 * sin(x) - 1 * sin((-1.7) * _E * x) + 2.1 * sin(0.7 * _PI * x)) * MAP_SCALE;
    }

    float minX = map->x[0];
    float minY = map->y[0];

    for (int i = 1; i < length; i++)
    {
        if (map->x[i] < minX)
        {
            minX = map->x[i];
        }
        if (map->y[i] < minY)
        {
            minY = map->y[i];
        }
    }

    for (int i = 0; i < length; i++)
    {
        map->x[i] -= minX;
        map->y[i] -= minY;
    }

    map->normal_x[0] = map->x[1] - map->x[0];
    map->normal_y[0] = map->y[1] - map->y[0];
    float normal_length = sqrt(map->normal_x[0] * map->normal_x[0] + map->normal_y[0] * map->normal_y[0]);
    map->normal_x[0] /= normal_length;
    map->normal_y[0] /= normal_length;

    for (int i = 1; i < length; i++)
    {
        map->normal_x[i] = map->x[i] - map->x[i - 1];
        map->normal_y[i] = map->y[i] - map->y[i - 1];
        float normal_length = sqrt(map->normal_x[i] * map->normal_x[i] + map->normal_y[i] * map->normal_y[i]);
        map->normal_x[i] /= normal_length;
        map->normal_y[i] /= normal_length;
    }
    // print map
    /*for (int i = 0; i < length; i++)
    {
        printf("%f %f %f %f\n", map->x[i], map->y[i], map->normal_x[i], map->normal_y[i]);
    }*/
    return map;
}