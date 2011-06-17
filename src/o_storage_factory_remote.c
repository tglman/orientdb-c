#include "o_storage_factory_remote.h"
#include "o_storage_factory_internal.h"
#include "o_memory.h"
#include "o_pool.h"
#include "o_storage_remote.h"
#include "o_native_socket_selector.h"
#include "o_native_lock.h"

struct o_storage_factory_remote
{
	struct o_storage_factory base_factory;
	struct o_native_socket_selector * selector;
	struct o_pool * connection_pool;
	struct o_native_lock * input_lock;
	struct o_native_lock * cond_lock;
	struct o_native_cond * cond;
	char * host;
	int port;
	int readed;
	int session_id;
	char response_status;
	struct o_connection_remote *conn;
};

void o_storage_factory_remote_free(struct o_storage_factory * to_free)
{
	struct o_storage_factory_remote * rem = (struct o_storage_factory_remote *) to_free;
	o_pool_free(rem->connection_pool);
}

enum o_url_type o_storage_factory_remote_get_type(struct o_storage_factory * storage_factory)
{
	return REMOTE;
}

struct o_storage * o_storage_factory_remote_storage_open(struct o_storage_factory *factory, char * storage_name, char * username, char * password)
{
	struct o_storage_factory_remote * remote = (struct o_storage_factory_remote *) factory;
	struct o_storage * new_storage = o_storage_remote_new(remote, storage_name, username, password);
	return new_storage;
}

struct o_storage_factory_class * o_storage_factory_remote_get_class()
{
	static struct o_storage_factory_class fact_class =
	{ .get_type = o_storage_factory_remote_get_type, .free = o_storage_factory_remote_free, .storage_open = o_storage_factory_remote_storage_open };
	return &fact_class;
}

void * o_storage_factory_pool_new_connection(void * factory)
{
	struct o_storage_factory_remote * fact = (struct o_storage_factory_remote *) factory;
	struct o_connection_remote * conn = o_connection_remote_new(fact->host, fact->port);
	o_connection_remote_add_to_selector(conn, fact->selector);
	return conn;
}

void o_storage_factory_pool_free_connection(void * factory, void * to_free)
{
	struct o_storage_factory_remote * fact = (struct o_storage_factory_remote *) factory;
	struct o_connection_remote * conn = (struct o_connection_remote *) to_free;
	o_connection_remote_remove_from_selector(conn, fact->selector);
	o_connection_remote_free(conn);
}

struct o_storage_factory * o_storage_factory_remote_new(char * path)
{
	struct o_storage_factory_remote * fact = o_malloc(sizeof(struct o_storage_factory_remote));
	fact->base_factory.clazz = o_storage_factory_remote_get_class();
	fact->connection_pool = o_pool_new(fact, o_storage_factory_pool_new_connection, o_storage_factory_pool_free_connection);
	fact->selector = o_native_socket_selector_new(READ);
	if (o_url_resolve_host_port_from_path(path, &fact->host, &fact->port))
		fact->port = 2424;
	return (struct o_storage_factory*) fact;
}

struct o_connection_remote * o_storage_factory_remote_begin_write(struct o_storage_factory_remote * factory)
{
	return (struct o_connection_remote *) o_pool_get(factory->connection_pool);
}

void o_storage_factory_remote_end_write(struct o_storage_factory_remote * factory, struct o_connection_remote * conn)
{
	o_pool_release(factory->connection_pool, conn);
}

struct o_connection_remote * o_storage_factory_remote_begin_read(struct o_storage_factory_remote * factory, int session_id, int *returnCode)
{
	do
	{
		int to_notify = 0;
		o_native_lock_lock(factory->input_lock);
		if (!factory->readed)
		{
			factory->conn = (struct o_connection_remote *) o_native_socket_selector_select(factory->selector, 0);
			factory->readed = 1;
			factory->response_status = o_connection_remote_read_byte(factory->conn);
			factory->session_id = o_connection_remote_read_int(factory->conn);
			to_notify = 1;
		}
		if (factory->session_id == session_id)
		{
			factory->readed = 0;
			*returnCode = factory->response_status;
			struct o_connection_remote * rem = factory->conn;
			o_native_lock_lock(factory->input_lock);
			return rem;
		}

		o_native_lock_unlock(factory->input_lock);
		o_native_lock_lock(factory->cond_lock);
		if (to_notify)
			o_native_cond_broadcast(factory->cond);
		o_native_cond_wait(factory->cond, factory->cond_lock);
		o_native_lock_unlock(factory->cond_lock);
	} while (1);
	/*never executed remove only warning.*/
	return 0;
}

void o_storage_factory_remote_end_read(struct o_storage_factory_remote * factory, struct o_connection_remote * conn)
{
	//TODO : verify directory use.
	o_native_socket_selector_end_select(factory->selector, (struct o_native_socket *) conn);
	o_native_lock_lock(factory->cond_lock);
	o_native_cond_broadcast(factory->cond);
	o_native_lock_unlock(factory->cond_lock);
}

