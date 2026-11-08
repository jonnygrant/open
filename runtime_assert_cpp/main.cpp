#include <cstdio>
#include <source_location>
#include <exception>
#include <stdexcept>

#include "runtime_assert.hpp"

void log_handler(const char * expr, const char * msg, const std::source_location & loc)
{
    std::printf("log_handler runtime_assert: %s:%d %s (%s) message: \"%s\"\n",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);
}


void custom_assert_handler(const char * expr, const char * msg, const std::source_location & loc)
{
    std::printf("custom runtime_assert: %s:%d %s (%s) message: \"%s\"\n",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);

    throw std::logic_error(msg);
}


void test()
{
    std::set_assert_runtime_handler(custom_assert_handler);

    try
    {
        runtime_assert(2 == 3, "exception check");
    }
    catch (const std::logic_error & e)
    {
        std::printf("std::logic_error: \"%s\"\n", e.what());
    }
    catch (const std::exception & e)
    {
        std::printf("Caught: \"%s\"\n", e.what());
    }

    std::set_assert_runtime_handler(nullptr);
}

void run(const char * name, int value)
{
    runtime_assert(nullptr != name, "error name is nullptr");
    runtime_assert(value > 0 && value <= 100, "value must be within bounds");

    std::printf("%s: %d\n", name, value);
}

bool device_connect()
{
    // do checks here
    return false;
}

void set_degraded_safe_mode()
{
}


void safe_handler(const char * expr, const char * msg, const std::source_location & loc)
{
    std::printf("log error runtime_assert: %s:%d %s (%s) message: \"%s\"\n",
            loc.file_name(), loc.line(), loc.function_name(), expr, msg);

    set_degraded_safe_mode();
    // continues
}

void safe_function()
{
    std::set_assert_runtime_handler(safe_handler);

    runtime_assert(device_connect(), "Device connect failure");

    std::set_assert_runtime_handler(nullptr);

    // continue
}


int main()
{
    //test();

    run("Jonathan", 140);

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
