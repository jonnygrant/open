/**
 * Copyright 2026 Jonathan Grant <jgrantonline AT gmail com>
 *
 * Distributed under the LICENSE.txt included in the release.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * https://www.boost.org/LICENSE_1_0.txt
*/


#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "fmt.h"

// TODO add validation that pointers are non-NULL

//#define DEBUG(x) x
#define DEBUG(x)

fmt_result_t fmt_init(fmt_string_t * buf)
{
    fmt_result_t result = FMT_OK;

    if(NULL == buf)
    {
        result = FMT_ERROR_NULL;
    }
    else
    {
        buf->heap_ptr = NULL;
        buf->length = 0;
        buf->capacity = FMT_CAPACITY;
        buf->result = FMT_OK;
        buf->internal_buf[0] = '\0'; // Only intialize the first byte, not the agregate {0}
    }

    return result;
}


fmt_string_t fmt_copy(const fmt_string_t * const src)
{
    fmt_string_t copy = FMT_INIT;

    if(NULL == src)
    {
        copy.result = FMT_ERROR_NULL;
        return copy;
    }

    if(src->heap_ptr)
    {
        copy.heap_ptr = malloc(src->capacity);
        if(NULL == copy.heap_ptr)
        {
            copy.result = FMT_ERROR_MEMORY;
        }
        else
        {
            memcpy(copy.heap_ptr, src->heap_ptr, src->length);
        }
    }
    else
    {
        memcpy(copy.internal_buf, src->internal_buf, src->length);
    }

    if(FMT_OK == copy.result)
    {
        copy.capacity = src->capacity;
        copy.length = src->length;
    }

    return copy;
}


const char * fmt_result(const fmt_string_t * const buf)
{
    if(NULL == buf)
    {
        return fmt_result2(FMT_ERROR_NULL);
    }
    else
    {
        return fmt_result2(buf->result);
    }
}


fmt_result_t fmt_result_code(const fmt_string_t * const buf)
{
    if(NULL == buf)
    {
        return FMT_ERROR_NULL;
    }
    else
    {
        return buf->result;
    }
}


const char * fmt_result2(const fmt_result_t err)
{
    switch (err)
    {
        case FMT_OK:                    return "FMT_OK";
        case FMT_ERROR_FORMAT:          return "FMT_ERROR_FORMAT";
        case FMT_ERROR_MEMORY:          return "FMT_ERROR_MEMORY";
        case FMT_ERROR_IO:              return "FMT_ERROR_IO";
        case FMT_ERROR_MANY_ARGS:   return "FMT_ERROR_MANY_ARGS";
        case FMT_ERROR_FEW_ARGS:    return "FMT_ERROR_FEW_ARGS";
        case FMT_ERROR_NULL:        return "FMT_ERROR_NULL";
        case FMT_ERROR_UNKNOWN:     return "FMT_ERROR_UNKNOWN";
        default: return "FMT_ERROR_VALUE_UNKNOWN";
    }
}


static void fmt_append_char(fmt_string_t * buf, const char c)
{
    if (buf->length + 1 >= buf->capacity)
    {
        size_t new_capacity = buf->capacity * 2;

        char * new_alloc_buf;

        if (buf->heap_ptr)
        {
            char * realloc_buf = realloc(buf->heap_ptr, new_capacity);
            if(realloc_buf)
            {
                new_alloc_buf = realloc_buf;
            }
            else
            {
                new_alloc_buf = NULL;
                buf->result = FMT_ERROR_MEMORY;
            }
        }
        else
        {
            new_alloc_buf = malloc(new_capacity);

            if (new_alloc_buf)
            {
                memcpy(new_alloc_buf, buf->internal_buf, buf->length);
            }
        }

        if (!new_alloc_buf)
        {
            buf->result = FMT_ERROR_MEMORY;

            return;
        }

        buf->heap_ptr = new_alloc_buf;
        buf->capacity = new_capacity;
    }

    char * data = buf->heap_ptr ? buf->heap_ptr : buf->internal_buf;

    data[buf->length++] = c;
    data[buf->length] = '\0';
}


static void fmt_append_string(fmt_string_t * buf, const char * s)
{
    if(NULL == buf)
    {
        // TODO: This is an error. Perhaps compile_assert(s != NULL) is suitable
        return;
    }

    if(NULL == s)
    {
        buf->result = FMT_ERROR_NULL;
        return;
    }

    while (*s)
    {
        fmt_append_char(buf, *s++);

        if(buf->result != FMT_OK)
        {
            return;
        }
    }
}


static void fmt_append_int(fmt_string_t * buf, const int64_t v)
{
    char tmp[64];

    snprintf(tmp, sizeof(tmp), "%lld", (long long)v);
    fmt_append_string(buf, tmp);
}


