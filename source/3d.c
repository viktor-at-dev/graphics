#include <math.h>
#include "3d.h"

point2D project_vertex(v3 vertex, int canvas_width, int canvas_height, float focal_length) {
    if (vertex.z <= 0.1f) {
        vertex.z = 0.1f;
    }

    float projected_x = (vertex.x * focal_length) / vertex.z;
    float projected_y = (vertex.y * focal_length) / vertex.z;

    point2D projected_point;
    projected_point.x = (int)roundf(projected_x + canvas_width / 2.0f);
    projected_point.y = (int)roundf((-projected_y) + canvas_height / 2.0f);

    return projected_point;
}
