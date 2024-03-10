   #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"
#include <math.h>

/* HELPER for grayscale:
 * convert a RGB pixel to a single grayscale intensity;
 * uses NTSC standard conversion
 */
unsigned char pixel_to_gray(const Pixel *p) {
  return (unsigned char)( (0.3 * (double)p->r) +
                          (0.59 * (double)p->g) +
                          (0.11 * (double)p->b) );
}

/* ______grayscale______
 * convert an image to grayscale (NOTE: pixels are still
 * RGB, but the three values will be equal)
 */
void grayscale(Image *im) {
  if (!im || !im->data) {
    fprintf(stderr, "Error:image_manip - grayscale given a bad image pointer\n");
    return;
  }

  for (int r=0; r<im->rows; r++){
    for (int c=0; c<im->cols; c++){
      // find grayscale intensity
      unsigned char grayLevel = pixel_to_gray(&(im->data[(r*im->cols)+c]));

      // set all chanels to it
      im->data[(r*im->cols)+c].r = grayLevel;
      im->data[(r*im->cols)+c].g = grayLevel;
      im->data[(r*im->cols)+c].b = grayLevel;

    }
  }
}

Image* swap (Image *original) {
  Image *im = make_copy(original); 

  for (int i = 0; i < (im->rows) * (im->cols); i++){
        char tempR = im->data[i].r;
        char tempG = im->data[i].g;
        char tempB = im->data[i].b; 

        im->data[i].r = tempG;
        im->data[i].b = tempR;
        im->data[i].g = tempB;
  }

  return im;
}

Image* invert (Image *original){
    Image *im = make_copy(original);
    for (int i = 0; i < (im->rows) * (im->cols); i++){
        im->data[i].r = 255 - im->data[i].r;
        im->data[i].g = 255 - im->data[i].g;
        im->data[i].b = 255 - im->data[i].b;
    }

    return im;
}

/* ______zoom_out______
 * "zoom out" an image, by taking a 2x2 square of pixels and averaging
 * each of the three color channels to make a single pixel. If an odd
 * number of rows in original image, we lose info about the bottom row.
 * If an odd number of columns in original image, we lose info about the
 * rightmost column.
 */
Image * zoom_out(Image * original){
  /* disregard last row/column if total number is odd */
  int tempRows = (original->rows % 2 == 0 ? original->rows : original->rows - 1);
  int tempCols = (original->cols % 2 == 0 ? original->cols : original->cols - 1);

  assert(tempRows % 2 == 0);
  assert(tempCols % 2 == 0);

  Image *im = make_image(tempRows/2, tempCols/2);
  
  // (r * Nc + c)

  Pixel temp;

  float sumRed;
  float sumBlue;
  float sumGreen;

  for(int i = 0; i < tempRows; i+=2){
    for(int j = 0; j < tempCols; j+=2){

      sumRed = (original->data[i*original->cols+j]).r + (original->data[(i+1)*original->cols+j]).r + (original->data[i*original->cols+(j+1)]).r + (original->data[(i+1)*original->cols+(j+1)]).r; 
      sumGreen = (original->data[i*original->cols+j]).g + (original->data[(i+1)*original->cols+j]).g + (original->data[i*original->cols+(j+1)]).g + (original->data[(i+1)*original->cols+(j+1)]).g;     
      sumBlue = (original->data[i*original->cols+j]).b + (original->data[(i+1)*original->cols+j]).b + (original->data[i*original->cols+(j+1)]).b + (original->data[(i+1)*original->cols+(j+1)]).b; 

      temp.r = sumRed/4;
      temp.g = sumGreen/4;
      temp.b = sumBlue/4;

      im->data[(i/2 * original->cols/2+ j/2)] = temp;
             
      /* each i/j pair is the topleft corner of each 2x2 square */

      /* cast char to int for each and average, then recast to char, and put into new img */
    }
  }

  return im;
}

Image* rotate_right (Image *original){
  Image *im = make_image(original->cols, original->rows);
  
  for(int r = 0; r < original->rows; r++){
    for(int c = 0; c < original->cols; c++){
      im->data[(im->cols * c) + (original->rows-1-r)] = original->data[(r*original->cols) + c];
    }
  }

  return im;
}

/* ________Swirl effect_________
 * Create a whirlpool effect!
 */
Image* swirl(Image* original, float fcx, float fcy, float s){
  int cx = (int)fcx;
  int cy = (int)fcy;

  Image *im = make_image(original->rows, original->cols);

  float originalCoord;
  double a;
  int x;
  int y; 

  cx = cx == -1 ? (original->cols)/2 : cx;
  cy = cy == -1 ? (original->rows)/2 : cy;

  for(int r = 0; r < original->rows; r++){ //y is row x is col
    for(int c = 0; c < original->cols; c++){

      a = sqrt(pow(c-cx, 2) + pow(r-cy, 2)) / s;
      x = (int) ((c-cx)*cos(a)-(r-cy)*sin(a)+cx);
      y = (int) ((c-cx)*sin(a)+(r-cy)*cos(a)+cy);
      
      originalCoord = y*im->cols + x;

      if (x < original->cols && x >= 0 && y < original->rows && y >= 0){
        im->data[r * im->cols + c] = original->data[(int)originalCoord];
      } else { // need to initialize all pixels, not just swirled, since make_image does not initialize pixels
        im->data[r * im->cols + c].r = 0;
        im->data[r * im->cols + c].g = 0;
        im->data[r * im->cols + c].b = 0;
      }
    }

  }

  return im;
}

/* _______edges________
 * apply edge detection as a grayscale conversion
 * followed by an intensity gradient computation and
 * thresholding
 */

Image * edges(Image* original, float threshold){
  grayscale(original);
  Image *im = make_copy(original);
  

  /* cycle through interior points: */

  for(int i = 1; i < im->rows - 1; i++){
    for(int j = 1; j < im->cols - 1; j++){
      /* for each interior point: (y,x)*/
      double intensityX = (original->data[i * im->cols + (j+1)].r - original->data[i * im->cols + (j-1)].r)/(2.0);
      double intensityY = (original->data[(i+1)*im->cols + j].r - original->data[(i-1) * im->cols + j].r)/(2.0);

      double magIntensity = sqrt(intensityX * intensityX + intensityY * intensityY);

      if(magIntensity > threshold){
        im->data[i * im->cols + j].r = 0;
        im->data[i * im->cols + j].g = 0;
        im->data[i * im->cols + j].b = 0;
      } else {
        im->data[i * im->cols + j].r = 255;
        im->data[i * im->cols + j].g = 255;
        im->data[i * im->cols + j].b = 255;
      }
    }
  }

  return im;

}
