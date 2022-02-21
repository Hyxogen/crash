#include <assert.h>

#define FOOS \
	X(op_plus, "+") Y \
	X(op_minus, "-") Y \
	X(op_multiply, "*") Y \
	X(op_divide, "/")

enum foo {
#define X(name, symbol) name
#define Y ,
FOOS
#undef X
#undef Y
};

const char *get_foo_name(enum foo foo)
{
	switch (foo)
	{
#define X(name, symbol) \
	case name: \
		return #name;
#define Y
FOOS
#undef X
#undef Y
	}
}

enum foo name_to_foo(const char *str)
{
#define X(name, symbol) \
	if (strcmp(str, #name) == 0) \
		return name;
#define Y
FOOS
#undef X
#undef Y
	assert(0);
}

enum foo symbol_to_operator(const char *str)
{
#define X(name, symbol) \
	if (strcmp(str, #symbol) == 0) \
		return name;
#define Y
FOOS
#undef X
#undef Y
	assert(0);
}