#include <stdio.h>
#include <string.h>

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
}