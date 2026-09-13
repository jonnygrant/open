#include <math.h>

#include "fmt.h"

typedef struct my_struct
{
    int a;
    float f;
} my_struct_t;


static const char * my_format(my_struct_t my)
{
    static char buf[50];

    snprintf(buf, sizeof(buf), "int %d, float %6.2f", my.a, my.f);

    return buf;
}

int main()
{
    fmt_result_t result;

    fmt_string_t buf;
    buf = fmt_format("Hello {} newline\n", "planet");
    printf("%s", fmt_string_data(&buf));

    fmt_print("Again {}", fmt_string_data(&buf));

    fmt_print("pi = {}\n", 3.14159);

    // free any heap that was allocated
    fmt_free(&buf);

    fmt_string_t my_buf = FMT_INIT;
    my_buf = fmt_format("Hello {}", "London");
    printf("%s\n", fmt_string_data(&my_buf));

    fmt_string_t buf_copy = fmt_copy(&my_buf);
    printf("%s\n", fmt_string_data(&buf_copy));

    result = fmt_fprint_string(stdout, &buf_copy);
    if(FMT_OK != result) printf("fmt_fprint_string err: %s\n", fmt_result_string(result));
    fmt_print("\n", 1);

    result = fmt_print("Hello {}\n", "world");
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("int = {}\n", 42);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("Mixed strings: {} {} {}\n", "name", 10, 1.25);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, "stderr {}\n", "foo");
    if(FMT_OK != result) printf("fmt_fprint err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, "NULL {}\n", NULL);
    if(FMT_OK != result) printf("fmt_fprint err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, "nullptr {}\n", nullptr);
    if(FMT_OK != result) printf("fmt_fprint err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, NULL, nullptr);
    if(FMT_OK != result) printf("fmt_fprint(stderr, NULL, nullptr) err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, NULL, NULL);
    if(FMT_OK != result) printf("fmt_fprint(stderr, NULL, NULL) err: %s\n", fmt_result_string(result));

#if 0
    // TODO this should stop if arg count is not correct and set error code
    result = fmt_print("Mixed strings2: {} {} {}\n", "name");
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("Mixed strings3: {} {} {}\n", 1);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    // TODO identify individual brace issue
    result = fmt_print("Mixed strings4: {\n", "name");
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));
#endif

    // Show malformed floating point
    const float test = (float)0xFFFFFFFF;
    result = fmt_print("float: {}\n", test);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    const float test2 = NAN;
    result = fmt_print("float: {}\n", test2);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_clear(&buf);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("float: {}\n", test2);

    char temp = {0};
    void * vptr = &temp;
    result = fmt_print("void*: {}\n", vptr);
    if(FMT_OK != result) printf("vptr err: %s\n", fmt_result_string(result));

    my_struct_t my = {10, 3.33f};

    result = fmt_print("my_format: {}\n", my_format(my));
    if(FMT_OK != result) printf("my_format err: %s\n", fmt_result_string(result));

    const int32_t value = 42;
    result = fmt_print("int32_t: {}\n", value);
    if(FMT_OK != result) printf("int32_t err: %s\n", fmt_result_string(result));

    const size_t size = 142;
    result = fmt_print("size_t: {}\n", size);
    if(FMT_OK != result) printf("size_t err: %s\n", fmt_result_string(result));


    return 0;
}
