#include <cstdio>
#include <print>
#include <string_view>
#include <source_location>

#include "runtime_assert.hpp"


void log_handler(std::string_view expr, std::string_view msg, const std::source_location & loc)
{
    std::println("log_handler runtime_assert: {}:{} {} ({}) message: \"{}\"",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);
}


void custom_assert_handler(std::string_view expr, std::string_view msg, const std::source_location & loc)
{
    std::println("custom runtime_assert: {}:{} {} ({}) message: \"{}\"",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);

    std::terminate();
}


void test()
{
    try
    {
        runtime_assert(2 == 3, "exception check");
    }
    catch (const std::logic_error & e)
    {
        std::println("std::logic_error: \"{}\"", e.what());
    }
    catch (const std::exception & e)
    {
        std::println("Caught: \"{}\"", e.what());
    }
}


int main()
{
    test();

    std::set_assert_runtime_handler(log_handler);

    runtime_assert(1 == 2, "no message");

    set_assert_runtime_handler(custom_assert_handler);

    runtime_assert(2 == 3, "no comment");

    {
        const char * buf = NULL;
        runtime_assert(buf != NULL, "error NULL");
    }

    return 0;
}
