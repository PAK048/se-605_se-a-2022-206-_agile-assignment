#ifndef CATCH_HPP
#define CATCH_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iomanip>

namespace Catch {
    // ANSI Color Codes for Windows Terminal
    class Colors {
    private:
        static inline bool colorsEnabled = false;
    public:
        static constexpr const char* RESET = "\033[0m";
        static constexpr const char* GREEN = "\033[32m";
        static constexpr const char* RED = "\033[31m";
        static constexpr const char* YELLOW = "\033[33m";
        static constexpr const char* BLUE = "\033[34m";
        static constexpr const char* CYAN = "\033[36m";
        static constexpr const char* WHITE = "\033[37m";
        static constexpr const char* BOLD = "\033[1m";
        static constexpr const char* BRIGHT_GREEN = "\033[1;32m";
        static constexpr const char* BRIGHT_RED = "\033[1;31m";
        static constexpr const char* BRIGHT_YELLOW = "\033[1;33m";
        static constexpr const char* BRIGHT_CYAN = "\033[1;36m";

        // Return color code only if colors are enabled
        static const char* get(const char* colorCode) {
            return colorsEnabled ? colorCode : "";
        }

        static void enable(bool enabled = true) {
            colorsEnabled = enabled;
        }

        static bool isEnabled() {
            return colorsEnabled;
        }
    };

    class AssertionException : public std::exception {
        std::string msg;
    public:
        explicit AssertionException(const std::string& s) : msg(s) {}
        const char* what() const noexcept override { return msg.c_str(); }
    };

    typedef void (*TestFunc)();

    struct TestResult {
        std::string name;
        bool passed;
        std::string error;
        double duration;
    };

    struct Registrar {
        struct Test { std::string name; TestFunc func; };
        static std::vector<Test>& getTests() {
            static std::vector<Test> tests;
            return tests;
        }
        Registrar(const std::string& name, TestFunc func) {
            getTests().push_back({name, func});
        }
    };

    // Progress bar implementation
    inline void printProgressBar(int passed, int failed, int total) {
        const int barWidth = 50;
        double passRatio = (double)passed / total;
        double failRatio = (double)failed / total;
        double passPercent = passRatio * 100;
        double failPercent = failRatio * 100;

        int passedBars = (int)(passRatio * barWidth);
        int failedBars = (int)(failRatio * barWidth);

        std::cout << "Progress: [" << Colors::get(Colors::BRIGHT_CYAN);

        for(int i = 0; i < passedBars; ++i) {
            std::cout << Colors::get(Colors::BRIGHT_GREEN) << "+";
        }
        for(int i = 0; i < failedBars; ++i) {
            std::cout << Colors::get(Colors::BRIGHT_RED) << "F";
        }
        std::cout << Colors::get(Colors::WHITE);
        for(int i = passedBars + failedBars; i < barWidth; ++i) {
            std::cout << "-";
        }

        std::cout << Colors::get(Colors::BRIGHT_CYAN) << "] " << Colors::get(Colors::RESET);
        std::cout << std::fixed;
        std::cout << std::setprecision(1);
        std::cout << Colors::get(Colors::BRIGHT_GREEN) << passPercent << "% passed" << Colors::get(Colors::RESET);
        std::cout << " / ";
        std::cout << Colors::get(Colors::BRIGHT_RED) << failPercent << "% failed" << Colors::get(Colors::RESET);
    }

