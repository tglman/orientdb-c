#ifndef O_REMOTE_PROTOCOL_SPECS_H_
#define O_REMOTE_PROTOCOL_SPECS_H_

// COMMANDS
#define SHUTDOWN 1
#define CONNECT 2

#define DB_OPEN	 3
#define DB_CREATE 4
#define DB_CLOSE 5
#define DB_EXIST 6
#define DB_DELETE 7
#define DB_SIZE 8
#define DB_COUNTRECORDS 9

#define CLUSTER_ADD 10
#define CLUSTER_REMOVE 11
#define CLUSTER_COUNT 12

#define DATASEGMENT_ADD 20
#define DATASEGMENT_REMOVE 21

#define RECORD_LOAD 30
#define RECORD_CREATE 31
#define RECORD_UPDATE 32
#define RECORD_DELETE 33

#define TX_COMMIT 60

#define COMMIT_UPDATE 1
#define COMMIT_DELETED 2
#define COMMIT_CREATE 3

#define COUNT 40
#define COMMAND 41

#define DICTIONARY_LOOKUP 50
#define DICTIONARY_PUT 51
#define DICTIONARY_REMOVE 52
#define DICTIONARY_SIZE 53
#define DICTIONARY_KEYS 54

// STATUSES
#define OK	 0
#define ERROR 1

// CONSTANTS
#define RECORD_NULL -2


#endif /* O_REMOTE_PROTOCOL_SPECS_H_ */
