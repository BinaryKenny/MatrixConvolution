#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int WH_SIZE = 4;
const int DW_DH_SIZE = 2;

int parseSizes(char * beg, char * end) //end - pointer across the end
{
  int size = end - beg;
  char * buffer = malloc(size + 1);
  if (buffer == NULL) {
    printf("\t<ERROR> : a buffer was not allocated\n");
    return 1;
  }
  while (beg <= end) {
    buffer[0] = *(--end);
  }
  buffer[0] = '\0';
  int result = atoi(buffer);
  free(buffer);
  return result;
}

int main(int argc, char * argv[]) {
  if (argc < 5) {
    printf("\t<ERROR> : not enough arguments\n");
    return 1;
  }
  FILE * input;
  FILE * output;
  if (strcmp(argv[1], "-i") == 0) {
    input = fopen(argv[2], "r");
    if (strcmp(argv[3], "-o") == 0) {
      output = fopen(argv[4], "w");
    }
    else {
      fclose(input);
      printf("\t<ERROR> : invalid arguments\n");
      return 1;
    }
  }
  else if (strcmp(argv[1], "-o") == 0) {
    output = fopen(argv[2], "w");
    if (strcmp(argv[3], "-i") == 0) {
      input = fopen(argv[4], "r");
    }
    else {
      fclose(output);
      printf("\t<ERROR> : invalid arguments\n");
      return 1;
    }
  }
  if (input == NULL) {
    printf("\t<ERROR> : cannot open input file\n");
    return 1;
  }
  if (output == NULL) {
    printf("\t<ERROR> : cannot open output file\n");
    return 1;
  }
  //find out input file's length
  fseek(input, 0, SEEK_END);
  long size = ftell(input);
  fseek(input, 0, SEEK_SET);
  char * buffer = malloc(size + 1);
  if (buffer == NULL) {
    printf("\t<ERROR> : a buffer was not allocated\n");
    return 1;
  }
  fread(buffer, 1, size, input);
  buffer[size] = '\0';

  //the begin of parsing matrix;
  int height = parseSizes(buffer, buffer + WH_SIZE);
  buffer += WH_SIZE;
  int width = parseSizes(buffer, buffer + DW_DH_SIZE);
  int * A = malloc(width * height);
  int * B = malloc(width * height);
  int * C = malloc(width * height);
  if (A == NULL || B == NULL || C == NULL) {
    printf("\t<ERROR> : not enough memory\n");
    return 2;
  }
  for (size_t i = 0; i < height; i++) {
    for (size_t j = 1; j <= width; j++) {
      A[i * width + j - 1] = (int)buffer[5 + i * j];
      B[i * width + j - 1] = (int)buffer[5 + i * j + 1];
      C[i * width + j - 1] = (int)buffer[5 + i * j + 2];
    }
  }
}