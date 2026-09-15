#include <stdio.h>
#include "canvas.h"
#include "3d.h"
#include <math.h>
// viewport to canvas

point2D project_vertex(v3 vertex, int canvas_width, int canvas_height, float focal_length){
    if(vertex.z <= 0.1f){
        fprintf(stderr, "Warning: Vertex z-coordinate is too close to zero or negative. Adjusting to 0.1 to avoid division by zero.\n");
        vertex.z = 0.1f; // Prevent division by zero
    }
    // prspective projection
    float projected_x = (vertex.x * focal_length) / vertex.z;
    float projected_y = (vertex.y * focal_length) / vertex.z;
    point2D projected_point;
    projected_point.x = (int)((projected_x + canvas_width / 2.0f)+ 0.5f);
    projected_point.y = (int)((projected_y + canvas_height / 2.0f)+ 0.5f);
    return projected_point;
}