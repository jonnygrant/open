#include <cstdio>
#include <print>
#include <stdexcept>

#include "runtime_assert.hpp"

namespace std {

void __default_assert_runtime_handler(const char * expr,
                           const char * msg, const std::source_location loc)
{
    std::printf("default runtime_assert:\n %s:%d\n %s\n expr (%s)\n message: \"%s\"\n",
            loc.file_name(), loc.line(), loc.function_name(), expr?expr:"", msg?msg:"");

    std::terminate();
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
