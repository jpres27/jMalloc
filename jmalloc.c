#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/resource.h>

#define HEAP_CAPACITY 640000
#define MANIFEST_CAPACITY 4096

typedef struct {
    void *start;
    size_t size;
    bool free;
} Manifest;

// These are deprecated and need to be rethought
char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

Manifest allocated[MANIFEST_CAPACITY] = {0};
size_t allocated_size = 0;

void *heap_alloc() {
    void *heap = mmap(NULL, HEAP_CAPACITY, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return heap;
}

void *jmalloc(size_t size) {
    if (size > 0) {
        assert(heap_size + size <= HEAP_CAPACITY);
        void *result = heap + heap_size;
        heap_size += size;

        const Manifest bucket = {
            .start = result,
            .size = size,
            .free = false,
        };
        assert(allocated_size < MANIFEST_CAPACITY);
        allocated[allocated_size++] = bucket;
        
        return result;
    } else return NULL;
}

void print_allocated(void) {
    printf("Allocated buckets (%zu):\n", allocated_size);
    for (size_t i = 0; i < allocated_size; i++) {
        printf(" start: %p, size: %zu\n", allocated[i].start, allocated[i].size);
    }
}

void jfree(void *ptr) {
    (void) ptr;
    assert(false && "TODO: implement jFree");
}

int main() {
    for (int i = 0; i < 32; i++) {
        jmalloc(i);
    }
    print_allocated();

    return 0;
}

