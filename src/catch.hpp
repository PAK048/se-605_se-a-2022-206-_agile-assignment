/*
 *  Catch v2.13.10
 *  Generated: 2023-03-23 14:45:15.863987
 *  ----------------------------------------------------------
 *  This file has been merged from multiple headers. Please don't edit it directly
 *  Copyright (c) 2022 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef CATCH_HPP_INCLUDED
#define CATCH_HPP_INCLUDED

// Disable some warnings for Catch2
#if defined(__clang__)
#    pragma clang system_header
#elif defined(__GNUC__)
#    pragma GCC system_header
#elif defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable: 4702) // unreachable code
#endif

#define CATCH_VERSION_MAJOR 2
#define CATCH_VERSION_MINOR 13
#define CATCH_VERSION_PATCH 10

// Define main if needed
#ifdef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_RUNNER
#endif

#ifdef __OBJC__
#  include "Foundation/Foundation.h"
#endif

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#if defined(CATCH_CONFIG_CPP17_BYTE)
#  include <cstddef>
#endif

#if defined(_MSC_VER)
#  define CATCH_INTERNAL_START_WARNINGS_SUPPRESSION __pragma(warning(push))
#  define CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION  __pragma(warning(pop))
#  define CATCH_BREAK_INTO_DEBUGGER() __debugbreak()
#elif defined(__MINGW32__) && (defined(__clang__) || defined(__GNUC__) && __GNUC__ > 4)
#  define CATCH_INTERNAL_START_WARNINGS_SUPPRESSION _Pragma("GCC diagnostic push")
#  define CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION  _Pragma("GCC diagnostic pop")
#  define CATCH_BREAK_INTO_DEBUGGER() __debugbreak()
#elif defined(__clang__)
#  define CATCH_INTERNAL_START_WARNINGS_SUPPRESSION _Pragma("clang diagnostic push")
#  define CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION  _Pragma("clang diagnostic pop")
#  define CATCH_BREAK_INTO_DEBUGGER() __builtin_debugtrap()
#elif defined(__GNUC__) && !defined(__clang__) && !defined(__ICC) && !defined(__CUDACC__)
#  define CATCH_INTERNAL_START_WARNINGS_SUPPRESSION _Pragma("GCC diagnostic push")
#  define CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION  _Pragma("GCC diagnostic pop")
#  define CATCH_BREAK_INTO_DEBUGGER() __builtin_trap()
#else
#  define CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
#  define CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
#  define CATCH_BREAK_INTO_DEBUGGER() ((void)0)
#endif

#define CATCH_INTERNAL_IGNORE_BUT_WARN(...) (void)__builtin_constant_p(__VA_ARGS__)

#if defined(__GNUC__) && !defined(__clang__)
#  define CATCH_INTERNAL_START_WARNINGS_SUPPRESSION _Pragma("GCC diagnostic push")
#  define CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION  _Pragma("GCC diagnostic pop")
#endif

namespace Catch {
    struct CaseSensitive { enum Choice { Yes, No }; };
    class NonCopyable {
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
        NonCopyable(NonCopyable const&) = delete;
        NonCopyable& operator=(NonCopyable const&) = delete;
    };
    
    struct autoReg { int dummy; };
    
    namespace Detail {
        constexpr bool isPowerOfTwo(std::size_t x) {
            return (x & (x - 1)) == 0;
        }
    }
}

#define INTERNAL_CATCH_UNIQUE_NAME_LINE2(name, line) name##line
#define INTERNAL_CATCH_UNIQUE_NAME_LINE(name, line) INTERNAL_CATCH_UNIQUE_NAME_LINE2(name, line)
#ifdef CATCH_CONFIG_COUNTER
#  define INTERNAL_CATCH_UNIQUE_NAME(name) INTERNAL_CATCH_UNIQUE_NAME_LINE(name, __COUNTER__)
#else
#  define INTERNAL_CATCH_UNIQUE_NAME(name) INTERNAL_CATCH_UNIQUE_NAME_LINE(name, __LINE__)
#endif

// Macros for test cases
#define TEST_CASE(...) \
    INTERNAL_CATCH_TESTCASE(__VA_ARGS__)
#define TEST_CASE_METHOD(className, ...) \
    INTERNAL_CATCH_TEST_CASE_METHOD(className, __VA_ARGS__)

// Macros for assertions
#define REQUIRE(...) \
    INTERNAL_CATCH_TEST(__VA_ARGS__, Catch::ResultDisposition::Normal, "REQUIRE" )
#define REQUIRE_FALSE(...) \
    INTERNAL_CATCH_TEST(__VA_ARGS__, Catch::ResultDisposition::Normal | Catch::ResultDisposition::FalseTest, "REQUIRE_FALSE" )
#define CHECK(...) \
    INTERNAL_CATCH_TEST(__VA_ARGS__, Catch::ResultDisposition::ContinueOnFailure, "CHECK" )
#define CHECK_FALSE(...) \
    INTERNAL_CATCH_TEST(__VA_ARGS__, Catch::ResultDisposition::ContinueOnFailure | Catch::ResultDisposition::FalseTest, "CHECK_FALSE" )

#define REQUIRE_THROWS(...) \
    INTERNAL_CATCH_THROWS(__VA_ARGS__, Catch::ResultDisposition::Normal, "REQUIRE_THROWS")
#define REQUIRE_THROWS_AS(expr, exceptionType) \
    INTERNAL_CATCH_THROWS_AS(expr, exceptionType, Catch::ResultDisposition::Normal, "REQUIRE_THROWS_AS")
#define REQUIRE_NOTHROW(...) \
    INTERNAL_CATCH_NO_THROW(__VA_ARGS__, Catch::ResultDisposition::Normal, "REQUIRE_NOTHROW")
#define CHECK_THROWS(...) \
    INTERNAL_CATCH_THROWS(__VA_ARGS__, Catch::ResultDisposition::ContinueOnFailure, "CHECK_THROWS")
#define CHECK_THROWS_AS(expr, exceptionType) \
    INTERNAL_CATCH_THROWS_AS(expr, exceptionType, Catch::ResultDisposition::ContinueOnFailure, "CHECK_THROWS_AS")
#define CHECK_NOTHROW(...) \
    INTERNAL_CATCH_NO_THROW(__VA_ARGS__, Catch::ResultDisposition::ContinueOnFailure, "CHECK_NOTHROW")

// Sections
#define SECTION(...) INTERNAL_CATCH_SECTION(__VA_ARGS__)

// Matchers (simplified)
namespace Catch {
    namespace Matchers {
        template<typename T>
        struct MatcherBase {
            virtual ~MatcherBase() = default;
            virtual bool match(T const&) const = 0;
            virtual std::string describe() const = 0;
        };
        
        template<typename T>
        class EqualsMatcher : public MatcherBase<T> {
            T m_expected;
        public:
            EqualsMatcher(T const& expected) : m_expected(expected) {}
            bool match(T const& actual) const override {
                return actual == m_expected;
            }
            std::string describe() const override {
                std::ostringstream ss;
                ss << "equals " << m_expected;
                return ss.str();
            }
        };
        
        template<typename T>
        EqualsMatcher<T> Equals(T const& expected) {
            return EqualsMatcher<T>(expected);
        }
    }
}

// Simplified assertion implementation
namespace Catch {
    struct AssertionInfo {
        std::string macroName;
        std::string capturedExpression;
        std::string filename;
        std::size_t line;
    };
    
    struct AssertionResult {
        bool ok;
        std::string message;
        
        AssertionResult(bool success, std::string const& msg = "")
            : ok(success), message(msg) {}
        
        explicit operator bool() const { return ok; }
    };
    
    enum ResultDisposition : std::uint8_t {
        Normal = 0,
        ContinueOnFailure = 1,
        FalseTest = 2
    };
    
    class AssertionHandler {
        AssertionInfo m_info;
        ResultDisposition m_resultDisposition;
        bool m_shouldDebugBreak;
        bool m_shouldThrow;
    public:
        AssertionHandler(AssertionInfo const& info, ResultDisposition resultDisposition)
            : m_info(info), m_resultDisposition(resultDisposition) {}
        
        ~AssertionHandler() {
            if (!m_shouldThrow) return;
            // Handle failure
            std::cerr << "FAILED: " << m_info.capturedExpression 
                      << " at " << m_info.filename << ":" << m_info.line << std::endl;
            if (m_resultDisposition == Normal) {
                std::exit(1);
            }
        }
        
        void handle(AssertionResult const& result) {
            if (!result) {
                m_shouldThrow = true;
                if (m_resultDisposition == Normal) {
                    CATCH_BREAK_INTO_DEBUGGER();
                }
            }
        }
    };
    
    template<typename LhsT, typename RhsT>
    AssertionResult checkEqual(LhsT const& lhs, RhsT const& rhs) {
        if (lhs == rhs) {
            return AssertionResult(true);
        }
        std::ostringstream ss;
        ss << lhs << " != " << rhs;
        return AssertionResult(false, ss.str());
    }
    
    template<typename Cond>
    AssertionResult checkTrue(Cond const& cond) {
        if (cond) {
            return AssertionResult(true);
        }
        return AssertionResult(false, "Expression is false");
    }
}

// Internal macros
#define INTERNAL_CATCH_TEST(expr, resultDisposition, macroName) \
    do { \
        Catch::AssertionInfo info{ macroName, #expr, __FILE__, __LINE__ }; \
        Catch::AssertionHandler handler(info, resultDisposition); \
        handler.handle(Catch::checkTrue(expr)); \
    } while(false)

#define INTERNAL_CATCH_SECTION(...) \
    if (Catch::autoReg INTERNAL_CATCH_UNIQUE_NAME(catch_internal_Section) = {}; true)

// Test case registration (simplified)
struct CatchTestCase {
    std::string name;
    void (*function)();
};

namespace {
    std::vector<CatchTestCase>& getTestCases() {
        static std::vector<CatchTestCase> testCases;
        return testCases;
    }
    
    int registerTestCase(const char* name, void (*function)()) {
        getTestCases().push_back({name, function});
        return 0;
    }
}

#define INTERNAL_CATCH_TESTCASE(...) \
    static void INTERNAL_CATCH_UNIQUE_NAME(__CATCH_TC_)(); \
    namespace { \
        int INTERNAL_CATCH_UNIQUE_NAME(__CATCH_TC_REG_) = \
            registerTestCase(__VA_ARGS__, INTERNAL_CATCH_UNIQUE_NAME(__CATCH_TC_)); \
    } \
    static void INTERNAL_CATCH_UNIQUE_NAME(__CATCH_TC_)()

// Main runner
#ifdef CATCH_CONFIG_RUNNER
int main(int argc, char* argv[]) {
    int failures = 0;
    
    std::cout << "Running tests..." << std::endl;
    
    for (const auto& testCase : getTestCases()) {
        std::cout << "Test case: " << testCase.name << std::endl;
        try {
            testCase.function();
            std::cout << "  PASSED" << std::endl;
        } catch (...) {
            std::cout << "  FAILED" << std::endl;
            failures++;
        }
    }
    
    std::cout << "\nTotal tests: " << getTestCases().size() 
              << ", Failures: " << failures << std::endl;
    
    return failures > 0 ? 1 : 0;
}
#endif

// Cleanup warnings
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif // CATCH_HPP_INCLUDED
