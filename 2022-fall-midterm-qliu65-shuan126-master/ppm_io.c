#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "ppm_io.h"



/* helper function for read_ppm, takes a filehandle
 * and reads a number, but detects and skips comment lines
 */
int read_num(FILE *fp) {
  assert(fp);

  int ch;
  while((ch = fgetc(fp)) == '#') { // # marks a comment line
    while( ((ch = fgetc(fp)) != '\n') && ch != EOF ) {
      /* discard characters til end of line */
    }
  }
  ungetc(ch, fp); // put back the last thing we found

  int val;
  if (fscanf(fp, "%d", &val) == 1) { // try to get an int
    while(isspace(ch = fgetc(fp))) {
      // drop trailing whitespace
    }
    ungetc(ch, fp);
    return val; // we got a value, so return it
  } else {
    fprintf(stderr, "Error:ppm_io - failed to read number from file\n");
    return -1;
  }
}


Image * read_ppm(FILE *fp) {

  /* confirm that we received a good file handle */
  assert(fp != NULL);

  /* allocate image (but not space to hold pixels -- yet) */
  Image *im = malloc(sizeof(Image));
  // * DYNAMIC ALLOCATION

  if (!im) {
    fprintf(stderr, "Error:ppm_io - failed to allocate memory for image!\n");
    return NULL;
  }

  /* initialize fields to error codes, in case we have to bail out early */
  im->rows = im->cols = -1;

  /* read in tag; fail if not P6 */
  char tag[20];
  tag[19]='\0';
  fscanf(fp, "%19s\n", tag);
  if (strncmp(tag, "P6", 20)) {
    fprintf(stderr, "Error:ppm_io - not a PPM (bad tag)\n");
    free(im);
    return NULL;
  }


  /* read image dimensions */
 
  //read in columns
  im->cols = read_num(fp); // NOTE: cols, then rows (i.e. X size followed by Y size)
  //read in rows
  im->rows = read_num(fp);

  //read in colors; fail if not 255
  int colors = read_num(fp);
  if (colors != 255) {
    fprintf(stderr, "Error:ppm_io - PPM file with colors different from 255\n");
    free(im);
    return NULL;
  }

  //confirm that dimensions are positive
  if (im->cols <= 0 || im->rows <= 0) {
    fprintf(stderr, "Error:ppm_io - PPM file with non-positive dimensions\n");
    free(im);
    return NULL;
  }

  /* finally, read in Pixels */

  /* allocate the right amount of space for the Pixels */
  im->data = malloc(sizeof(Pixel) * (im->rows) * (im->cols));

  // NOTE: IN THIS PROJECT, 2D ARRAYS ARE ALLOCATED VIA ONE LARGE ARRAY

  if (!im->data) {
    fprintf(stderr, "Error:ppm_io - failed to allocate memory for image pixels!\n");
    free(im);
    return NULL;
  }

  /* read in the binary Pixel data */
  if (fread(im->data, sizeof(Pixel), (im->rows) * (im->cols), fp) != 
(size_t)((im->rows) * (im->cols))) {
    fprintf(stderr, "Error:ppm_io - failed to read data from file!\n");
    free(im);
    return NULL;
  }

  //return the image struct pointer
  return im;
}



/* Write given image to disk as a PPM.
 * Return -1 if any failure occurs, otherwise return the number of pixels written.
 */
int write_ppm(FILE *fp, const Image *im) {
  // TODO: IMPLEMENT THIS FUNCTION

  /* confirm that we received a good file handle */
  assert(fp != NULL);

  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  /* pixels are wrote to file fp */
  if(fwrite(im->data, sizeof(Pixel), im->rows * im->cols, fp) != (size_t)(im->rows * im->cols)){
    fprintf(stderr, "Error:ppm_io - failed to write data to file!\n");
    return -1;
  } else {
    return im->rows * im->cols;
  }

  // close fp after???????

}


/* allocate a new image of the specified size;
 * doesn't initialize pixel values */
Image * make_image (int rows, int cols) {
  // allocate space
  Image *im = malloc(sizeof(Image));
  if (!im) {
    return NULL;
  }
  // set size 
  im->rows = rows;
  im->cols = cols;

  // allocate pixel array
  im->data = malloc((im->rows * im->cols) * sizeof(Pixel));
  if (!im->data) {
    free(im);
    return NULL;
  }

  return im;
}


/* output dimensions of the image to stdout */
void output_dims(Image *im) {
  printf("cols = %d, rows = %d", im->cols, im->rows);
}

/* free_image - utility function to free inner and outer pointers, 
 * and set to null 
 */
void free_image(Image **im) {
  // TODO: IMPLEMENT THIS FUNCTION
  // first: free data from inside of image 
  free((*im)->data);

  // second: free image pointer itself 
  free(*im);

  return;

}

/* resize_image - utility function to reallocate an image to the specified size;
 * doesn't initialize pixel values 
 * return -1 if error, otherwise 0
 */
int resize_image(Image **im, int r, int c) {
  Image * temp = *im;
  
  temp->rows = r;
  temp->cols = c;

  temp->data = realloc(temp->data, sizeof(Pixel) * (r*c));

  if(!temp->data){
    printf("Error: ppm_io - failed to reallocate image (ppm_io.c -> resize_image()\n");
    free_image(im);
    return -1;
  }
  return 0;
}

/* copy_image
 * utility function to  allocate and fill a new image to be 
 * a copy of the image given as a parameter 
 */
Image* make_copy (Image *orig) {
  // allocate space
  Image *copy = make_image(orig->rows, orig->cols);

  // if we got space, copy pixel values
  if (copy) {
    memcpy(copy->data, orig->data, (copy->rows * copy->cols) * sizeof(Pixel));
  }

  return copy;
}

