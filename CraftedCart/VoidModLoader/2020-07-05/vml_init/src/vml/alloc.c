#include "vml/alloc.h"
#include "vml/log.h"
#include <string.h>

/** If TRUE, this will fill memory with 0xAA when calling `vml_alloc_add_block` */
#ifdef NDEBUG
    #define VML_ALLOC_DEBUG 0
#else
    #define VML_ALLOC_DEBUG 1
#endif

typedef struct VmlAllocFreeEntry {
    void *ptr;
    u32 size;
} VmlAllocFreeEntry;

#define FREE_LIST_CAPACITY 256

// TODO: Shove this over to the .bss when cube_code supports it
static VmlAllocFreeEntry free_list[FREE_LIST_CAPACITY] = {
    (VmlAllocFreeEntry) {
        .ptr = (void*) 0xABADCAFE,
        .size = 0,
    },
};
static u32 free_list_size = 1;

void vml_alloc_init(void *start, u32 size) {
#if VML_ALLOC_DEBUG
    memset(start, 0xAA, size);
#endif

    free_list[0].ptr = start;
    free_list[0].size = size;

    free_list_size = 1;
}

void vml_alloc_add_block(void *start, u32 size) {
#if VML_ALLOC_DEBUG
    memset(start, 0xAA, size);
#endif

    free_list[free_list_size].ptr = start;
    free_list[free_list_size].size = size;

    free_list_size++;
}

void vml_alloc_print_free_list() {
    vml_info("Free list:");

    for (u32 i = 0; i < free_list_size; i++) {
        VmlAllocFreeEntry *entry = &free_list[i];

        vml_info("> 0x%08X (+ 0x%X)", entry->ptr, entry->size);
    }
}

u32 vml_alloc_get_free_bytes() {
    u32 free_bytes = 0;
    for (u32 i = 0; i < free_list_size; i++) {
        VmlAllocFreeEntry *entry = &free_list[i];
        free_bytes += entry->size;
    }

    return free_bytes;
}

static VmlAllocFreeEntry* find_free_entry(u32 size) {
    VmlAllocFreeEntry *best_entry = NULL;

    // Look for the smallest free block, to try and reduce fragmentation
    for (u32 i = 0; i < free_list_size; i++) {
        VmlAllocFreeEntry *entry = &free_list[i];

        if ((best_entry == NULL && entry->size >= size) ||
            (best_entry != NULL && entry->size >= size && entry->size < best_entry->size)) {
            best_entry = entry;
        }
    }

    return best_entry;
}

void* vml_malloc(u32 size) {
    // Round to next multiple of 4
    size = (size + 3) & ~0x03;

    // Add 4 as we need space to store the size of the allocation
    size += 4;

    VmlAllocFreeEntry *entry = find_free_entry(size);
    if (entry == NULL) {
        vml_error("Panicking in vml_malloc - printing free list...");
        vml_alloc_print_free_list();
        vml_fatal("Could not vml_malloc with size %u", size - 4);
    }

    void *base_ptr = entry->ptr;
    u32 *size_ptr = base_ptr;
    void *user_ptr = (u8*) base_ptr + 4;

    *size_ptr = size;

    if (entry->size == size) {
        // Get rid of the free block
        free_list_size--;

        VmlAllocFreeEntry *last_entry = &free_list[free_list_size];
        if (entry != last_entry) {
            entry->ptr = last_entry->ptr;
            entry->size = last_entry->size;
        }
    } else {
        // Shrink the free block
        entry->ptr = (u8*) entry->ptr + size;
        entry->size -= size;
    }

    return user_ptr;
}

