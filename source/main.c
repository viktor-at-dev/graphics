#include <stdio.h>
#include <math.h>
#include "canvas.h"
#include "3d.h"

int main(void) {
    // 1. Initialize canvas (800x800)
    Canvas *c = create_canvas(800, 800);
    if (!c) {
        fprintf(stderr, "Failed to allocate canvas memory.\n");
        return 1;
    }

    // 2. Define color palette
    Pixel bg           = {20, 20, 30};    // Dark Slate
    Pixel red          = {255, 50, 50};   // Vibrant Red
    Pixel green        = {50, 255, 50};   // Neon Green
    Pixel blue         = {50, 150, 255};  // Sky Blue
    Pixel white        = {255, 255, 255}; // White
    Pixel yellow       = {255, 220, 50};  // Bright Yellow
    
    // 3d rendering essentials
    v3 v0 = { -1.0f, -1.0f, 2.0f };
    v3 v1 = {  1.0f, -1.0f, 2.0f };
    v3 v2 = {  0.0f,  1.0f, 2.0f };
    float focal_length = 400.0f; // Focal length for perspective projection
    point2D p0 = project_vertex(v0, c->width, c->height, focal_length);
    point2D p1 = project_vertex(v1, c->width, c->height, focal_length);
    point2D p2 = project_vertex(v2, c->width, c->height, focal_length);

    clear_canvas(c, bg);

    // TEST 1: Direct Line Drawing (Testing all octants & slopes)
    draw_better_line(c, 50, 50, 750, 200, green);
    draw_better_line(c, 100, 50, 200, 750, red);
    draw_better_line(c, 750, 750, 50, 650, blue);
    draw_better_line(c, 0, 0, 799, 799, yellow);
    draw_better_line(c, 0, 799, 799, 0, yellow);
    clear_canvas(c, bg);
    
    // TEST 2: Wireframe Triangle
    draw_triangle_wireframe(c, 100, 600, 300, 750, 50, 750, white);

    // TEST 3: Solid Filled Triangle
    draw_filled_triangle(c, 500, 100, 750, 100, 625, 400, blue);

    // TEST 4: Composite Shaded & Outlined Triangle
    draw_shaded_outlined_triangle(c, 300, 200, 500, 500, 150, 450, green, red);
    clear_canvas(c, bg);

    // TEST 5: Drawing a shaded triangle with interpolated intensity values
    triangle_shading(c,
                     400, 600, 0.0f,
                     600, 600, 1.0f,
                     500, 400, 0.5f,
                     yellow);

    clear_canvas(c, bg);

    triangle_shading(c,
                    p0.x, p0.y, 0.0f,
                    p1.x, p1.y, 1.0f,
                    p2.x, p2.y, 0.5f,
                    white);

    clear_canvas(c, bg);

    // -------------------------------------------------------------
    // TEST 6: 3D Cube Rendering
    // -------------------------------------------------------------

    // 8 Vertices of a Cube
    v3 cube_vertices[8] = {
        { -1.0f, -1.0f, -1.0f }, // 0: Bottom-left-back
        {  1.0f, -1.0f, -1.0f }, // 1: Bottom-right-back
        {  1.0f,  1.0f, -1.0f }, // 2: Top-right-back
        { -1.0f,  1.0f, -1.0f }, // 3: Top-left-back
        { -1.0f, -1.0f,  1.0f }, // 4: Bottom-left-front
        {  1.0f, -1.0f,  1.0f }, // 5: Bottom-right-front
        {  1.0f,  1.0f,  1.0f }, // 6: Top-right-front
        { -1.0f,  1.0f,  1.0f }  // 7: Top-left-front
    };

    // 12 Triangular Faces
    triangle_indices cube_faces[12] = {
        { 0, 1, 2, {255, 50, 50} },   // Back face (Red)
        { 0, 2, 3, {255, 50, 50} },
        { 4, 5, 6, {50, 255, 50} },   // Front face (Green)
        { 4, 6, 7, {50, 255, 50} },
        { 0, 1, 5, {50, 150, 255} },  // Bottom face (Blue)
        { 0, 5, 4, {50, 150, 255} },
        { 2, 3, 7, {255, 220, 50} },  // Top face (Yellow)
        { 2, 7, 6, {255, 220, 50} },
        { 1, 2, 6, {50, 255, 255} },  // Right face (Cyan)
        { 1, 6, 5, {50, 255, 255} },
        { 3, 0, 4, {255, 50, 255} },  // Left face (Magenta)
        { 3, 4, 7, {255, 50, 255} }
    };

    point2D projected_cube[8];
    float cube_z_offset = 4.0f;     // Move cube in front of lens
    float cube_focal = 300.0f;      // Focal length

    // Project all 8 cube vertices
    for (int i = 0; i < 8; i++) {
        v3 world_pos = cube_vertices[i];
        world_pos.z += cube_z_offset;

        projected_cube[i] = project_vertex(world_pos, c->width, c->height, cube_focal);
    }

    // Draw all 12 triangles forming the cube
    for (int i = 0; i < 12; i++) {
        triangle_indices face = cube_faces[i];
        point2D cp0 = projected_cube[face.v0];
        point2D cp1 = projected_cube[face.v1];
        point2D cp2 = projected_cube[face.v2];

        draw_filled_triangle(c, cp0.x, cp0.y, cp1.x, cp1.y, cp2.x, cp2.y, face.color);
    }

    // -------------------------------------------------------------
    // Output Generation & Cleanup
    // -------------------------------------------------------------
    if (save_ppm(c, "output.ppm")) {
        printf("Successfully rendered test frame to output.ppm\n");
    } else {
        fprintf(stderr, "Error: Failed to write output.ppm\n");
    }

    free_canvas(c);
    return 0;
}