#include "o_query_internal.h"
#include "o_string_buffer.h"
#include "o_output_stream_byte.h"
#include "o_output_stream_data.h"
#include "o_memory.h"
#include "o_record.h"
#include <string.h>

#define CMD_CREATE "CREATE"
#define CMD_UPDATE "UPDATE"

struct o_query
{
	void (*o_query_free)(struct o_query *query);
	void (*o_query_seriealize)(struct o_query * query, struct o_output_stream * stream);
	int limit;
	struct o_record_id *beginRange;
	struct o_record_id *endRange;
	char * fetchPlan;
};

struct o_query_sql
{
	struct o_query query;
	char * sql;
};

void o_query_sql_free(struct o_query *query)
{
	struct o_query_sql * qsql = (struct o_query_sql *) query;
	o_free(qsql->sql);
	o_free(qsql);
}

void o_query_sql_serialize(struct o_query * query, struct o_output_stream * stream)
{
	struct o_query_sql * qsql = (struct o_query_sql *) query;
	struct o_output_stream_data *data = o_output_stream_data_new(stream);
	
	// support create, update
	char query_begin[6];
	int i;
	
	strncpy(query_begin, qsql->sql, 6);
	
	for (i = 0; i < 6; i++)
		query_begin[i] = toupper(query_begin[i]);
	
	int idempotent = ( strncmp(query_begin, CMD_CREATE, 6) || strncmp(query_begin, CMD_UPDATE, 6));
	
	if (idempotent == 1) {
	  
	  o_output_stream_data_write_string(data, "com.orientechnologies.orient.core.sql.OCommandSQL");
	  
	} else {
	
	  o_output_stream_data_write_string(data, "com.orientechnologies.orient.core.sql.query.OSQLSynchQuery");
	
	}
	
	o_output_stream_data_write_string(data, "com.orientechnologies.orient.core.sql.query.OSQLSynchQuery");
	o_output_stream_data_write_string(data, qsql->sql);
	o_output_stream_data_write_int(data, qsql->query.limit);
	o_output_stream_data_write_string(data, qsql->query.fetchPlan != 0 ? qsql->query.fetchPlan : "");
	//Write an empty byte array of params.
	o_output_stream_data_write_string(data, "");
	//Write next record.
	o_output_stream_data_write_string(data, "");
	//Write next record params.
	o_output_stream_data_write_string(data, "");
	o_output_stream_data_free(data);
}

struct o_query * o_query_sql(char * query)
{
	struct o_query_sql * qsql = o_malloc(sizeof(struct o_query_sql));
	memset(qsql, 0, sizeof(struct o_query_sql));
	qsql->sql = o_memdup(query, strlen(query) + 1);
	qsql->query.o_query_free = o_query_sql_free;
	qsql->query.o_query_seriealize = o_query_sql_serialize;
	qsql->query.limit = -1;
	return &qsql->query;
}

void o_query_seriealize(struct o_query * query, struct o_output_stream * stream)
{
	query->o_query_seriealize(query, stream);
}

void o_query_free(struct o_query * query)
{
	query->o_query_free(query);
}
