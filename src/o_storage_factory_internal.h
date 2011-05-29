#ifndef O_STORAGE_FACTORY_INTERNAL_H_
#define O_STORAGE_FACTORY_INTERNAL_H_
#include "o_storage_factory.h"

struct o_storage_factory_class
{
	enum o_url_type (*get_type)(struct o_storage_factory *factory);
	struct o_storage * (*storage_open)(struct o_storage_factory *factory, char * storage_name, char * username, char * password);
	void (*free)(struct o_storage_factory *factory);
};

struct o_storage_factory
{
	struct o_storage_factory_class *clazz;
	char * path;
	int opened_storage_count;
};

/*! \brief Release the storage to the current factory.
 *
 * \param factory where release.
 * \param storage to release.
 */
void o_storage_factory_release_storage(struct o_storage_factory *factory, struct o_storage * storage);

#endif /* O_STORAGE_FACTORY_INTERNAL_H_ */
