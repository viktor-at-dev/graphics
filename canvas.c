#include "canvas.h"
#include <stdlib.h>
#include <math.h>

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
void draw_line_naive(Canvas *c, int x0, int y0, int x1, int y1, Pixel color) {
    // Edge case guard: avoid division by zero if x0 == x1 (pure vertical line)
    if (x0 == x1) {
        // If x doesn't change, we just draw a vertical column
        int start_y = y0 < y1 ? y0 : y1;
        int end_y   = y0 < y1 ? y1 : y0;
        for (int y = start_y; y <= end_y; y++) {
            set_pixel(c, x0, y, color);
        }
        return;
    }

    // Step horizontally from x0 to x1
    for (int x = x0; x <= x1; x++) {
        // 1. Calculate t (progress factor between 0.0 and 1.0)
        float t = (float)(x - x0) / (float)(x1 - x0);

        // 2. Interpolate y
        int y = y0 + (int)((y1 - y0) * t);

        // 3. Write to our memory buffer!
        set_pixel(c, x, y, color);
    }
}
// swap function to swap two integers
void swap(int *a, int*b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
void draw_line_naive2(Canvas *c, int x0, int y0, int x1, int y1, Pixel color){
    if(x1 < x0){
        swap(&x0,&x1);
        swap(&y0,&y1);
        float a = (float)(y1 - y0) / (float)(x1 - x0);
        float y = y0;
        for(int x = x0;x <= x1;x++){
            set_pixel(c,x,(int)y,color);
            y += a;
        }
    }
}
// Fixed Bresenham implementation
void draw_line_bresenham(Canvas *c, int x0, int y0, int x1, int y1, Pixel color) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1) {
        set_pixel(c, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}