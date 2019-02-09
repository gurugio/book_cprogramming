#include <stdio.h>

struct cstring {
	char *_buf;
	size_t _len;
	size_t _max_size;
	/* http://www.cplusplus.com/reference/string/string/ */
	void (*clear)(struct cstring *this);
	size_t (*length)(struct cstring *this);
	size_t (*max_size)(struct cstring *this);
	char (*at)(struct cstring *this, size_t pos);
	size_t (*compare)(struct cstring *this, struct cstring *cstr);
	void (*print)(struct cstring *this);
	void (*debug)(struct cstring *this);
};

typedef struct cstring cstring;


#define DEFINE_CSTRING(__name, __buf)					\
	struct cstring __name = {						\
		._buf = strdup(__buf),						\
		._len = strlen(__buf),						\
		._max_size = strlen(__buf),					\
		.clear = cstring_clear,						\
		.length = cstring_length,					\
		.max_size = cstring_max_size,				\
		.at = cstring_at,							\
		.print = cstring_print,						\
		.debug = cstring_debug};

cstring *create_cstring(char *str, size_t max_size);
void destroy_cstring(cstring *cstr);
