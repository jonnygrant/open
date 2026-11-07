#include <stdlib.h>
#include <stdio.h>

#include "runtime_assert.h"

void __default_assert_runtime_handler(const char * file, int line, const char * expr, const char * message)
{
    fprintf(stderr, "default runtime_assert: %s:%d (%s) message: '%s'\n", file, line, expr, message);

    abort();
}

runtime_assert_callback __assert_handler = __default_assert_runtime_handler;

void set_assert_runtime_handler(runtime_assert_callback handler)
{
    if(handler)
    {
        __assert_handler = handler;
    }
    else
    {
        __assert_handler = __default_assert_runtime_handler;
    }
}

/* Default runtime_assert error */
void __runtime_assert_fail(const char * file, int line, const char * expr,
                                       const char * msg, runtime_assert_callback callback)
{
    if (callback != nullptr)
    {
        callback(file, line, expr, msg);
    }
    else
    {
        /* Call default handler */
        __assert_handler(file, line, expr, msg);
    }
}
