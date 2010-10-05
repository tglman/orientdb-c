#ifndef O_URL_RESOLVER_H_
#define O_URL_RESOLVER_H_

#define HTTP_PROTOCOL_PARAMETER "HTTP://"
#define HTTP_PROTOCOL_PARAMETER_LENGHT 7

#define REMOTE_PROTOCOL_PARAMETER "REMOTE:"
#define REMOTE_PROTOCOL_PARAMETER_LENGHT 7

#define LOCAL_PROTOCOL_PARAMETER "LOCAL:"
#define LOCAL_PROTOCOL_PARAMETER_LENGHT 6

enum o_url_type
{
	HTTP = 1, REMOTE = 2, LOCAL = 3
};

/** Resolve the connection information from url.
 *
 * @param connection_url
 * @param type filled with the type of connection.
 * @param path filled with the path of connection.
 * @param db_name filled with the name of database.
 * @return 0 if error occur otherwise != 0
 *
 */
int o_url_resolve_information(char * connection_url, enum o_url_type * type, char **path, char ** db_name);

/**
 * Parse the host and port from a path.
 *
 * @param path start where parse.
 * @param host filled with the host name.
 * @param port filled with the port.
 * @return  1 if the path contains the port number otherwise 0 if the path not contains the port.
 */
int o_url_resolve_host_port_from_path(char * path, char ** host, int * port);

#endif //O_URL_RESOLVER_H_
