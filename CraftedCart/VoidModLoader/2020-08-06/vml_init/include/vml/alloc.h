#ifndef VML_VML_INIT_ALLOC_H
#define VML_VML_INIT_ALLOC_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize allocation with a region of memory that's allowed to be used as a heap */
void vml_alloc_init(void *start, u32 size);

/** Add a block of memory allowed to be used as a heap */
void vml_alloc_add_block(void *start, u32 size);

/** Logs the list of free blocks of memory */
void vml_alloc_print_free_list(void);

/** Get the number of free bytes available */
u32 vml_alloc_get_free_bytes(void);

/** Try and allocate memory - panics if allocation failed */
void* vml_malloc(u32 size);

/** Try and allocate memory - returns NULL if allocation failed */
void* vml_try_malloc(u32 size);

/** Try and resize a memory allocation - panics if reallocation failed */
void* vml_realloc(void *ptr, u32 new_size);

/** Try and resize a memory allocation - returns NULL if reallocation failed */
void* vml_try_realloc(void *ptr, u32 new_size);

/** Try and free memory - panics if free failed */
void vml_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
