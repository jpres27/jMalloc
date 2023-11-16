#include <cstdint>
#include <cstdio>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <cassert>


using word_t = intptr_t;

struct Block {
    size_t size;
    bool free;
    Block *next;

    //Payload pointer
    word_t data[1];
};

static Block *heapStart = nullptr;
static auto top = heapStart;

// Ensures word alignment on all allocation request sizes
inline size_t align(size_t sizeToAlign) {
    return (sizeToAlign + sizeof(word_t) - 1) & ~(sizeof(word_t) -1);
}

// Returns an allocation size minus the single word which is allocated during the creation of a Block, eg data[1]
// Included in this is space for the Block metadata
// sizeof(std::declval<Block>().data) == sizeof(word_t)
inline size_t allocSize(size_t size) {
    return size + sizeof(Block) - sizeof(std::declval<Block>().data);
}

Block *getHeader(word_t *data) {
    return (Block *)((char *)data + sizeof(std::declval<Block>().data) - sizeof(Block));
}


Block *requisition(size_t size) {
    //Current heap break
    auto block = (Block *) sbrk(0);
    if(sbrk(allocSize(size)) == (void *) - 1) {
        return nullptr;
    }
    return block;
}

Block *findBlock(size_t size) {
    auto block = heapStart;
    while(block != nullptr) {
        if(!block->free || block->size < size) {
            block = block->next;
            continue;
        }
        return block;
    }
    return nullptr;
}

word_t *alloc(size_t size) {
    size = align(size);

    if(auto block = findBlock(size)) {
        return block->data;
    }

    auto block = requisition(size);
    block->size = size;
    block->free = false;

    if(heapStart == nullptr) {
        heapStart = block;
    }
    if (top != nullptr) {
        top->next = block;
    }

    top = block;
    return block->data;
}

void free(word_t *data) {
    auto block = getHeader(data);
    block->free = true;
}

int main() {
    
    auto p1 = alloc(3);
    auto p1b = getHeader(p1);
    assert(p1b->size == sizeof(word_t));
    
    auto p2 = alloc(8);
    auto p2b = getHeader(p2);
    assert(p2b->size == 8);
    
    free(p2);
    assert(p2b->free == true);

    auto p3 = alloc(8);
    auto p3b = getHeader(p3);
    assert(p3b->size == 8);
    assert(p3b == p2b);

    puts("\nAll assertions passed!\n");

}