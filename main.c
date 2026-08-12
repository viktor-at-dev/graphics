// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>

// #define WIDTH  800
// #define HEIGHT 800

// typedef struct {
//     uint8_t r;
//     uint8_t g;
//     uint8_t b;
// } Pixel;

// typedef struct {
//     int width;
//     int height;
//     Pixel *pixels;
// } Canvas;

// Canvas* create_canvas(int width, int height) {
//     Canvas *c = malloc(sizeof(Canvas));
//     c->width = width;
//     c->height = height;
//     c->pixels = malloc(sizeof(Pixel) * width * height);
//     return c;
// }

// void clear_canvas(Canvas *c, Pixel color) {
//     for (int i = 0; i < c->width * c->height; i++) {
//         c->pixels[i] = color;
//     }
// }

// void free_canvas(Canvas *c) {
//     free(c->pixels);
//     free(c);
// }

// void set_pixel(Canvas *c, int x, int y, Pixel color) {
//     if (x < 0 || x >= c->width || y < 0 || y >= c->height) {
//         return;
//     }
//     int flipped_y = c->height - 1 - y;
//     int index = flipped_y * c->width + x;
//     c->pixels[index] = color;
// }

// int save_ppm(const Canvas *c, const char *filename) {
//     FILE *fp = fopen(filename, "wb");
//     if (!fp) {
//         perror("Failed to open file");
//         return 0;
//     }
//     fprintf(fp, "P6\n%d %d\n255\n", c->width, c->height);
//     fwrite(c->pixels, sizeof(Pixel), c->width * c->height, fp);
//     fclose(fp);
//     return 1;
// }

// // ATTEMPT #1
// void draw_line_attempt1(Canvas *c, int x0, int y0, int x1, int y1, Pixel color) {
//     for (int x = x0; x <= x1; x++) {
//         float t = (float)(x - x0) / (float)(x1 - x0);
//         int y = y0 + (y1 - y0) * t;
//         set_pixel(c, x, y, color);
//     }
// }

// int main(void) {
//     Canvas *canvas = create_canvas(WIDTH, HEIGHT);

//     Pixel bg = {30, 30, 30};
//     clear_canvas(canvas, bg);

//     Pixel red   = {255, 50, 50};
//     Pixel green = {50, 255, 50};
//     Pixel blue  = {50, 150, 255};

//     // Gentle line (Red) - Works fine
//     draw_line_attempt1(canvas, 100, 100, 600, 200, red);

//     // Steep line (Green) - Leaves gaps!
//     draw_line_attempt1(canvas, 200, 100, 250, 600, green);

//     // Right-to-left line (Blue) - Fails completely!
//     draw_line_attempt1(canvas, 700, 700, 300, 600, blue);

//     if (save_ppm(canvas, "output.ppm")) {
//         printf("Rendered output.ppm successfully!\n");
//     }

//     free_canvas(canvas);
//     return 0;
// }