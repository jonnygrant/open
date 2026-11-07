#include <cstdio>
#include <print>
#include <exception>

#include "runtime_assert.hpp"

namespace std {

void __default_assert_runtime_handler(std::string_view expr,
                           std::string_view msg, const std::source_location & loc)
{
    std::println(stderr, "default runtime_assert: {}:{} {} ({}) message: \"{}\"",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);

    throw std::logic_error(std::string(msg));
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