    inline void runAllTests() {
        std::vector<TestResult> results;
        auto tests = Registrar::getTests();
        int totalTests = tests.size();

        std::cout << std::endl;
        std::cout << Colors::get(Colors::BRIGHT_CYAN) << "================================================================" << Colors::get(Colors::RESET) << std::endl;
        std::cout << Colors::get(Colors::BRIGHT_YELLOW) << Colors::get(Colors::BOLD) << "                    CATCH++ TEST RUNNER                    " << Colors::get(Colors::RESET) << std::endl;
        std::cout << Colors::get(Colors::BRIGHT_CYAN) << "================================================================" << Colors::get(Colors::RESET) << std::endl;

        std::cout << "Running " << totalTests << " test(s)..." << std::endl;
        std::cout << "=================================================================" << std::endl;

        int passed = 0, failed = 0;

        for(size_t i = 0; i < tests.size(); ++i) {
            auto& t = tests[i];
            TestResult result;
            result.name = t.name;

            std::cout << Colors::get(Colors::CYAN) << "Test " << (i+1) << "/" << totalTests << ": " << Colors::get(Colors::WHITE) << t.name << Colors::get(Colors::CYAN) << " ... " << Colors::get(Colors::RESET);
            std::cout.flush();

            try {
                t.func();
                result.passed = true;
                result.error = "";
                passed++;

                std::cout << Colors::get(Colors::BRIGHT_GREEN) << "PASSED" << Colors::get(Colors::RESET) << std::endl;

            } catch(const AssertionException& e) {
                result.passed = false;
                result.error = e.what();
                failed++;

                std::cout << Colors::get(Colors::BRIGHT_RED) << "FAILED" << Colors::get(Colors::RESET) << std::endl;
                std::cout << "    --> Error: " << Colors::get(Colors::RED) << e.what() << Colors::get(Colors::RESET) << std::endl;

            } catch(const std::exception& e) {
                result.passed = false;
                result.error = std::string("Exception: ") + e.what();
                failed++;

                std::cout << Colors::get(Colors::BRIGHT_RED) << "FAILED" << Colors::get(Colors::RESET) << std::endl;
                std::cout << "    --> Exception: " << Colors::get(Colors::RED) << e.what() << Colors::get(Colors::RESET) << std::endl;
            }

            results.push_back(result);
        }

        std::cout << Colors::get(Colors::BRIGHT_CYAN) << "=================================================================" << Colors::get(Colors::RESET) << std::endl;
        std::cout << Colors::get(Colors::BRIGHT_YELLOW) << Colors::get(Colors::BOLD) << "                          TEST SUMMARY" << Colors::get(Colors::RESET) << std::endl;
        std::cout << Colors::get(Colors::BRIGHT_CYAN) << "=================================================================" << Colors::get(Colors::RESET) << std::endl;

        std::cout << std::endl;
        printProgressBar(passed, failed, totalTests);
        std::cout << std::endl << std::endl;

        if(failed > 0) {
            std::cout << Colors::get(Colors::BRIGHT_RED) << "FAILED TESTS:" << Colors::get(Colors::RESET) << std::endl;
            std::cout << "----------------------------------------" << std::endl;

            for(const auto& result : results) {
                if(!result.passed) {
                    std::cout << "  " << Colors::get(Colors::BRIGHT_RED) << "[F]" << Colors::get(Colors::RESET) << " " << result.name << std::endl;
                    std::cout << "      Reason: " << result.error << std::endl;
                }
            }
            std::cout << std::endl;
        }

        if(passed > 0) {
            std::cout << Colors::get(Colors::BRIGHT_GREEN) << "PASSED TESTS:" << Colors::get(Colors::RESET) << std::endl;
            std::cout << "----------------------------------------" << std::endl;

            for(const auto& result : results) {
                if(result.passed) {
                    std::cout << "  " << Colors::get(Colors::BRIGHT_GREEN) << "[PASS]" << Colors::get(Colors::RESET) << " " << result.name << std::endl;
                }
            }
            std::cout << std::endl;
        }

        std::cout << "=================================================================" << std::endl;

        if(failed == 0) {
            std::cout << Colors::get(Colors::BRIGHT_GREEN) << Colors::get(Colors::BOLD) << "*** ALL TESTS PASSED! ***" << Colors::get(Colors::RESET) << std::endl;
        } else {
            std::cout << Colors::get(Colors::BRIGHT_RED) << Colors::get(Colors::BOLD) << "!!! SOME TESTS FAILED !!!" << Colors::get(Colors::RESET) << std::endl;
        }

        double successRate = (totalTests > 0) ? ((double)passed / totalTests * 100) : 0;
        std::cout << std::endl;
        std::cout << "FINAL RESULTS:" << std::endl;
        std::cout << "  Total Tests: " << totalTests << std::endl;
        std::cout << "  Tests Passed: " << passed << std::endl;
        std::cout << "  Failed:      " << failed << std::endl;
        std::cout << "  Success Rate: " << std::fixed << std::setprecision(1);
        std::cout << successRate << "%" << std::endl;

        std::cout << "=================================================================" << std::endl;
    }

    // Helper function to disable colors for online terminals
    inline void enableColors() {
        Colors::enable(true);
    }
}

#define REQUIRE(cond) do { \
    if(!(cond)) { \
        std::ostringstream oss; \
        oss << "Requirement failed: " << #cond; \
        throw Catch::AssertionException(oss.str()); \
    } \
} while(0)

#define REQUIRE_EQ(actual,expected) do { \
    auto actual_val = (actual); \
    auto expected_val = (expected); \
    if(!(actual_val == expected_val)) { \
        std::ostringstream oss; \
        oss << "Requirement failed: " #actual " == " #expected; \
        oss << "  expected: " << expected_val; \
        oss << "  actual: " << actual_val; \
        throw Catch::AssertionException(oss.str()); \
    } \
} while(0)

#define REQUIRE_NE(actual,expected) do { \
    auto actual_val = (actual); \
    auto expected_val = (expected); \
    if(!(actual_val != expected_val)) { \
        std::ostringstream oss; \
        oss << "Requirement failed: " #actual " != " #expected; \
        oss << "  actual: " << actual_val; \
        throw Catch::AssertionException(oss.str()); \
    } \
} while(0)

#endif
