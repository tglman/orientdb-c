#ifndef O_CONNECTION_REMOTE_H_
#define O_CONNECTION_REMOTE_H_
#include "o_connection_internal.h"
#include "o_database_socket.h"
struct o_connection_remote;

/*! \brief Retrieve a new remote connection from an listen socket.
 *
 * \param listen_socket the listen socket.
 * \return the new connection.
 */
struct o_connection * o_connection_remote_new_accept(struct o_database_socket * listen_sock);

/**
 * Read an int from connection.
 *
 * @param connection where read.
 * @return the integer readed.
 */
int o_connection_remote_read_int(struct o_connection_remote * connection);

/**
 * Read an 64 bit long from connection.
 *
 * @param connection where read.
 * @return the long readed.
 */
long long o_connection_remote_read_long64(struct o_connection_remote * connection);

/**
 * Read a byte from the connection.
 *
 * @param connection where read.
 * @return the byte read.
 */
char o_connection_remote_read_byte(struct o_connection_remote * connection);

/**
 * Read a short from the connection.
 *
 * @param connection where read.
 * @return the short read.
 */
short o_connection_remote_read_short(struct o_connection_remote * connection);

/**
 * Read an array of byte/char from the connection.
 *
 * @param connection where read.
 * @param byte_read the nunmber of byte read.
 * @return a new array of byte/char filled.
 */
char * o_connection_remote_read_bytes(struct o_connection_remote * connection, int *byte_read);

/*! \brief Read a null terminated string from connection.
 *
 * \param connection wher read.
 * \return a null terminated string.
 */
char * o_connection_remote_read_string(struct o_connection_remote * connection);

/**
 * Read an array of strings from connections.
 *
 * @param connection where read the array.
 * @param array_size filled with the size of readed size.
 * @return the array of null terminated string.
 */
char ** o_connection_remote_read_array_strings(struct o_connection_remote * connection, int *array_size);

/** Write an int to connection.
 *
 * @param connection where write.
 * @param int_value to write.
 */
void o_connection_remote_write_int(struct o_connection_remote * connection, int int_value);

/** Write an long 64 to connection.
 *
 * @param connection where write.
 * @param long_value to write.
 */
void o_connection_remote_write_long64(struct o_connection_remote * connection, long long long_value);

/** Write an byte to connection.
 *
 * @param connection where write.
 * @param byte_value to write.
 */
void o_connection_remote_write_byte(struct o_connection_remote * connection, char byte_value);

/** Write an short to connection.
 *
 * @param connection where write.
 * @param short_value to write.
 */
void o_connection_remote_write_short(struct o_connection_remote * connection, short short_value);

/** Write an byte [] to connection.
 *
 * @param connection where write.
 * @param byte_array to write.
 * @param lenght the length of byte to string
 */
void o_connection_remote_write_bytes(struct o_connection_remote * connection,unsigned char *byte_array, int length);

/*! \brief Write a  null terminated string to connection.
 *
 * \param connection where write.
 * \param string to write.
 */
void o_connection_remote_write_string(struct o_connection_remote * connection, char *string);

/** Write an array of string null terminated to connection.
 *
 * @param connection where write.
 * @param strings_array to write.
 * @param the length of array to write.
 */
void o_connection_remote_write_array_strings(struct o_connection_remote * connection, char * *strings_array, int length);

/**
 * Flush buffered data on connection.
 *
 * @param connection to flush.
 */
void o_connection_remote_flush(struct o_connection_remote * connection);

#endif /* O_CONNECTION_REMOTE_H_ */
