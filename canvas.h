#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct{
    int width;
    int height;
    Pixel *pixels;
} Canvas;

// We need to allocate memory to the canvas and its pixels. The function returns a pointer to the newly created canvas.
Canvas *create_canvas(int width, int height){
    Canvas *c = malloc(sizeof(Canvas));
    if (c == NULL) {
        return NULL; // Handle memory allocation failure
    }
    c->width = width;
    c->height = height;
    c->pixels = malloc(sizeof(Pixel) * width * height);
    if (c->pixels == NULL) {
        free(c); // Free the previously allocated canvas if pixel allocation fails
        return NULL; // Handle memory allocation failure
    }
    return c;
}
void clear_canvas(Canvas *c, Pixel color){
    for (int i =0; i < c->width *c->height; i++ ){
        c->pixels[i] = color;
    }
};
void free_canvas(Canvas *c){
    free(c->pixels);
    free(c);
};
void set_pixel(Canvas *c, int x, int y, Pixel color){
    if(x<0 || x >= c->width || y <0 || y >= c-> height){
        return; // Out of bounds, do nothing
    }
    int flipped_y = c->height -1 - y; // Flip the y-coordinate
    int index = flipped_y * c->width + x;
    c->pixels[index] = color;
}
int save_ppm(const Canvas *c, const char *filename){
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file");
        return 0; // Return 0 to indicate failure
    }
    fprintf(fp, "P6\n%d %d\n255\n", c->width, c->height);
    fwrite(c->pixels, sizeof(Pixel), c->width * c->height, fp);
    fclose(fp);
    return 1; // Return 1 to indicate success
}