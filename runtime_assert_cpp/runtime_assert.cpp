#include <cstdio>
#include <print>
#include <stdexcept>

#include "runtime_assert.hpp"

namespace std {

void __default_assert_runtime_handler(const char * expr,
                           const char * msg, const std::source_location & loc)
{
    std::printf("default runtime_assert: %s:%d %s (%s) message: \"%s\"\n",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);

    std::terminate();
}

void runtime_assert_cpp(bool expr, const char * msg, const std::source_location loc)
{
    __builtin_printf("%d, %s, %s:%d\n", expr, msg, loc.file_name(), loc.line());

    if(!expr)
    {
        __builtin_printf("after __builtin_constant_p\n");

        std::__assert_runtime_handler(msg, msg, loc);
    }
}

runtime_assert_callback __assert_runtime_handler = std::__default_assert_runtime_handler;

void set_assert_runtime_handler(std::runtime_assert_callback handler) noexcept
{
    if(handler)
    {
        __assert_runtime_handler = handler;
    }
    else
    {
        __assert_runtime_handler = std::__default_assert_runtime_handler;
    }
}


} // namespace
