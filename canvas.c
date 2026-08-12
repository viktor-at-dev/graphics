#include "canvas.h"
#include <stdlib.h>

Canvas* create_canvas(int width, int height) {
    Canvas *c = malloc(sizeof(Canvas));
    if (c == NULL) return NULL;

    c->width = width;
    c->height = height;
    c->pixels = malloc(sizeof(Pixel) * width * height);
    if (c->pixels == NULL) {
        free(c);
        return NULL;
    }
    return c;
}

void clear_canvas(Canvas *c, Pixel color) {
    if (!c || !c->pixels) return;
    for (int i = 0; i < c->width * c->height; i++) {
        c->pixels[i] = color;
    }
}

void free_canvas(Canvas *c) {
    if (!c) return;
    free(c->pixels);
    free(c);
}

void set_pixel(Canvas *c, int x, int y, Pixel color) {
    if (!c || x < 0 || x >= c->width || y < 0 || y >= c->height) {
        return;
    }
    int flipped_y = c->height - 1 - y;
    int index = flipped_y * c->width + x;
    c->pixels[index] = color;
}

int save_ppm(const Canvas *c, const char *filename) {
    if (!c || !c->pixels) return 0;
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file");
        return 0;
    }
    fprintf(fp, "P6\n%d %d\n255\n", c->width, c->height);
    fwrite(c->pixels, sizeof(Pixel), c->width * c->height, fp);
    fclose(fp);
    return 1;
}