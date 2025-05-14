#include "input.h"

char* read_file(const char* filename) {
  FILE *file = fopen(filename, "rb");
  char *result = NULL;

  if (file) {
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    result = (char*)malloc(size + 1);
    if (result) {
      fread(result, 1, size, file);
      result[size] = '\0';
    }
    fclose(file);
  }
  
  return result;
}

