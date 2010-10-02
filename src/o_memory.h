#ifndef O_MEMORY_H_
#define O_MEMORY_H_
#include <stdlib.h>

/*! \brief Alloc a new memory area.
 *
 */
void * o_malloc(size_t size);

/*! \brief Free a memory area.
 *
 */
void o_free(void * ptr);

#endif /* O_MEMORY_H_ */
