#include "../../include/cache/cache.h"

void init_cache(CachedFile* cache,
                     size_t cache_size) {
  for (size_t file_num = 0; file_num < cache_size; ++file_num)
    cache[file_num].filename = cache[file_num].file = NULL;
}

void clear_cache(CachedFile* cache,
                      size_t cache_size) {
  for (size_t file_num = 0; file_num < cache_size; ++file_num) {
    free(cache[file_num].filename);
    free(cache[file_num].file);
  }
}

char* cache_file(CachedFile* cache,
                 size_t      cache_size,
                 const char* filename,
                 ReadFunc    func_read) {
  
  // Указатель на файл (взятый из кеша, если он там будет)
  char *result = NULL;

  size_t file_num = 0;
  CachedFile cached_file;
  char *file;

  while (!result && file_num < cache_size) {
    cached_file = cache[file_num];
    file = cached_file.file;
    if (file) {
      if (!strcmp(filename, cached_file.filename))
        // Если файл есть в кеше — возвращаем его
        result = file;
    }
    else {
      result = func_read(filename);
      if (result) {
        size_t filename_size = strlen(filename) + 1;
        // Кешируем считанный файл
        cache[file_num].filename = memcpy((char*)malloc(filename_size),
                                          filename, filename_size);  
        cache[file_num].file = result;
      }
      else
        // Если файл не считался — заканчиваем перебор кеша
        file_num = cache_size;
    }

    ++file_num;
  }
  
  return result;
}

