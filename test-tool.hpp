#pragma once
#include <iostream>
#include <string>
#include <string_view>

/**
 *  Public interface,
 *  assertion-like macro.
 */
#define ENSURE(cond, reason) ::test::impl::ensure(  \
    (cond), (#cond), (reason), (__PRETTY_FUNCTION__),   \
    (__FILE__), (__LINE__))


/**
 *  Impl.
 */
namespace test::impl
{

//* Helper class tasked with outputting text and counting
struct Statistics
{
    static inline auto tests_passed = 0ULL;
    static inline auto tests_failed = 0ULL;
    static inline bool fail_mode    = false;

    ~Statistics()
    {
        std::cout
            << "\n"
            << "Tests total: " << (tests_failed + tests_passed) << "\n"
            << "     passed: " << tests_passed << "\n"
            << "     failed: " << tests_failed << "\n";
        tests_passed = 0;
        tests_failed = 0;
    }


    void pass(const char* reason) const noexcept
    {
        tests_passed++;
        fail_mode = false;
        std::cout << "test passed: " << reason  << "\n";
    }


    void fail(
        const char* cond_str,
        const char* reason,
        const char* func,
        const char* file,
        int         line
    )const noexcept
    {
        tests_failed++;

        if (!fail_mode) {
            fail_mode = true;
            std::cout << "\n";
        }

        std::cout
            << "test FAILED: " << reason << "\n"
            << "  (" << cond_str << ")\n"
            << "in " << file << ":" << std::to_string(line) << "\n"
            << "     " << func << "\n\n"
        ;
    }

};


//* Unique instance of our helper
inline Statistics td;


//* The ENSURE macro expands to that
inline void ensure(bool condition, const char* cond_str,
    const char* reason, const char* func, const char* file, int line)
{
    if (condition)
        td.pass(reason);
    else
        td.fail(cond_str, reason, func, file, line);
}


} // namespace test::impl
