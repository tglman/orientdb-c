#ifndef O_EXCEPTION_INTERNAL_H_
#define O_EXCEPTION_INTERNAL_H_
#include "o_exception.h"

struct o_exception
{
	char * message;
	int code;
	char * what_cache;
	void (*free)(struct o_exception *);
	char *(*what)(struct o_exception *);
	int (*match_type)(struct o_exception *,char *type);
};

/*! \brief fill an exception instance with default information.
 *
 * \param exception to fill.
 * \param message of exception
 * \parma code of exception.
 */
void o_exception_internal_init(struct o_exception * exception, char * message, int code);

/*!\brief return if the type match exception.
 *
 * \param exception to check.
 * \param type to verify.
 * \return 1 if match type otherwise 0;
 */
int o_exception_match_exception(struct o_exception *exception, char * type);

/*! \brief dealloc substructure of exception.
 *
 * \param exception to dealloc.
 */
void o_exception_internal_free(struct o_exception * exception);

#endif /* O_EXCEPTION_INTERNAL_H_ */
