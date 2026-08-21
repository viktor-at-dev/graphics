#include <stdio.h>
#include "canvas.h"

int main(void) {
    Canvas *c = create_canvas(800, 800);
    if (!c) return 1;

    Pixel bg    = {20, 20, 30};
    Pixel red   = {255, 50, 50};
    Pixel green = {50, 255, 50};
    Pixel blue  = {50, 150, 255};

    clear_canvas(c, bg);

    // Test 1: Gentle Line (Width > Height) -> WORKS GREAT
    // draw_line_naive(c, 100, 100, 700, 300, red);

    // Test 2: Steep Line (Height > Width) -> LEAVES GAPS!
    // draw_line_naive(c, 200, 100, 300, 700, green);

    // Test 3: Right-to-Left Line (x0 > x1) -> FAILS TO DRAW!
    // draw_line_naive(c, 700, 700, 100, 600, blue);
    draw_line_naive2(c, 700,700,100,600,blue);
    draw_poor_line(c,-50,-200,60,240,red);
    drawline(c,100,100,700,300,green);
    drawline(c,0,0,800,800,blue);

    if (save_ppm(c, "output.ppm")) {
        printf("Rendered naive lines to output.ppm successfully!\n");
    }

    free_canvas(c);
    return 0;
}