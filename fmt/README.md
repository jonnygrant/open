# fmt
Implements {fmt} style formatting in C in fmt.h

# Alignment with C++ {fmt}
Needs improvements to align with C++ {fmt} Format Specifiers.

<https://open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0645r4.html>



# Known issues to address
Add fmt_print_string()
Identify mismatch of {} and args and set error (should avoid any string modification until checked the count matches)
Handle escaped {{}}
 Could validate implemenation using compile_assert()
Zero argument handling fmt_("Hello world!\n); does not work

# TODO

#define DEBUG(x) x

DEBUG(fmt_append_string(buf, "{INT: ");)
DEBUG(fmt_append_string(buf, "}");)

# Testsuite
Create a variety of tests which check the output is expected, and that error return is accurate
Add fmt_copy() test

