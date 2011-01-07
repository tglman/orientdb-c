#ifndef O_MEMORY_H_
#define O_MEMORY_H_
#include <stdlib.h>

/*! \brief Alloc a new memory area.
 *
 *\param size to alloc.
 *\retunr the new allocated data.
 */
void * o_malloc(size_t size);

/*! \brief Enlarge an existing memory or alloc new and copy content.
 *
 * \param cur to expand.
 * \param new size.
 * \return cur or a new allocate memory pointer,
 */
void * o_realloc(void * cur, size_t new_size);

/*! \brief Duplicate a content in a new allocated memory.
 *
 * \param to_dup memory to duplicate.
 * \param dup_len the length of memory to duplicate.
 */
void * o_memdup(void * to_dup, size_t dup_len);

/*! \brief Free a memory area.
 *
 *\param ptr to free.
 */
void o_free(void * ptr);

#endif /* O_MEMORY_H_ */
