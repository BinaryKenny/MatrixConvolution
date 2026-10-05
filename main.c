#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const int WH_SIZE = 4;
const int DW_DH_SIZE = 2;
const int STD_DISP = 1;

int parse_byte(unsigned char * beg, int size);

int main(int argc, char * argv[])
{
  if (argc < 5) {
    printf("\t<ERROR> : not enough arguments\n");
    return 1;
  }
  FILE * input = NULL;
  FILE * output = NULL;
  if (strcmp(argv[1], "-i") == 0) {
    input = fopen(argv[2], "rb");
    if (strcmp(argv[3], "-o") == 0) {
      output = fopen(argv[4], "wb");
    }
    else {
      fclose(input);
      printf("\t<ERROR> : invalid arguments\n");
      return 1;
    }
  }
  else if (strcmp(argv[1], "-o") == 0) {
    output = fopen(argv[2], "wb");
    if (strcmp(argv[3], "-i") == 0) {
      input = fopen(argv[4], "rb");
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

  int size = 0;
  fseek(input, 0, SEEK_END);
  size = ftell(input);
  fseek(input, 0, SEEK_SET);

  unsigned char * buffer = (unsigned char*)calloc(size + 1, 1);
  if (buffer == NULL) {
    printf("\t<ERROR> : not enough memory\n");
    return 2;
  }
  fread(buffer, 1, size, input);
  buffer[size] = '\0';
  unsigned char * ptr = buffer;

  int height = parse_byte(ptr, WH_SIZE);
  ptr += WH_SIZE;
  int width = parse_byte(ptr, WH_SIZE);
  ptr += WH_SIZE;

  int * A = (int *)calloc(width * height, sizeof(int));
  int * B = (int *)calloc(width * height, sizeof(int));
  int * C = (int *)calloc(width * height, sizeof(int));
  if (A == NULL || B == NULL || C == NULL) {
    printf("\t<ERROR> : not enough memory\n");
    return 2;
  }

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      A[i * width + j] = parse_byte(ptr++, STD_DISP);
      B[i * width + j] = parse_byte(ptr++, STD_DISP);
      C[i * width + j] = parse_byte(ptr++, STD_DISP);
    }
  }

  int d_height = parse_byte(ptr, DW_DH_SIZE);
  ptr += DW_DH_SIZE;
  int d_width = parse_byte(ptr, DW_DH_SIZE);
  ptr += DW_DH_SIZE;
  int * D = (int *)calloc(d_width * d_height, sizeof(int));
  if (D == NULL) {
    printf("\t<ERROR> : not enough memory\n");
    return 2;
  }
  for (size_t i = 0; i < d_height; i++) {
    for (size_t j = 0; j < d_width; j++) {
      D[i * d_width + j] = parse_byte(ptr++, STD_DISP);
    }
  }
}

int parse_byte(unsigned char * beg, int size)
{
  int result = 0;
  memcpy(&result, beg, size);
  return result;
}