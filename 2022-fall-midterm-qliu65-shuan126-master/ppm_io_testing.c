#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "image_manip.h"

// Return (exit) codes
#define RC_SUCCESS            0
#define RC_MISSING_FILENAME   1
#define RC_OPEN_FAILED        2
#define RC_INVALID_PPM        3
#define RC_INVALID_OPERATION  4
#define RC_INVALID_OP_ARGS    5
#define RC_OP_ARGS_RANGE_ERR  6
#define RC_WRITE_FAILED       7
#define RC_UNSPECIFIED_ERR    8

void print_usage();

int main(int argc, char* argv[]){

    /* check if no. of cmd line args is valid */
    if (argc < 3) {
        fprintf(stderr, "Missing input/output filenames\n");
        print_usage();
        return RC_MISSING_FILENAME;
    }

    FILE * input = fopen(argv[1], "rb");
    FILE * output = fopen(argv[2], "wb");
    //


    
    
    Image * inputImage = read_ppm(input);
    grayscale(inputImage);

    Image * testImg = edges(inputImage, 12);
    //output_dims(inputImage);
    //output_dims(testImg);
    printf("\n");

    if(write_ppm(output, testImg) == -1){
        printf("Error in writing image to file :(\n");
        return -1;
    }
    /*
    fclose(input);
    fclose(output);
    */

}


void print_usage() {
  printf("USAGE: ./project <input-image> <output-image> <command-name> <command-args>\n");
  printf("SUPPORTED COMMANDS:\n");
  printf("   swap\n");
  printf("   invert\n");
  printf("   zoom-out\n");
  printf("   rotate-right\n");
  printf("   swirl <cx> <cy> <strength>\n");
  printf("   edge-detection <threshold>\n");
}
