#include <stdio.h>
#include "canvas.h"

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

    clear_canvas(c, bg);

    // -------------------------------------------------------------
    // TEST 1: Direct Line Drawing (Testing all octants & slopes)
    // -------------------------------------------------------------
    // Gentle line (Width > Height, dx > dy)
    draw_better_line(c, 50, 50, 750, 200, green);
    
    // Steep line (Height > Width, dy > dx)
    draw_better_line(c, 100, 50, 200, 750, red);
    
    // Right-to-Left line (x0 > x1)
    draw_better_line(c, 750, 750, 50, 650, blue);
    
    // Diagonals across canvas
    draw_better_line(c, 0, 0, 799, 799, yellow);
    draw_better_line(c, 0, 799, 799, 0, yellow);

    // -------------------------------------------------------------
    // TEST 2: Wireframe Triangle
    // -------------------------------------------------------------
    draw_triangle_wireframe(c, 100, 600, 300, 750, 50, 750, white);

    // -------------------------------------------------------------
    // TEST 3: Solid Filled Triangle
    // -------------------------------------------------------------
    draw_filled_triangle(c, 500, 100, 750, 100, 625, 400, blue);

    // -------------------------------------------------------------
    // TEST 4: Composite Shaded & Outlined Triangle
    // -------------------------------------------------------------
    draw_shaded_outlined_triangle(c, 300, 200, 500, 500, 150, 450, green, red);

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