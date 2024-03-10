//project.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

int is_all_digits(char str[]){
  int length = strlen(str);

  for (int i = 0; i < length; i++){
    if(i == 0 && str[i] == '-'){
      continue;
    }

    if (!isdigit(str[i]) && (str[i] != '.')){
      return 0;
    }
  }
  return 1;
}

int main(int argc, char* argv[]) {

  // less than 2 command line args means that input or output filename
  // wasn't specified
  if (argc < 3) {
    fprintf(stderr, "Missing input/output filenames\n");
    print_usage();
    return RC_MISSING_FILENAME;
  }

  // TODO: implement the rest of this project!

  FILE * input = fopen(argv[1], "rb");
  if(!input){
    fprintf(stderr, "Failed to open input .ppm file.\n");
    return RC_OPEN_FAILED;
  }

  FILE * output = fopen(argv[2], "wb");
  if(!output){
    fprintf(stderr, "Failed to open output .ppm file.\n");
    return RC_OPEN_FAILED;
  }

  Image * inputImage = read_ppm(input);
  if(!inputImage){
    fprintf(stderr, "Failed to read .ppm file into Image.\n");
    return RC_INVALID_PPM;
  }

  char operation[strlen(argv[3])];
  
  strcpy(operation, argv[3]);

  int error = RC_SUCCESS;

  Image * outputImage;

  /* switch statement to call different method depending on operation */
  if(strcmp(operation, "swap") == 0){
    if(argc > 4){
      fprintf(stderr, "Too many arguments for swap function.\n");
      error = RC_INVALID_OP_ARGS;
    } else {
      outputImage = swap(inputImage);
    }
  } else if(strcmp(operation, "invert") == 0){
    if(argc > 4){
      fprintf(stderr, "Too many arguments for invert function.\n");
      error = RC_INVALID_OP_ARGS;
    } else {
      outputImage = invert(inputImage);
    }

  } else if (strcmp(operation, "rotate-right") == 0){
    if(argc > 4){
      fprintf(stderr, "Too many arguments for rotate-right function.\n");
      error = RC_INVALID_OP_ARGS;
    } else {
      outputImage = rotate_right(inputImage);
    }

  } else if(strcmp(operation, "zoom-out") == 0){
    if(argc > 4){
      fprintf(stderr, "Too many arguments for zoom-out function.\n");
      error = RC_INVALID_OP_ARGS;
    } else {
      outputImage = zoom_out(inputImage);
    }

  } else if(strcmp(operation, "swirl") == 0){
    if(argc < 7){
      fprintf(stderr, "Not enough arguments for swirl function.\n");
      error = RC_INVALID_OP_ARGS;
    } else if(argc > 7){
      fprintf(stderr, "Too many arguments for swirl function.\n");
      error = RC_INVALID_OP_ARGS;
    } else if (is_all_digits(argv[4]) == 0 || is_all_digits(argv[5]) == 0 || is_all_digits(argv[6]) == 0){
      fprintf(stderr, "One or more swirl inputs are of incorrect data type.\n");
      error = RC_OP_ARGS_RANGE_ERR; 
    } else {
      outputImage = swirl(inputImage, atof(argv[4]), atof(argv[5]), atof(argv[6]));
    }
  } else if(strcmp(operation, "edge-detection") == 0){
    if(argc < 5){
      fprintf(stderr, "Not enough arguments for edge detection function.\n");
      error = RC_INVALID_OP_ARGS;
    } else if(argc > 5){
      fprintf(stderr, "Too many arguments for edge detection function.\n");
      error = RC_INVALID_OP_ARGS;
    } else if (is_all_digits(argv[4]) == 0){
      fprintf(stderr, "Edge detection input is of incorrect data type: '%s'.\n", argv[4]);
      error = RC_OP_ARGS_RANGE_ERR;
    } else {
      outputImage = edges(inputImage, atof(argv[4]));
    }

    // check for rc6
  } else {
    fprintf(stderr, "Invalid operation.\n"); 
    free_image(&inputImage);
    fclose(input);
    fclose(output);
    return RC_INVALID_OPERATION;
  }

  if(error == 0){
    if(!write_ppm(output, outputImage)){
      fprintf(stderr, "Failed to write swapped image to output file.\n");
      error = RC_WRITE_FAILED;  
    } 
    free_image(&inputImage);
    free_image(&outputImage);
    fclose(input);
    fclose(output);
  } else {
    free_image(&inputImage);
    fclose(input);
    fclose(output);
  }

  return error;


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
