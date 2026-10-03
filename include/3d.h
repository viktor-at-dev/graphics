#ifndef _3D_H_
#define _3D_H_

#include <stdio.h>
#include "canvas.h"

typedef struct{
    float x,y,z;
}v3;

typedef struct{
    int x,y;
}point2D;

typedef struct{
    int v0,v1,v2;
    Pixel color;
}triangle_indices;


point2D project_vertex(v3 vertex, int canvas_width, int canvas_height, float focal_length);














#endif // _3D_H_