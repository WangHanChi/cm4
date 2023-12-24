#include <stddef.h>

extern char end;  // Defined by the linker

static char *heap_end = &end;

void *_sbrk(ptrdiff_t incr)
{
    char *prev_heap_end = heap_end;

    // Check for heap collision
    if (heap_end + incr > &end) {
        // Heap collision occurred, return (void*)-1 as an error indicator
        return (void *) -1;
    }

    heap_end += incr;
    return (void *) prev_heap_end;
}