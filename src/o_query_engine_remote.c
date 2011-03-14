#include "o_query_engine_remote.h"
#include "o_query_engine_internal.h"
#include "o_query_internal.h"
#include "o_storage_remote_internal.h"
#include "o_remote_protocol_specs.h"
#include "o_connection_remote.h"
#include "o_memory.h"
#include "o_output_stream_byte.h"
#include "o_raw_buffer_byte.h"
#include <string.h>

struct o_query_engine_remote
{
	struct o_query_engine engine;
	struct o_storage_remote * storage;
};

void o_query_engine_remote_query_parameter(struct o_query_engine * engine, struct o_query * query, struct o_document_value ** parameters, void * add_info,
		query_result_callback callback)
{
	int req_id = o_storage_remote_new_request_id();
	struct o_query_engine_remote *engine_remote = (struct o_query_engine_remote *) engine;
	o_connection_remote_begin_write_session(engine_remote->storage->connection, req_id, COMMAND);
	o_connection_remote_write_byte(engine_remote->storage->connection, 's');
	struct o_output_stream *str = o_output_stream_byte_buffer();
	o_query_seriealize(query, str);
	int cont_size = 0;
	unsigned char *content = o_output_stream_byte_content(str, &cont_size);
	o_connection_remote_write_bytes(engine_remote->storage->connection, content, cont_size);
	o_connection_remote_end_write(engine_remote->storage->connection);
	o_output_stream_free(str);

	o_storage_remote_begin_response(engine_remote->storage, req_id);
	int status;
	while ((status = o_connection_remote_read_byte(engine_remote->storage->connection)) > 0)
	{
		int size;
		//CLASS!! RIQUERED FOR THE PROTOCOLL I IGNORE
		o_connection_remote_read_int(engine_remote->storage->connection);

		char type = o_connection_remote_read_byte(engine_remote->storage->connection);
		short rec_cl = o_connection_remote_read_short(engine_remote->storage->connection);
		int rec_pos = o_connection_remote_read_int(engine_remote->storage->connection);
		int version = o_connection_remote_read_int(engine_remote->storage->connection);
		unsigned char * o_p = o_connection_remote_read_bytes(engine_remote->storage->connection, &size);
		struct o_raw_buffer * buff = o_raw_buffer_byte(type, version, o_p, size);
		if (status == 1)
			callback(add_info, o_record_id_new(rec_cl, rec_pos), buff);

	}

}

void o_query_engine_remote_free(struct o_query_engine * engine)
{
	o_free(engine);
}

struct o_query_engine * o_query_engine_remote_new(struct o_storage_remote * remote)
{
	struct o_query_engine_remote * remeng = o_malloc(sizeof(struct o_query_engine_remote));
	memset(remeng, 0, sizeof(struct o_query_engine_remote));
	remeng->storage = remote;
	remeng->engine.o_query_engine_query_parameter = o_query_engine_remote_query_parameter;
	remeng->engine.o_query_engine_free = o_query_engine_remote_free;
	return &remeng->engine;
}

