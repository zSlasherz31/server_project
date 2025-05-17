#ifndef OUTPUT_H
#define OUTPUT_H

#include "../constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> // Работает только на Linux/glibc

char* format_response(char* response,
                       int* va);

int output_to_stdout(const char* format,
                     const char* result);

int output_to_file(const char* filename,
                   const char* format,
                   const int   error_code);

#endif // OUTPUT_H

