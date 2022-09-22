#ifndef VML_V_CONTAINER_VEC_H
#define VML_V_CONTAINER_VEC_H

#include "vml/alloc.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VVEC_MALLOC(size) vml_malloc(size)
#define VVEC_REALLOC(ptr, size) vml_realloc(ptr, size)
#define VVEC_FREE(ptr) vml_free(ptr)

typedef struct VVec_Meta {
    size_t length;
    size_t capacity;
    size_t elem_size;

    // and then the vec data comes after those two fields
} VVec_Meta;

#define VVec(T) T*

void* VVec_new_internal(size_t elem_size);
void VVec_free(void* vec);
#define VVec_new(T) (VVec_new_internal(sizeof(T)))

static inline size_t VVec_capacity(void *vec) {
    VVec_Meta *meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta));
    return meta->capacity;
}

static inline size_t VVec_length(void *vec) {
    VVec_Meta *meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta));
    return meta->length;
}

void VVec_grow(void *vec);

#define VVec_push(vec, item) \
    do { \
        VVec_Meta *meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta)); \
        size_t __cap = VVec_capacity(vec); \
        size_t __len = VVec_length(vec); \
        if (__cap <= __len) { \
            VVec_grow(vec); \
        } \
        \
        vec[VVec_length(vec)] = item; \
        meta->length++; \
    } while (0)

void VVec_remove(void *vec, size_t idx);

#ifdef __cplusplus
}
#endif

#endif

