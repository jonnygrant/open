#ifndef RUNTIME_ASSERT_HPP
#define RUNTIME_ASSERT_HPP

#include <string_view>
#include <source_location>

/* Standarize as follows
runtime_assert(expr)
runtime_assert(expr, message)
*/

namespace std {

typedef void (*runtime_assert_callback)(const char * expr, const char * msg, const std::source_location loc);

void set_assert_runtime_handler(runtime_assert_callback callback) noexcept;

extern runtime_assert_callback __assert_runtime_handler;

inline void __runtime_assert(const char * expr, const char * msg, const std::source_location loc)
{
    __assert_runtime_handler(expr?expr:"", msg?msg:"", loc);
}

#define runtime_assert(expr, msg)                    \
    do                                               \
    {                                                \
        if (!__builtin_constant_p(expr) || !(expr))   \
        {                                            \
            if (!(expr))                             \
            {                                        \
                std::__assert_runtime_handler(#expr, msg, \
                    std::source_location::current());\
            }                                        \
        }                                            \
    } while (0)

} // namespace

#endif // RUNTIME_ASSERT_HPP
