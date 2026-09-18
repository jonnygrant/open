#ifndef FMT_H
#define FMT_H

/**
 * Copyright 2026 Jonathan Grant <jgrantonline AT gmail com>
 *
 * Distributed under the LICENSE.txt included in the release.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * https://www.boost.org/LICENSE_1_0.txt
*/

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* Implemented as macros
 *
fmt_result_t fmt_print(const char *restrict format, ...);
fmt_result_t fmt_fprint(FILE * restrict stream, const char *restrict format, ...);
fmt_string_t fmt_format(const char *restrict format, ...);
fmt_result_t fmt_format_append(fmt_string_t * buf, const char * restrict format, ...);
*/

#define FMT_CAPACITY (0x100)

#define FMT_INIT (fmt_string_t){ \
    .heap_ptr = NULL, \
    .length = 0, \
    .capacity = FMT_CAPACITY, \
    .result = FMT_OK, \
    .internal_buf[0] = '\0' \
}


typedef enum
{
    FMT_OK = 0,
    FMT_ERROR_FORMAT = 1,
    FMT_ERROR_MEMORY = 2,
    FMT_ERROR_IO = 3,
    FMT_ERROR_MANY_ARGS = 4,
    FMT_ERROR_FEW_ARGS = 5,
    FMT_ERROR_NULL = 6,
    FMT_ERROR_STATE = 7,
    FMT_ERROR_UNKNOWN = 8
} fmt_result_t;


typedef enum
{
    FMT_INT = 0,
    FMT_UINT = 1,
    FMT_DOUBLE = 2,
    FMT_STRING = 3,
    FMT_BOOL = 4,
    FMT_PTR = 5,
    FMT_CHAR = 6,
    FMT_LONG_DOUBLE = 7,
    FMT_STRING_T = 8
} fmt_type_t;


// capacity - is set to FMT_CAPACITY if internal_buf is in use
// heap_ptr is NULL when not in use
typedef struct
{
    char * heap_ptr;    // NULL if using internal_buf
    size_t length;      // Current string length (excluding NUL terminator)
    size_t capacity;    // Current buffer size (internal, or heap)
    fmt_result_t result;
    char internal_buf[FMT_CAPACITY];
} fmt_string_t;


/*
 * There may be a build note. fmt.h:127:1: note: the ABI of passing union with ‘long double’ has changed in GCC 4.4
 */
typedef struct
{
    fmt_type_t type;

    union {
        int64_t i;
        uint64_t u;
        double d;
        const char * str;
        bool b;
        const void * ptr;
        char c;
        long double long_d;
        fmt_string_t * fmt_ptr_string;
    } data;
} fmt_tag_t;


#define FMT_COUNT(...) \
    FMT_NARG(__VA_ARGS__)

fmt_result_t fmt_init(fmt_string_t * buf);
fmt_string_t fmt_copy(const fmt_string_t * src);
fmt_result_t fmt_fprint_string(FILE * restrict stream, const fmt_string_t * buf);
fmt_result_t fmt_concat(fmt_string_t * dst, const fmt_string_t * src);

/* Access and inspect */
const char * fmt_string_data(const fmt_string_t * buf);

/* The capacity in bytes of the buffer */
size_t fmt_capacity(const fmt_string_t * buf);

/* The length of text in bytes excluding null terminator */
size_t fmt_length(const fmt_string_t * buf);

/* Access a string representing of the result status code */
const char * fmt_result(const fmt_string_t * buf);

fmt_result_t fmt_result_code(const fmt_string_t * buf);

/* Access a string representing of the result status code */
const char * fmt_result_string(const fmt_result_t result);

fmt_string_t fmt_format_impl(const char * restrict format, const fmt_tag_t * args, size_t tag_count);
fmt_result_t fmt_format_append_impl(fmt_string_t * dst, const char * restrict format, const fmt_tag_t * args, size_t tag_count);
fmt_result_t fmt_print_impl(FILE * restrict stream, const char * restrict str, const fmt_tag_t * args, size_t tag_count);

fmt_result_t fmt_free(fmt_string_t * buf);
fmt_result_t fmt_clear(fmt_string_t * buf);

#define fmt_print(fmt, ...) \
    fmt_print_impl( \
        (stdout), \
        (fmt), \
        (fmt_tag_t[]){ FMT_ARGS(__VA_ARGS__) }, \
        FMT_COUNT(__VA_ARGS__) \
    )

#define fmt_fprint(stream, fmt, ...) \
    fmt_print_impl( \
        (stream), \
        (fmt), \
        (fmt_tag_t[]){ FMT_ARGS(__VA_ARGS__) }, \
        FMT_COUNT(__VA_ARGS__) \
    )

static inline fmt_tag_t fmt_tag_int(int64_t v)     { return (fmt_tag_t){ .type = FMT_INT,     .data.i = v }; }
static inline fmt_tag_t fmt_tag_int_ptr(int64_t * v)
{
    if(NULL != v)
    {
        return (fmt_tag_t){ .type = FMT_INT,     .data.i = *v };
    }
    else
    {
        return (fmt_tag_t){ .type = FMT_STRING, .data.str = "*int NULL" };
    }
}

