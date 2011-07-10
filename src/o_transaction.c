#include "o_transaction.h"
#include "o_map.h"

enum record_operation_type{
	SAVE,REMOVE,LOADED
};

struct o_transaction_entry
{
	enum record_operation_type type;
	struct o_record * record;
};

struct o_transaction
{
	struct o_map * records;
};
