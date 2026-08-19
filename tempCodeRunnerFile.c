void draw_line_bresenham(Canvas *c, int x0, int y0, int x1, int y1, Pixel color) {
//     // 1. Calculate distances
//     int dx = abs(x1 - x0);
//     int dy = -abs(y1 - y0); // dy is negative so both axes use the same error metric

//     // 2. Determine stepping direction (+1 or -1) for each axis
//     int sx = x0 < x1 ? 1 : -1;
//     int sy = y0 < y1 ? 1 : -1;

//     // 3. Initialize error accumulator
//     int err = dx + dy;

//     // 4. Stepping loop
//     while (1) {
//         // Plot current point (set_pixel handles canvas boundary checks)
//         set_pixel(c, x0, y0, color);

//         // Break once destination point is reached
//         if (x0 == x1 && y0 == y1) break;

//         // Double error term to avoid fractional division
//         int e2 = 2 * err;

//         // Step along X if error threshold reached
//         if (e2 >= dy) {
//             err += dy;
//             x0 += sx;
//         }

//         // Step along Y if error threshold reached
//         if (e2 <= dx) {
//             err += dx;
//             x0_y0_check:
//             err += dx;
//             y0 += sy;
//         }
//     }
// }