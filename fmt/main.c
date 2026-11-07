#include <math.h>

#include "fmt.h"

#define __FILE_LINE__ __FILE__ ":" STRINGIFY(__LINE__)

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

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

    result = fmt_print("{} {} {} {}", 1);
    if(FMT_OK != result) printf("%s fmt_print err: %s\n", __FILE_LINE__, fmt_result_string(result));

    fmt_string_t buf;
    buf = fmt_format("Hello {} newline\n", "planet");
    printf("%s", fmt_string_data(&buf));

    result = fmt_print("Again {}", fmt_string_data(&buf));
    if(FMT_OK != result) printf("fmt_string_data err: %s\n", fmt_result_string(result));

    // free any heap that was allocated
    result = fmt_free(&buf);
    if(FMT_OK != result) printf("fmt_free err: %s\n", fmt_result_string(result));

    fmt_tag_t my_tag = {FMT_BOOL, .data.b = true};
    result = fmt_print_impl(stdout, "my_tag {}\n", &my_tag, 1);

    result = fmt_print("pi = {}\n", 3.14159);
    result = fmt_print("FMT_ARG pi = {}\n", FMT_ARG(3.14159));

    // Not supported, would cause a dangling pointer
    //fmt_print("fmt_format pi = {}\n", fmt_format("{}", 3.14159));

    float my_float = 6.1f;
    fmt_string_t buf2 = fmt_format("{}", my_float);
    result = fmt_print("my_float {}\n", &buf2);
    if(FMT_OK != result) printf("%s fmt_print err: %s\n", __FILE_LINE__, fmt_result_string(result));

    result = fmt_free(&buf2);
    if(FMT_OK != result) printf("fmt_free err: %s\n", fmt_result_string(result));

    //fmt_print("my name {}\n", &"name");

    fmt_string_t my_buf = FMT_INIT;
    my_buf = fmt_format("Hello {}", "London", "Today");
    if(FMT_OK != my_buf.result) printf("my_buf err: %s\n", fmt_result_string(my_buf.result));
    printf("1 %s\n", fmt_string_data(&my_buf));

    fmt_string_t buf_copy = fmt_copy(&my_buf);
    if(FMT_OK != buf_copy.result) printf("buf_copy err: %s\n", fmt_result_string(buf_copy.result));

    printf("2 %s\n", fmt_string_data(&buf_copy));

    result = fmt_fprint_string(stdout, &buf_copy);
    if(FMT_OK != result) printf("fmt_fprint_string err: %s\n", fmt_result_string(result));
    result = fmt_print("\n\n", 1);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("Hello {}\n", "world");
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("int = {}\n", 42);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_print("Mixed strings: {} {} {}\n", "name", 10, 1.25);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, "{} stderr {}\n", __FILE_LINE__, "foo");
    if(FMT_OK != result) printf("fmt_fprint err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, "{} NULL {}\n", __FILE_LINE__, NULL);
    if(FMT_OK != result) printf("fmt_fprint err: %s\n", fmt_result_string(result));

    result = fmt_fprint(stderr, "{} nullptr {}\n", __FILE_LINE__, nullptr);
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

    const char byte = 'a';
    result = fmt_print("char: {}\n", byte);
    if(FMT_OK != result) printf("char err: %s\n", fmt_result_string(result));

    fmt_string_t first = fmt_format("{}", "first ");
    if(FMT_OK != fmt_result_code(&first)) printf("fmt_format err: %s\n", fmt_result(&first));

    result = fmt_format_append(&first, "{}", "second ");
    if(FMT_OK != result) printf("fmt_format second err: %s\n", fmt_result_string(result));

    fmt_string_t third = fmt_format("{}", "third\n");

    result = fmt_concat(&first, &third);
    if(FMT_OK != result) printf("fmt_concat err: %s\n", fmt_result_string(result));

    result = fmt_print("{}", &first);
    if(FMT_OK != result) printf("fmt_print err: %s\n", fmt_result_string(result));

    const fmt_result_t free_result = fmt_free(&buf);
    if(FMT_OK != result) printf("fmt_free err: %s\n", fmt_result_string(free_result));

#if 0
    {
        static int int_buf[] = {1,2,3};
        result = fmt_print("int_buf: {}\n", int_buf);
        if(FMT_OK != result) printf("int_buf err: %s\n", fmt_result_string(result));
    }
#endif

#if 0
    {
        const char * const char_str_ptr = "Message";
        result = fmt_print("char_str_ptr: {}\n", char_str_ptr);
        if(FMT_OK != result) printf("char_str_ptr err: %s\n", fmt_result_string(result));
    }
#endif

    return 0;
}
