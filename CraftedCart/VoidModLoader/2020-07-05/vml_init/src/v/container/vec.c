#include "v/container/vec.h"
#include <string.h>

#define VVEC_INIT_CAPACITY 10

void* VVec_new_internal(size_t elem_size) {
    VVec_Meta *mem = VVEC_MALLOC(sizeof(VVec_Meta) + (elem_size * VVEC_INIT_CAPACITY));
    mem->length = 0;
    mem->capacity = VVEC_INIT_CAPACITY;
    mem->elem_size = elem_size;

    return ((char*) mem) + sizeof(VVec_Meta);
}

void VVec_free(void* vec) {
    void *mem = (((char*) vec) - sizeof(VVec_Meta));
    VVEC_FREE(mem);
}

void VVec_grow(void *vec) {
    VVec_Meta *mem = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta));
    mem->capacity *= 2;
    mem = VVEC_REALLOC(mem, sizeof(VVec_Meta) + (mem->elem_size * mem->capacity));
}

void VVec_remove(void *vec, size_t idx) {
    VVec_Meta *meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta));

    // Shift all elements after idx down by one
    size_t len = VVec_length(vec);
    for (size_t i = idx; i < len; i++) {
        memcpy(
                ((char*) vec) + (i * meta->elem_size),
                ((char*) vec) + ((i + 1) * meta->elem_size),
                meta->elem_size
                );
    }

    meta->length--;
}
