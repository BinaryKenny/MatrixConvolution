#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const int WH_SIZE = 4;
const int DW_DH_SIZE = 2;
const int STD_DISP = 1;

int parse_byte(unsigned char * beg, int size);
int * convolution(int * A, int wA, int hA,int * D, int wD, int hD, int * result);

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
    fclose(input);
    fclose(output);
    return 2;
  }
  size_t read = fread(buffer, 1, size, input);
  if (read != size) {
    printf("\t<ERROR> : garbage read\n");
    free(buffer);
    fclose(input);
    fclose(output);
    return 2;
  }
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
    free(A);
    free(B);
    free(C);
    free(buffer);
    fclose(input);
    fclose(output);
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
    free(A);
    free(B);
    free(C);
    free(buffer);
    fclose(input);
    fclose(output);
    return 2;
  }
  for (size_t i = 0; i < d_height; i++) {
    for (size_t j = 0; j < d_width; j++) {
      D[i * d_width + j] = (signed char)*ptr++;
    }
  }
  fclose(input);
  free(buffer);
  ptr = NULL;
  int * conv_A = (int *)malloc(width * height * sizeof(int));
  int * conv_B = (int *)malloc(width * height * sizeof(int));
  int * conv_C = (int *)malloc(width * height * sizeof(int));
  if (conv_A == NULL || conv_B == NULL || conv_C == NULL) {
    printf("\t<ERROR> : not enough memory\n");
    free(A);
    free(B);
    free(C);
    free(D);
    free(conv_A);
    free(conv_B);
    free(conv_C);
    fclose(output);
    return 2;
  }
  conv_A = convolution(A, width, height, D, d_width, d_height, conv_A);
  conv_B = convolution(B, width, height, D, d_width, d_height, conv_B);
  conv_C = convolution(C, width, height, D, d_width, d_height, conv_C);
  fwrite(&height, sizeof(int), 1, output);
  fwrite(&width, sizeof(int), 1, output);
  for (size_t i = 0; i < width * height; i++) {
    fwrite(&conv_A[i], 1, 1, output);
    fwrite(&conv_B[i], 1, 1, output);
    fwrite(&conv_C[i], 1, 1, output);
  }
  free(A);
  free(B);
  free(C);
  free(D);
  free(conv_A);
  free(conv_B);
  free(conv_C);
  fclose(output);
  return 0;
}

int parse_byte(unsigned char * beg, int size)
{
  int result = 0;
  memcpy(&result, beg, size);
  return result;
}

int * convolution(int * A, int wA, int hA,int * D, int wD, int hD, int * result)
{
  int dw_centre = wD / 2;
  int dh_centre = hD / 2;
  for (int i = 0; i < hA; i++) {
    for (int j = 0; j < wA; j++) {
      long long int tempResult = 0;
      for (int k = 0; k < hD; k++) {
        for (int z = 0; z < wD; z++) {
          int i_A = i + k - dh_centre;
          int j_A = j + z - dw_centre;
          if (i_A >= 0 && j_A >= 0 && i_A < hA && j_A < wA) {
            tempResult += A[i_A * wA + j_A] * D[k * wD + z];
          }
        }
      }
      if (tempResult > 255) {
        tempResult %= 251;
      }
      else if (tempResult < 0) {
        tempResult = -tempResult;
        tempResult %= 241;
      }
      result[i * wA + j] = tempResult;
    }
  }
  return result;
}