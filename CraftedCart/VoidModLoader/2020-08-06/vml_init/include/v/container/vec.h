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

/** For internal usage: the struct stored before the VVec pointer */
typedef struct VVec_Meta {
    size_t length;
    size_t capacity;
    size_t elem_size;

    // and then the vec data comes after those two fields
} VVec_Meta;

/**
 * A generic vector type.
 *
 * @code
 * VVec(char*) vec = VVec_new(char*);
 * @endcode
 */
#define VVec(T) T*

void* VVec_new_internal(size_t elem_size);

/**
 * Allocate a new VVec that stores items of type T
 *
 * @code
 * VVec(char*) vec = VVec_new(char*);
 * @endcode
 */
#define VVec_new(T) (VVec_new_internal(sizeof(T)))

/** Free all memory used by a VVec */
void VVec_free(void* vec);

/** Returns the number of items the VVec can store before needing to allocate a larger memory area */
static inline size_t VVec_capacity(void *vec) {
    VVec_Meta *meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta));
    return meta->capacity;
}

/** Returns the number of items stored in the VVec */
static inline size_t VVec_length(void *vec) {
    VVec_Meta *meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta));
    return meta->length;
}

/** For internal usage: reallocates the memory used for the VVec */
void VVec_grow(void *vec);

/** Add an item to the end of a VVec */
#define VVec_push(vec, item) \
    do { \
        VVec_Meta *__meta = (VVec_Meta*) (((char*) vec) - sizeof(VVec_Meta)); \
        size_t __cap = VVec_capacity(vec); \
        size_t __len = VVec_length(vec); \
        if (__cap <= __len) { \
            VVec_grow(vec); \
        } \
        \
        vec[VVec_length(vec)] = item; \
        __meta->length++; \
    } while (0)

/** Remove an item by index from the VVec, shifting all further items down by one */
void VVec_remove(void *vec, size_t idx);

#ifdef __cplusplus
}
#endif

#endif

