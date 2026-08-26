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

void draw_poor_line(Canvas *c, int x0, int y0, int x1, int y1, Pixel color){
    if(x1 < x0){
        swap(&x0,&x1);
        swap(&y0,&y1);
    }
    float a = (float)(y1 - y0)/(float)(x1 - x0);
    float y = y0;
    for(int x = x0;x <= x1;x++){
        set_pixel(c,x,(int)y,color);
        y += a;
    }
}
void drawline(Canvas *c,int x0, int y0, int x1, int y1, Pixel color){
    if(y1 < y0){
        swap(&x0,&x1);
        swap(&y0,&y1);
    }
    float a = (float)(x1 - x0)/(float)(y1 - y0);
    float x = x0;
    for(int y = y0; y <=y1;y++){
        set_pixel(c,(int)x,y,color);
        x += a;
    }
}
void draw_better_line(Canvas *c, int x0, int y0, int x1, int y1, Pixel color){
    float dx = x1 -x0;
    float dy = y1-y0;
    if(abs(dx)>abs(dy)){
        if(x0 >x1){
            swap(&x0,&x1);
            swap(&y0,&y1);
        }
        float a = dy/dx;
        float y = y0;
        for(int x = x0;x <= x1;x++){
            set_pixel(c,x,(int)y,color);
            y += a;
        }
    }else{
        if(y0 > y1){
            swap(&x0,&x1);
            swap(&y0,&y1);
        }
        float a = dy/dx;
        float x = x0;
        for(int y = y0;y <= y1;y++){
            set_pixel(c,(int)x,y,color);
            x += a;
        }
    }
}
void interpolate(int i0, int d0, int i1, int d1){
    if(i0 == i1){
        return;
    };
    int count = i1 - i0 + 1;
    float values[count];
    float a = (float)(d1 - d0)/(float)(i1 - i0);
    for(int i = i0;i <= i1;i++){
        values[i - i0] = d0 + a*(i - i0);
    }
}
void drawline_with_interpolation(Canvas *c,int x0, int y0, int x1, int y1, Pixel color){
    if(abs(x1 - x0)> abs(y1-y0)){
        if(x0 > x1){
            swap(&x0,&x1);
            swap(&y0,&y1);
        };
       interpolate(x0, y0, x1, y1);
        for(int x = x0;x <=x1;x++){
            float t = (float)(x-x0)/(float)(x1-x0);
            int y = y0 + (int)((y1 - y0)*t);
            set_pixel(c,x,y,color);
        }
    }else{
        if(y0 > y1){
            swap(&x0,&x1);
            swap(&y0,&y1);
        };
        interpolate(y0, x0, y1, x1);
        for(int y = y0;y <=y1;y++){
            float t = (float)(y-y0)/(float)(y1-y0);
            int x = x0 + (int)((x1 - x0)*t);
            set_pixel(c,x,y,color);
        }
    }
}
void draw_wireframe(Canvas *c,int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Pixel color){
    drawline_with_interpolation(c,x0,y0,x1,y1,color);
    drawline_with_interpolation(c,x1,y1,x2,y2,color);
    drawline_with_interpolation(c,x2,y2,x3,y3,color);
}