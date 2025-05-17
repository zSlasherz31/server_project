#ifndef CACHE_H
#define CACHE_H

#include <string.h>
#include <stdlib.h>

typedef char* (*ReadFunc)(const char*);
typedef struct {
  char *filename;
  char *file;
} CachedFile;

void init_cache(CachedFile* cache,
                     size_t cache_size);

void clear_cache(CachedFile* cache,
                      size_t cache_size);

char* cache_file(CachedFile* cache,
                 size_t      cache_size,
                 const char* filename,
                 ReadFunc    func_read);

#endif // CACHE_H