static void fmt_append_uint(fmt_string_t * buf, const uint64_t v)
{
    char tmp[64];

    snprintf(tmp, sizeof(tmp), "%llu",
             (unsigned long long)v);

    fmt_append_string(buf, tmp);
}


static void fmt_append_double(fmt_string_t * buf, const double v)
{
    char tmp[128];

    snprintf(tmp, sizeof(tmp), "%g", v);
    fmt_append_string(buf, tmp);
}


static void fmt_append_bool(fmt_string_t * buf, const bool v)
{
    fmt_append_string(buf, v ? "true" : "false");
}


static void fmt_append_ptr(fmt_string_t * buf, const void * const ptr)
{
    char tmp[64];

    snprintf(tmp, sizeof(tmp), "%p", ptr);
    fmt_append_string(buf, tmp);
}


static void fmt_append_tag(fmt_string_t * buf, const fmt_tag_t * const tag)
{
    if(NULL == buf)
    {
        // TODO consider error handling
        return;
    }

    if(NULL == tag)
    {
        // TODO consider error handling
        return;
    }

    switch (tag->type)
    {
        case FMT_INT:
        {
            fmt_append_int(buf, tag->data.i);
            break;
        }
        case FMT_UINT:
        {
            fmt_append_uint(buf, tag->data.u);
            break;
        }
        case FMT_DOUBLE:
        {
            fmt_append_double(buf, tag->data.d);
            break;
        }
        case FMT_STRING:
        {
            fmt_append_string(buf, tag->data.str);
            break;
        }
        case FMT_BOOL:
        {
            fmt_append_bool(buf, tag->data.b);
            break;
        }
        case FMT_PTR:
        {
            fmt_append_ptr(buf, tag->data.ptr);
            break;
        }
        default:
        {
            buf->result = FMT_ERROR_UNKNOWN;

            // TODO consider using compile_assert() to issue warning diagnostic
            break;
        }
    }
}

// TODO add tag_count and args checks
fmt_string_t fmt_format_impl(const char * format, const fmt_tag_t * const args, const size_t tag_count)
{
    fmt_string_t buf = FMT_INIT;
    size_t arg = 0;

    if(NULL == format)
    {
        buf.result = FMT_ERROR_NULL;
        return buf;
    }

    while (*format)
    {
        // TODO currently handles only {} placement
        if (*format == '{' && format[1] == '}')
        {
            if (arg < tag_count)
            {
                fmt_append_tag(&buf, &args[arg++]);

                if(buf.result != FMT_OK)
                {
                    return buf;
                }
            }

            format += 2;
            continue;
        }

        fmt_append_char(&buf, *format++);
    }

    return buf;
}


fmt_result_t fmt_print_impl(FILE * restrict stream, const char * const str, const fmt_tag_t * const args, const size_t tag_count)
{
    fmt_string_t s = fmt_format_impl(str, args, tag_count);

    if(s.result != FMT_OK)
    {
         return s.result;
    }

    const int fputs_result = fputs(fmt_string_data(&s), stream);

    if(EOF == fputs_result)
    {
        DEBUG(printf("fputs returned %d, errno %d\n", result, errno);)

        s.result = FMT_ERROR_IO;
    }

    fmt_result_t free_result = fmt_free(&s);

    /* If no existing, error, copy fmt_free() error if one occured */
    if(FMT_OK == s.result)
    {
        if(FMT_OK != free_result)
        {
            s.result = free_result;
        }
    }

    DEBUG(printf("--fmt_print_impl %d\n", s.result);)

    return s.result;
}


/* Access the formatted string */
const char * fmt_string_data(const fmt_string_t * const buf)
{
    const char * ptr;

    if(NULL == buf)
    {
        ptr = "";
    }
    else
    {
        ptr = buf->heap_ptr ? buf->heap_ptr : buf->internal_buf;
    }

    return ptr;
}


size_t fmt_capacity(const fmt_string_t * const buf)
{
    size_t capacity = 0;

    if(NULL != buf)
    {
        capacity = buf->capacity;
    }

    return capacity;
}


size_t fmt_length(const fmt_string_t * const buf)
{
    size_t length = 0;

    if(NULL != buf)
    {
        length = buf->length;
    }

    return length;
}


fmt_result_t fmt_free(fmt_string_t * const buf)
{
    if(NULL == buf)
    {
        return FMT_ERROR_NULL;
    }

    if (buf->heap_ptr != NULL)
    {
        free(buf->heap_ptr);
        buf->heap_ptr = NULL;
    }

    buf->length = 0;
    buf->capacity = FMT_CAPACITY;

    buf->internal_buf[0] = '\0';

    return FMT_OK;
}


fmt_result_t fmt_clear(fmt_string_t * const buf)
{
    if(NULL == buf)
    {
        return FMT_ERROR_NULL;
    }

    char * str = buf->heap_ptr ? buf->heap_ptr : buf->internal_buf;
    str[0] = '\0';

    buf->length = 0;

    return FMT_OK;
}
