#ifndef MAP_H
#define MAP_H

typedef struct Map
{
    int length;
    float *x;
    float *y;
    float *normal_x;
    float *normal_y;
} Map;

Map *map_init(int length);

#endif /* MAP_H */