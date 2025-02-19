#ifndef IMAGE_MANIP_H
#define IMAGE_MANIP_H

#include "ppm_io.h"

// store PI as a constant
#define PI 3.14159265358979323846

// macro to square a number
#define sq(X) ((X) * (X))

// macro to find the max of a number
#define MAX(a,b) ((a > b) ? (a) : (b))

Image* swap (Image *original);

Image* invert (Image *original);

/* ______zoom_out______
 * "zoom out" an image, by taking a 2x2 square of pixels and averaging
 * each of the three color channels to make a single pixel. If an odd
 * number of rows in original image, we lose info about the bottom row.
 * If an odd number of columns in original image, we lose info about the
 * rightmost column.
 */

Image * zoom_out(Image *original);

/* _______rotate-right________
 * rotate the input image clockwise 90 degrees
 */

Image* rotate_right (Image *original);

/* ________Swirl effect_________
 * Create a whirlpool effect!
 */

Image* swirl(Image* original, float fcx, float fcy, float fs);

/* _______edges________
 * apply edge detection as a grayscale conversion
 * followed by an intensity gradient computation and
 * thresholding
 */

Image* edges(Image* original, float threshold);

#endif
