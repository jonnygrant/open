#ifndef RUNTIME_ASSERT_H
#define RUNTIME_ASSERT_H

/* Standarize as follows
runtime_assert(expr, message)
runtime_assert(expr, message)
runtime_assert(expr, message, callback)
*/

#include <stdio.h>
#include <stdlib.h>

typedef void (*runtime_assert_callback)(const char * file, int line, const char * expr,
                                        const char * msg);

void set_assert_runtime_handler(runtime_assert_callback callback);

void assert_runtime_handler(const char * file, int line, const char * expr, const char * message);

extern runtime_assert_callback __assert_handler;

/* Default runtime_assert error */
void __runtime_assert_fail(const char * file, int line, const char * expr,
                                       const char * msg, runtime_assert_callback callback);

#define runtime_assert(expr, msg, callback) \
    do \
    { \
        if (!(expr)) \
        { \
            __runtime_assert_fail(__FILE__, __LINE__, \
                                  #expr, msg, callback); \
        } \
    } while (0)

#endif // RUNTIME_ASSERT_H
