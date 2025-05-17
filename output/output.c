#include "output.h"

char* format_response(char* response,
                       int* va) {
  size_t additional_bytes = NUMBER_COUNT * (BYTES_PER_NUMBER  -
                            CONVERSION_SPECIFICATION_LENGTH);
  char *formatted = (char*)malloc(malloc_usable_size(response)
                                  + additional_bytes);
  if (formatted)
    sprintf(formatted, response, va[0], va[1], va[2], va[3],
                                 va[4], va[5], va[6], va[7],
                                 va[8], va[9], va[10], va[11],
                                 va[12], va[13], va[14], va[15],
                                 va[16], va[17]);
  return formatted;
}

int output_to_stdout(const char* format,
                     const char* result) {
  return printf(format, result); 
}

int output_to_file(const char* filename,
                   const char* format,
                   const int   error_code) {
  FILE *file = fopen(filename, "a");
  int state_result = -1;

  if (file) {
    fprintf(file, format, error_code);
    fclose(file);
    state_result = 0;
  }
  
  return state_result;
}

