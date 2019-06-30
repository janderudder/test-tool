#pragma once
#include <iostream>
#include <stdexcept>
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

/*  This is used as an object to take advantage of static initialization
    and output text at program exit.  */
struct Test_Driver
{
    static inline auto tests_passed = 0ULL;
    static inline auto tests_failed = 0ULL;


    ~Test_Driver()
    {
        std::cout
            << "\nEnd of "  << (tests_failed + tests_passed) << " tests:\n"
            << "  passed: " << tests_passed << "\n"
            << "  failed: " << tests_failed << "\n";
        tests_passed = 0;
        tests_failed = 0;
    }


    void pass(const char* reason) const noexcept
    {
        tests_passed++;
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

        std::cout
            << "\n"
            << "test FAILED: " << reason << "\n"
            << "  (" << cond_str << ")\n"
            << "in " << file << ":" << std::to_string(line) << "\n"
            << "   " << func << "\n"
        ;
    }

};

static Test_Driver td;


// Macro ENSURE expands to this
void ensure(bool condition, const char* cond_str,
    const char* reason, const char* func, const char* file, int line)
{
    if (condition)
        td.pass(reason);
    else
        td.fail(cond_str, reason, func, file, line);
}


} // namespace test::impl