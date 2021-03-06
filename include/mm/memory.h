#ifndef MEMORY_H
#define MEMORY_H

#define KERNEL_HEAP_START	CONFIG_KERNEL_HEAP_START
#define KERNEL_HEAP_END		CONFIG_KERNEL_HEAP_END
#define MAX_KERNEL_HEAP_SIZE	(KERNEL_HEAP_END - KERNEL_HEAP_START)

#define USER_HEAP_START		CONFIG_USER_HEAP_START
#define USER_HEAP_END		CONFIG_USER_HEAP_END
#define MAX_USER_HEAP_SIZE	(USER_HEAP_END - USER_HEAP_START)

#ifdef CONFIG_TLSF

#include <tlsf.h>

extern tlsf_t tlsf_mem_kernel_pool;
extern tlsf_t tlsf_mem_user_pool;

#else

#define CHUNK_PER_BLOCK		32
#define CHUNK_SIZE		(1 << 4)
#define MAGIC			0xABCD
#define BLOCK_SIZE		(CHUNK_PER_BLOCK * CHUNK_SIZE)

#define KERNEL_NUM_BLOCKS	((KERNEL_HEAP_END - KERNEL_HEAP_START) / BLOCK_SIZE)

#define MASK(n) ((unsigned int)((1UL << (n)) - 1))

struct alloc_header
{
	unsigned int magic;
	unsigned int chunks;
};

struct memory_block
{
	unsigned int free_chunks;
	unsigned int free_mask;
};

struct memory_block kernel_heap[KERNEL_NUM_BLOCKS];

static inline int addr_to_chunks_offset(void *addr, void *base)
{
	return (((unsigned int *)addr - (unsigned int *)base) % BLOCK_SIZE) / CHUNK_SIZE;
}

static inline int is_free(unsigned int free_mask, unsigned int mask)
{
	return ((free_mask & mask) == 0);
}

static inline int addr_to_block(void *addr, void *base)
{
	return ((unsigned int *)addr - (unsigned int *)base) / BLOCK_SIZE;
}

static inline void *to_addr(unsigned int index, unsigned int chunk_offset, void *base)
{
	return (void *)((unsigned int *)base + index * BLOCK_SIZE + chunk_offset * CHUNK_SIZE);
}
#endif

#endif /* MEMORY_H */
