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
void swapf(float *a, float *b) {
    float temp = *a;
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
    }0;
    
    float a = (float)(d1 - d0) / (float)(i1 - i0);
    for (int i = i0; i <= i1; i++) {
        values[i - i0] = (int)roundf(d0 + a * (i - i0));
    }
}
void interpolate_float(int i0, float d0, int i1, float d1, float *values) {
    if (!values) return;
    
    // Single-step guard: handle flat lines without segfaulting
    if (i0 == i1) {
        values[0] = d0;
        return;
    }
    
    float a = (d1 - d0) / (float)(i1 - i0);
    for (int i = i0; i <= i1; i++) {
        values[i - i0] = d0 + a * (i - i0);
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

void triangle_shading(Canvas *c, 
                      int x0, int y0, float h0,
                      int x1, int y1, float h1,
                      int x2, int y2, float h2,
                      Pixel color) 
{
    // 1. Sort vertices ascending by Y (y0 <= y1 <= y2)
    if (y0 > y1) { swap(&x0, &x1); swap(&y0, &y1); swapf(&h0, &h1); }
    if (y0 > y2) { swap(&x0, &x2); swap(&y0, &y2); swapf(&h0, &h2); }
    if (y1 > y2) { swap(&x1, &x2); swap(&y1, &y2); swapf(&h1, &h2); }

    if (y0 == y2) return; // Flat zero-height triangle

    // 2. Segment Heights
    int h02 = y2 - y0 + 1;
    int h01 = y1 - y0 + 1;
    int h12 = y2 - y1 + 1;

    // 3. Allocate Arrays (x-coordinates AND float intensities)
    int x02[h02], x01[h01], x12[h12];
    float h02_arr[h02], h01_arr[h01], h12_arr[h12];

    // 4. Interpolate X coordinates
    interpolate(y0, x0, y1, x1, x01);
    interpolate(y1, x1, y2, x2, x12);
    interpolate(y0, x0, y2, x2, x02);

    // 5. Interpolate Intensity Values (using float interpolation)
    interpolate_float(y0, h0, y1, h1, h01_arr);
    interpolate_float(y1, h1, y2, h2, h12_arr);
    interpolate_float(y0, h0, y2, h2, h02_arr);

    // 6. Concatenate Short Edges (X and Intensity)
    int x012[h02];
    float h012_arr[h02];

    for (int i = 0; i < h01 - 1; i++) {
        x012[i] = x01[i];
        h012_arr[i] = h01_arr[i];
    }
    for (int i = 0; i < h12; i++) {
        x012[h01 - 1 + i] = x12[i];
        h012_arr[h01 - 1 + i] = h12_arr[i];
    }

    // 7. Determine Left vs. Right Rail
    int *x_left, *x_right;
    float *h_left, *h_right;
    int mid = h02 / 2;

    if (x02[mid] < x012[mid]) {
        x_left = x02;  h_left = h02_arr;
        x_right = x012; h_right = h012_arr;
    } else {
        x_left = x012; h_left = h012_arr;
        x_right = x02;  h_right = h02_arr;
    }

    // 8. Scanline Rasterization with Horizontal Intensity Interpolation
    for (int y = y0; y <= y2; y++) {
        int idx = y - y0;
        int xl = x_left[idx];
        int xr = x_right[idx];
        
        float hl = h_left[idx];
        float hr = h_right[idx];

        // Interpolate intensity across the current scanline row
        int row_width = xr - xl + 1;
        float h_row[row_width > 0 ? row_width : 1];
        interpolate_float(xl, hl, xr, hr, h_row);

        for (int x = xl; x <= xr; x++) {
            float intensity = h_row[x - xl];
            
            // Scale color channels by intensity (0.0 to 1.0)
            Pixel shaded_color = {
                (uint8_t)(color.r * intensity),
                (uint8_t)(color.g * intensity),
                (uint8_t)(color.b * intensity)
            };

            set_pixel(c, x, y, shaded_color);
        }
    }
}