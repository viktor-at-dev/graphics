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

typedef struct{
    v3 position;
    v3 rotation;
    v3 scale;
}transform;

typedef struct{
    v3 *vertices;
    int vertex_count;
    triangle_indices *faces;
    int face_count;
} mesh;

typedef struct{
    mesh *mesh;
    transform transform;
}instance;


point2D project_vertex(v3 vertex, int canvas_width, int canvas_height, float focal_length);
v3 rotate_x(v3 vertex, float angle);
v3 rotate_y(v3 vertex, float angle);
v3 rotate_z(v3 vertex, float angle);
v3 transform_vertex(v3 vertex, transform t);













#endif // _3D_H_