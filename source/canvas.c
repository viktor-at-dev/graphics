#include "canvas.h"
#include <stdio.h>
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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void draw_better_line(Canvas *c, int x0, int y0, int x1, int y1, Pixel color) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    if (abs((int)dx) > abs((int)dy)) {
        if (x0 > x1) {
            swap(&x0, &x1);
            swap(&y0, &y1);
        }
        float a = dy / dx;
        float y = y0;
        for (int x = x0; x <= x1; x++) {
            set_pixel(c, x, (int)roundf(y), color);
            y += a;
        }
    } else {
        if (y0 > y1) {
            swap(&x0, &x1);
            swap(&y0, &y1);
        }
        float a = (dy == 0) ? 0 : dx / dy;
        float x = x0;
        for (int y = y0; y <= y1; y++) {
            set_pixel(c, (int)roundf(x), y, color);
            x += a;
        }
    }
}

void interpolate(int i0, int d0, int i1, int d1, int *values) {
    if (!values) return;
    
    // Single-step guard: handle flat lines without segfaulting
    if (i0 == i1) {
        values[0] = d0;
        return;
    }
    
    float a = (float)(d1 - d0) / (float)(i1 - i0);
    for (int i = i0; i <= i1; i++) {
        values[i - i0] = (int)roundf(d0 + a * (i - i0));
    }
}

void draw_triangle_wireframe(Canvas *c, int x0, int y0, int x1, int y1, int x2, int y2, Pixel color) {
    draw_better_line(c, x0, y0, x1, y1, color);
    draw_better_line(c, x1, y1, x2, y2, color);
    draw_better_line(c, x2, y2, x0, y0, color);
}

void draw_filled_triangle(Canvas *c, int x0, int y0, int x1, int y1, int x2, int y2, Pixel color) {
    // 1. Sort vertices vertically so y0 <= y1 <= y2
    if (y0 > y1) { swap(&x0, &x1); swap(&y0, &y1); }
    if (y0 > y2) { swap(&x0, &x2); swap(&y0, &y2); }
    if (y1 > y2) { swap(&x1, &x2); swap(&y1, &y2); }

    // Edge case: flat line triangle (no height)
    if (y0 == y2) return;

    // Calculate edge heights
    int h02 = y2 - y0 + 1;
    int h01 = y1 - y0 + 1;
    int h12 = y2 - y1 + 1;

    // Buffer memory for edge x-coordinates
    int x02[h02];
    int x01[h01];
    int x12[h12];
    printf("x02 size: %d, x01 size: %d, x12 size: %d\n", h02, h01, h12);

    // 2. Compute x-coordinates along the 3 edges
    interpolate(y0, x0, y2, x2, x02); // Long side
    interpolate(y0, x0, y1, x1, x01); // Short side 1
    interpolate(y1, x1, y2, x2, x12); // Short side 2

    // 3. Concatenate short sides into one continuous array x012
    int x012[h02];
    for (int i = 0; i < h01 - 1; i++) {
        x012[i] = x01[i];
    }
    for (int i = 0; i < h12; i++) {
        x012[h01 - 1 + i] = x12[i];
    }

    // 4. Determine left vs. right edge arrays
    int *x_left, *x_right;
    int mid = h02 / 2;
    if (x02[mid] < x012[mid]) {
        x_left = x02;
        x_right = x012;
    } else {
        x_left = x012;
        x_right = x02;
    }

    // 5. Fill interior scanlines row by row using set_pixel
    for (int y = y0; y <= y2; y++) {
        int idx = y - y0;
        for (int x = x_left[idx]; x <= x_right[idx]; x++) {
            set_pixel(c, x, y, color);
        }
    }
}

void draw_shaded_outlined_triangle(Canvas *c, 
                                   int x0, int y0, 
                                   int x1, int y1, 
                                   int x2, int y2, 
                                   Pixel fill_color, 
                                   Pixel outline_color) {
    draw_filled_triangle(c, x0, y0, x1, y1, x2, y2, fill_color);
    draw_triangle_wireframe(c, x0, y0, x1, y1, x2, y2, outline_color);
}

void triangle_shading(Canvas *c, int x0, int y0, int x1, int y1, int x2, int y2, Pixel color) {
    if(y0<y1) { swap(&x0, &x1); swap(&y0, &y1); }
    if(y0<y2) { swap(&x0, &x2); swap(&y0, &y2); }
    if(y1<y2) { swap(&x1, &x2); swap(&y1, &y2); }

    if(y0 == y2) return;
    // get the triangles heights
    int h02 = y0 - y2 + 1;
    int h01 = y0 - y1 + 1;
    int h12 = y1 - y2 + 1;
    // instantiate the arrays for the x coordinates of the edges
    int x02[h02];
    int x01[h01];
    int x12[h12];

    interpolate(y2, x2, y0, x0, x02);
    interpolate(y1, x1, y0, x0, x01);
    interpolate(y2, x2, y1, x1, x12);


}