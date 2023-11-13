#include <sys/mman.h>
#include <sys/resource.h>

void jMalloc(size_t size);
void jFree(void ptr);
void jRealloc(void ptr, size_t size);
void jCalloc(size_t count, size_t size);