void* vml_try_malloc(u32 size) {
    // Round to next multiple of 4
    size = (size + 3) & ~0x03;

    // Add 4 as we need space to store the size of the allocation
    size += 4;

    VmlAllocFreeEntry *entry = find_free_entry(size);
    if (entry == NULL) {
        // Allocation failed
        return NULL;
    }

    void *base_ptr = entry->ptr;
    u32 *size_ptr = base_ptr;
    void *user_ptr = (u8*) base_ptr + 4;

    *size_ptr = size;

    if (entry->size == size) {
        // Get rid of the free block
        free_list_size--;

        VmlAllocFreeEntry *last_entry = &free_list[free_list_size];
        if (entry != last_entry) {
            entry->ptr = last_entry->ptr;
            entry->size = last_entry->size;
        }
    } else {
        // Shrink the free block
        entry->ptr = (u8*) entry->ptr + size;
        entry->size -= size;
    }

    return user_ptr;
}

void* vml_realloc(void *ptr, u32 new_size) {
    void *base_ptr = (u8*) ptr - 4;
    u32 *size_ptr = base_ptr;
    u32 size = *size_ptr;

    if (new_size == size) return ptr;

    // TODO: Shrink in-place
    // TODO: Expand in-place if there is space to do so
    void *new_ptr = vml_malloc(new_size);
    memcpy(new_ptr, ptr, size);
    vml_free(ptr);

    return new_ptr;
}

void* vml_try_realloc(void *ptr, u32 new_size) {
    void *base_ptr = (u8*) ptr - 4;
    u32 *size_ptr = base_ptr;
    u32 size = *size_ptr;

    if (new_size == size) return ptr;

    // TODO: Shrink in-place
    // TODO: Expand in-place if there is space to do so
    void *new_ptr = vml_try_malloc(new_size);
    if (new_ptr == NULL)  {
        return NULL;
    }
    memcpy(new_ptr, ptr, size);
    vml_free(ptr);

    return new_ptr;
}

/** Remove an entry from the free list, swapping in the last element in the list if need be */
static void remove_entry(VmlAllocFreeEntry *entry) {
    free_list_size--;

    // Swap with the former last entry
    VmlAllocFreeEntry *last_entry = &free_list[free_list_size];

    if (entry == last_entry) { return; }

    entry->ptr = last_entry->ptr;
    entry->size = last_entry->size;
}

/** Tries to merge the free entry at block_idx with other free blocks, recursively */
static void try_merge_free_blocks(u32 block_idx) {
    VmlAllocFreeEntry *this_entry = &free_list[block_idx];
    void *this_entry_end = (u8*) this_entry->ptr + this_entry->size;

    for (u32 i = 0; i < free_list_size; i++) {
        if (i == block_idx) continue;
        VmlAllocFreeEntry *other_entry = &free_list[i];
        void *other_entry_end = (u8*) other_entry->ptr + other_entry->size;

        if (this_entry_end == other_entry->ptr) {
            // Grow this_entry, get rid of other_entry
            this_entry->size += other_entry->size;
            u8 is_last = this_entry == &free_list[free_list_size - 1];

            remove_entry(other_entry);

            // remove_entry swaps with the last block, hence the check here
            if (is_last) {
                try_merge_free_blocks(i);
            } else {
                try_merge_free_blocks(block_idx);
            }
            return;
        } else if (this_entry->ptr == other_entry_end) {
            vml_alloc_print_free_list();

            // Grow other_entry, get rid of this_entry
            other_entry->size += this_entry->size;
            vml_info("removing block %u starting at 0x%08X", block_idx, this_entry->ptr);
            remove_entry(this_entry);

            try_merge_free_blocks(i);
            return;
        }
    }
}

void vml_free(void *ptr) {
    void *base_ptr = (u8*) ptr - 4;
    u32 *size_ptr = base_ptr;
    u32 size = *size_ptr;

    if (free_list_size >= FREE_LIST_CAPACITY) {
        vml_error("Panicking in vml_free - printing free list...");
        vml_alloc_print_free_list();
        vml_fatal("Could not vml_free pointer %p - the free list has been exhausted", ptr);
    }

    // We can't grow an existing block - create a new one
    VmlAllocFreeEntry *entry = &free_list[free_list_size];
    entry->ptr = base_ptr;
    entry->size = size;
    free_list_size++;

    try_merge_free_blocks(free_list_size - 1);
}
