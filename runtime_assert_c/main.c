#include "runtime_assert.h"

void log_handler(const char * file, int line, const char * expr, const char * message)
{
    fprintf(stderr, "log_handler: %s:%d (%s) message: '%s'\n", file, line, expr, message);
}

void custom_assert_handler(const char * file, int line, const char * expr, const char * message)
{
    fprintf(stderr, "custom runtime_assert: %s:%d (%s) message: '%s'\n", file, line, expr, message);
}

int main()
{
    set_assert_runtime_handler(log_handler);

    __builtin_printf("log_handler %p\n", log_handler);

    runtime_assert(1 == 2, "", NULL);

    set_assert_runtime_handler(custom_assert_handler);

    runtime_assert(2 == 3, "", NULL);

    {
        const char * buf = NULL;
        runtime_assert(buf != NULL, "error NULL", NULL);
    }

    return 0;
}
