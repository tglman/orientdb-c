#ifndef O_ROW_BUFFER_INTERNAL_H_
#define O_ROW_BUFFER_INTERNAL_H_

struct o_raw_buffer
{
	int (*o_raw_buffer_content_size)(struct o_raw_buffer * buff);
	char * (*o_raw_buffer_content)(struct o_raw_buffer * buff, int * content_size);
	int (*o_raw_buffer_version)(struct o_raw_buffer * buff);
	char (*o_raw_buffer_type)(struct o_raw_buffer * buff);
};

#endif /* O_ROW_BUFFER_INTERNAL_H_ */
