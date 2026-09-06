#ifndef CANVAS_H
#define CANVAS_H

#include <stdint.h>
#include <stdio.h>

// Struct Definitions
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct {
    int width;
    int height;
    Pixel *pixels;
} Canvas;

// Core Canvas API
Canvas* create_canvas(int width, int height);
void clear_canvas(Canvas *c, Pixel color);
void free_canvas(Canvas *c);
void set_pixel(Canvas *c, int x, int y, Pixel color);
int save_ppm(const Canvas *c, const char *filename);

// Helper / Math Routine
void interpolate(int i0, int d0, int i1, int d1, int *values);

// Line Drawing Algorithms
void draw_better_line(Canvas *c, int x0, int y0, int x1, int y1, Pixel color);
void drawline_with_interpolation(Canvas *c, int x0, int y0, int x1, int y1, Pixel color);

// Triangle Primitive Routines
void draw_triangle_wireframe(Canvas *c, int x0, int y0, int x1, int y1, int x2, int y2, Pixel color);
void draw_filled_triangle(Canvas *c, int x0, int y0, int x1, int y1, int x2, int y2, Pixel color);
void draw_shaded_outlined_triangle(Canvas *c, int x0, int y0, int x1, int y1, int x2, int y2, Pixel fill_color, Pixel outline_color);

#endif // CANVAS_H