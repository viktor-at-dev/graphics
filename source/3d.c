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

v3 rotate_x(v3 vertex, float angle) {
    v3 rotated;
    rotated.x = vertex.x;
    rotated.y = vertex.y * cos(angle) - vertex.z * sin(angle);
    rotated.z = vertex.y * sin(angle) + vertex.z * cos(angle);
    return rotated;
}

v3 rotate_y(v3 vertex, float angle) {
    v3 rotated;
    rotated.x = vertex.x * cos(angle) + vertex.z * sin(angle);
    rotated.y = vertex.y;
    rotated.z = -vertex.x * sin(angle) + vertex.z * cos(angle);
    return rotated;
}

v3 rotate_z(v3 vertex, float angle) {
    v3 rotated;
    rotated.x = vertex.x * cos(angle) - vertex.y * sin(angle);
    rotated.y = vertex.x * sin(angle) + vertex.y * cos(angle);
    rotated.z = vertex.z;
    return rotated;
}

v3 transform_vertex(v3 vertex, transform t) {
    v3 transformed = rotate_x(vertex, t.rotation.x);
    transformed = rotate_y(transformed, t.rotation.y);
    transformed = rotate_z(transformed, t.rotation.z);
    transformed.x *= t.scale.x;
    transformed.y *= t.scale.y;
    transformed.z *= t.scale.z;
    transformed.x += t.position.x;
    transformed.y += t.position.y;
    transformed.z += t.position.z;
    return transformed;
}
// cube definition