static inline fmt_tag_t fmt_tag_uint(uint64_t v)   { return (fmt_tag_t){ .type = FMT_UINT,    .data.u = v }; }
static inline fmt_tag_t fmt_tag_double(double v)   { return (fmt_tag_t){ .type = FMT_DOUBLE,  .data.d = v }; }
static inline fmt_tag_t fmt_tag_string(const char * str){ return (fmt_tag_t){ .type = FMT_STRING, .data.str = str ? str : "(null)" }; }
static inline fmt_tag_t fmt_tag_bool(bool v)       { return (fmt_tag_t){ .type = FMT_BOOL,    .data.b = v }; }
static inline fmt_tag_t fmt_tag_ptr(const void * ptr) { return (fmt_tag_t){ .type = FMT_PTR, .data.ptr = ptr }; }
static inline fmt_tag_t fmt_tag_char(char v)             { return (fmt_tag_t){ .type = FMT_CHAR,        .data.c = v }; }
static inline fmt_tag_t fmt_tag_long_double(long double v) { return (fmt_tag_t){ .type = FMT_LONG_DOUBLE, .data.long_d = v }; }
static inline fmt_tag_t fmt_tag_fmt_string_ptr(fmt_string_t * v) { return (fmt_tag_t){ .type = FMT_STRING_T, .data.fmt_ptr_string = v }; }
static inline fmt_tag_t fmt_tag_fmt_null_ptr(nullptr_t v) { return (fmt_tag_t){ .type = FMT_STRING, .data.str = "nullptr_t" }; }
static inline fmt_tag_t fmt_tag_fmt_tag(fmt_tag_t v) { return v; }
static inline fmt_tag_t fmt_arg_UNKNOWN_ERROR(void * __attribute__((unused)) ptr) { return (fmt_tag_t){ .type = FMT_STRING, .data.str = "Unknown_Tag" }; }

// Map types to formatter
#define FMT_ARG(x) _Generic((x), \
    bool:               fmt_tag_bool, \
    char:               fmt_tag_char, \
    signed char:        fmt_tag_int, \
    short:              fmt_tag_int, \
    int:                fmt_tag_int, \
    int*:               fmt_tag_int_ptr, \
    long:               fmt_tag_int, \
    long long:          fmt_tag_int, \
    unsigned char:      fmt_tag_uint, \
    unsigned short:     fmt_tag_uint, \
    unsigned int:       fmt_tag_uint, \
    unsigned long:      fmt_tag_uint, \
    unsigned long long: fmt_tag_uint, \
    float:              fmt_tag_double, \
    double:             fmt_tag_double, \
    char*:              fmt_tag_string, \
    const char*:        fmt_tag_string, \
    long double:        fmt_tag_long_double, \
    fmt_string_t*:      fmt_tag_fmt_string_ptr, \
    fmt_tag_t:          fmt_tag_fmt_tag, \
    void*:              fmt_tag_ptr, \
    nullptr_t:          fmt_tag_fmt_null_ptr \
)(x)

/* A default with const void * */

/* Provide a simpler macro name */
#define fmt(x) FMT_ARG (x)

#define FMT_ARG_1(a) \
    FMT_ARG(a)

#define FMT_ARG_2(a, ...) \
    FMT_ARG(a), FMT_ARG_1(__VA_ARGS__)

#define FMT_ARG_3(a, ...) \
    FMT_ARG(a), FMT_ARG_2(__VA_ARGS__)

#define FMT_ARG_4(a, ...) \
    FMT_ARG(a), FMT_ARG_3(__VA_ARGS__)

#define FMT_ARG_5(a, ...) \
    FMT_ARG(a), FMT_ARG_4(__VA_ARGS__)

#define FMT_ARG_6(a, ...) \
    FMT_ARG(a), FMT_ARG_5(__VA_ARGS__)

#define FMT_ARG_7(a, ...) \
    FMT_ARG(a), FMT_ARG_6(__VA_ARGS__)

#define FMT_ARG_8(a, ...) \
    FMT_ARG(a), FMT_ARG_7(__VA_ARGS__)

#define FMT_NARG(...) \
    FMT_NARG_I(__VA_ARGS__,8,7,6,5,4,3,2,1)

#define FMT_NARG_I(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N

#define FMT_ARGS(...) \
    FMT_ARGS_I(FMT_NARG(__VA_ARGS__), __VA_ARGS__)

#define FMT_ARGS_I(n, ...) \
    FMT_ARGS_II(n, __VA_ARGS__)

#define FMT_ARGS_II(n, ...) \
    FMT_ARG_##n(__VA_ARGS__)

#define fmt_format(fmt, ...) \
    fmt_format_impl( \
        (fmt), \
        (fmt_tag_t[]){ FMT_ARGS(__VA_ARGS__) }, \
        FMT_NARG(__VA_ARGS__) \
    )

#define fmt_format_append(dst, fmt, ...) \
    fmt_format_append_impl( \
        (dst), \
        (fmt), \
        (fmt_tag_t[]){ FMT_ARGS(__VA_ARGS__) }, \
        FMT_NARG(__VA_ARGS__) \
    )

#endif
