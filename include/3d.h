#ifndef _3D_H_
#define _3D_H_

#include <stdio.h>

typedef struct{
    float x,y,z;
}v3;

typedef struct{
    int x,y;
}point2D;


point2D project_vertex(v3 vertex, int canvas_width, int canvas_height, float focal_length);














#endif // _3D_H_