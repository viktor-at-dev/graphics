#include <stdio.h>
#include "canvas.h"

int main(void) {
    Canvas *c = create_canvas(800, 800);
    if (!c) return 1;

    Pixel bg = {30, 30, 30};
    clear_canvas(c, bg);

    Pixel red = {255, 50, 50};
    // Draw a test line
    for (int i = 0; i < 400; i++) {
        set_pixel(c, i, i, red);
    }

    save_ppm(c, "output.ppm");
    free_canvas(c);

    printf("Done!\n");
    return 0;
}