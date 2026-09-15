#include <stdio.h>
#include "canvas.h"
#include "3d.h"
#include <math.h>

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

    // Gentle line (Width > Height, dx > dy)
    draw_better_line(c, 50, 50, 750, 200, green);
    
    // Steep line (Height > Width, dy > dx)
    draw_better_line(c, 100, 50, 200, 750, red);
    
    // Right-to-Left line (x0 > x1)
    draw_better_line(c, 750, 750, 50, 650, blue);
    
    // Diagonals across canvas
    draw_better_line(c, 0, 0, 799, 799, yellow);
    draw_better_line(c, 0, 799, 799, 0, yellow);
    clear_canvas(c, bg); // Clear canvas to test triangle rendering without line clutter
    
    // TEST 2: Wireframe Triangle

    draw_triangle_wireframe(c, 100, 600, 300, 750, 50, 750, white);

    
    // TEST 3: Solid Filled Triangle
    draw_filled_triangle(c, 500, 100, 750, 100, 625, 400, blue);

    
    // TEST 4: Composite Shaded & Outlined Triangle
    
    draw_shaded_outlined_triangle(c, 300, 200, 500, 500, 150, 450, green, red);

    clear_canvas(c, bg); // Clear canvas to test triangle rendering without line clutter

    // Test 5: Drawing a shaded triangle with interpolated intensity values
    triangle_shading(c,
                     400, 600, 0.0f,   // Vertex A with intensity
                     600, 600, 1.0f,   // Vertex B with intensity
                     500, 400, 0.5f,   // Vertex C with intensity
                     yellow);          // Color to shade with

    clear_canvas(c, bg); // Clear canvas to test triangle rendering without line clutter

     triangle_shading(c,
                    p0.x, p0.y, 0.0f,   // Vertex A with intensity
                    p1.x, p1.y, 1.0f,   // Vertex B with intensity
                    p2.x, p2.y, 0.5f,   // Vertex C with intensity
                    white);             // Color to shade with

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