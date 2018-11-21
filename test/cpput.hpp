/******************************************************************************
  Copyright (c) 2011-2014 Tommy Back

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

/******************************************************************************
 *
 *  \brief    A light-weight and easy to use unit testing framework for C++
 *  \details  Header-only unit testing framework that makes unit testing easy
 *            and quick to set up.
 *  \version  0.2.0
 *  \author   Tommy Back
 *
 *
 *  Simply include this header file to get started.
 *
 *  \code
 *  #include <cpput/TestHarness.h>
 *  #include <Foo.h>
 *
 *  TEST(Foo, some_descriptive_name)
 *  {
 *    // Arrange
 *    Foo foo;
 *    // Act
 *    bool result = foo.isBar();
 *    // Assert
 *    ASSERT_TRUE(result);
 *  }
 *  \endcode
 *
 *  In case you want to keep a single file with tests that compile to an
 *  executable you can also add the main function to the end of the file.
 *  This is simple to do with the provided macro:
 *
 *  \code
 *  CPPUT_TEST_MAIN;
 *  \endcode
 *
 *  For larger test suits, it's recommended to group the tests per class
 *  in separate files and let the compiler combine them into a single
 *  executable which has a main.cpp file that only has the main function
 *  declared.
 *
 *  \example
 *  [Test_Foo.cpp]
 *
 *  \code
 *  #include <cpput/TestHarness.h>
 *  #include <Foo.h>
 *
 *  TEST(Foo, foo_bar_z)
 *  {
 *    ...
 *  \endcode
 *
 *  [main.cpp]
 *
 *  \code
 *  #include <cpput/TestHarness.h>
 *
 *  CPPUT_TEST_MAIN;
 *  \endcode
 *
 */

#ifndef CPPUT_TESTHARNESS_HPP
#define CPPUT_TESTHARNESS_HPP

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// #define BOOST_STACKTRACE_USE_BACKTRACE
#define BOOST_STACKTRACE_USE_ADDR2LINE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <boost/stacktrace.hpp>
#pragma GCC diagnostic pop

namespace cpput {

struct AssertionFailureException : public std::exception {};
// ----------------------------------------------------------------------------

struct ResultWriter {
    virtual ~ResultWriter() {}

    virtual void startTest(const std::string& className, const std::string& name) = 0;
    virtual void endTest(bool success) = 0;

    virtual void failure(const std::string& filename, std::size_t line, const std::string& message) = 0;
    virtual int getNumberOfFailures() const = 0;
};

// ----------------------------------------------------------------------------

class TextResultWriter : public ResultWriter {
 public:
    TextResultWriter() : testCount_(0), failures_(0) {}

    virtual ~TextResultWriter() {
        if (failures_ == 0) {
            std::cout << "\nAll tests pass.\n";
            return;
        }
        std::cout << "\n" << failures_ << " out of " << testCount_ << " tests failed.\n";
        std::string s = ss.str();
        if (s != "") { std::cout << s << std::flush; }
    }

    virtual void startTest(const std::string&, const std::string&) { testCount_++; }

    virtual void endTest(bool success) {
        if (success)
            std::cout << '.';
        else
            std::cout << 'F';
        std::cout << std::flush;
    }

    virtual void failure(const std::string& filename, std::size_t line, const std::string& message) {
        failures_++;
        ss << "------------------------------------------------------------" << std::endl;
        ss << "Failure: " << filename << ", line " << line << ": " << message << std::endl;
        ss << boost::stacktrace::stacktrace();
    }

    virtual int getNumberOfFailures() const { return failures_; }

 private:
    int testCount_;
    int failures_;
    std::stringstream ss;
};

// ----------------------------------------------------------------------------

class XmlResultWriter : public ResultWriter {
 public:
    XmlResultWriter() : startTime_(0), failureCount_(0) {
        std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        std::cout << "<testsuite>\n";
    }

    virtual ~XmlResultWriter() { std::cout << "</testsuite>\n"; }

    virtual void startTest(const std::string& className, const std::string& name) {
        startTime_ = std::clock();
        std::cout << "  <testcase classname=\"" << className << "\" name=\"" << name << "\" time=\"";
    }

    virtual void endTest(bool success) {
        if (success) {
            std::cout << static_cast<float>(std::clock() - startTime_) / CLOCKS_PER_SEC << "\"";
            std::cout << "/>\n";
        } else
            std::cout << "  </testcase>\n";
    }

    virtual void failure(const std::string& filename, std::size_t line, const std::string& message) {
        std::cout << static_cast<float>(std::clock() - startTime_) / CLOCKS_PER_SEC << "\"";
        std::cout << ">\n"
                  << "    <failure>" << message << " in " << filename << ", line " << line << "</failure>\n";
        failureCount_++;
    }

    virtual int getNumberOfFailures() const { return failureCount_; }

 private:
    std::clock_t startTime_;
    int failureCount_;
};

// ----------------------------------------------------------------------------
template <typename T>
class has_operator_rrshift {
    typedef char one;
    typedef long two;

    template <typename C>
    static one test(decltype(&C::operator << ));
    template <typename C>
    static two test(...);

 public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};


struct Result {
    static Result *current(Result *new_res = nullptr) {
        static Result *cur = nullptr;
        if (new_res) {
            cur = new_res;
        }
        return cur;
    }

    void start(const std::string& testClassName, const std::string& testName, ResultWriter& out) {
        test_class_name = testClassName;
        test_name = testName;

        out_ = &out;
        out_->startTest(testClassName, testName);
        current(this);
    }

    void end() {
        out_->endTest(pass_);
        out_ = nullptr;
    }

    template <typename T, typename U>
    typename std::enable_if<
        (std::is_fundamental<T>::value and std::is_fundamental<U>::value) or
        (has_operator_rrshift<T>::value and has_operator_rrshift<U>::value), void>::type
    addFailure(const char* filename, std::size_t line, T expected, U actual) {
        pass_ = false;
        std::stringstream ss;
        ss << std::setprecision(20) << "failed comparison, expected " << expected << " got " << actual;
        out_->failure(filename, line, ss.str());
    }

    template <typename T, typename U>
    typename std::enable_if<not (
        (std::is_fundamental<T>::value and std::is_fundamental<U>::value) or
        (has_operator_rrshift<T>::value and has_operator_rrshift<U>::value)), void>::type
    addFailure(const char* filename, std::size_t line, T expected, U actual) {
        pass_ = false;
        std::stringstream ss;
        ss << std::setprecision(20) << "failed comparison, expected";
        out_->failure(filename, line, ss.str());
    }

    void addFailure(const char* filename, std::size_t line, const char* message) {
        pass_ = false;
        out_->failure(filename, line, message);
    }
    
    std::string test_class_name, test_name;

    ResultWriter* out_;
    bool pass_ = true;
};

inline std::string curTestClassName() {
    if (Result::current()) {
        return Result::current()->test_class_name;
    }
    return "<none>";
}

inline std::string curTestName() {
    if (Result::current()) {
        return Result::current()->test_name;
    }
    return "<none>";
}

// ----------------------------------------------------------------------------

class Repository;

class Test {
    friend class Repository;

 public:
    Test(const char* className, const char* name);
    virtual ~Test() {}

    void run(ResultWriter& out) {
        testResult_.start(test_unit_class_name_, test_unit_name_, out);
        try {
            do_run();
        } catch (const ::cpput::AssertionFailureException& e) {
            // skip
        } catch (const std::exception& e) {
            testResult_.addFailure(__FILE__, __LINE__, std::string("Unexpected exception: ").append(e.what()).c_str());
        } catch (...) { testResult_.addFailure(__FILE__, __LINE__, "Unspecified exception!"); }
        testResult_.end();
    }

    Test* next() { return test_unit_next_; }

 private:
    virtual void do_run() = 0;

 protected:
    std::string test_unit_class_name_;
    std::string test_unit_name_;
    Test* test_unit_next_;
    Result testResult_;
};

// ----------------------------------------------------------------------------

class Repository {
 public:
    static Repository& instance() {
        static Repository repo;
        return repo;
    }

    void add(Test* tc) {
        if (!tests_) {
            tests_ = tc;
            return;
        }

        // add as last
        Test* tmp = tests_;
        while (tmp->test_unit_next_) tmp = tmp->test_unit_next_;
        tmp->test_unit_next_ = tc;
    }

    Test* getTests() { return tests_; }

 private:
    Repository() : tests_(0) {}
    Repository(const Repository& other);
    Repository& operator=(const Repository& rhs) const;

 private:
    Test* tests_;
};

inline Test::Test(const char* className, const char* name)
    : test_unit_class_name_(className), test_unit_name_(name), test_unit_next_(0) {
    Repository::instance().add(this);
}

// ----------------------------------------------------------------------------

inline int runAllTests(ResultWriter& writer) {
    Test* c = Repository::instance().getTests();
    while (c) {
        c->run(writer);
        c = c->next();
    }
    return writer.getNumberOfFailures();
}

}  // namespace cpput

// Convenience macro to get main function.
#define CPPUT_TEST_MAIN                                 \
    if (argc == 2 && std::string(argv[1]) == "--xml") { \
        ::cpput::XmlResultWriter writer;                \
        return ::cpput::runAllTests(writer);            \
    }                                                   \
    ::cpput::TextResultWriter writer;                   \
    ::cpput::runAllTests(writer);                       \


// ----------------------------------------------------------------------------
// Test Macros
// ----------------------------------------------------------------------------

/// Stand-alone test case.
///
#define TEST(group, name)                                     \
    class group##name##Test : public ::cpput::Test {          \
     public:                                                  \
        group##name##Test() : ::cpput::Test(#group, #name) {} \
        virtual ~group##name##Test() {}                       \
                                                              \
     private:                                                 \
        virtual void do_run();                                \
    } group##name##TestInstance;                              \
    inline void group##name##Test::do_run()

/// Test case with fixture.
///
#define TEST_F(group, name)                          \
    class group##name##FixtureTest : public group {  \
     public:                                         \
        void do_run();                               \
    };                                               \
    class group##name##Test : public ::cpput::Test { \
     public:                                         \
        group##name##Test() : Test(#group, #name) {} \
        virtual void do_run() override;              \
    } group##name##TestInstance;                     \
    inline void group##name##Test::do_run() {        \
        group##name##FixtureTest test;               \
        test.do_run();                               \
    }                                                \
    inline void group##name##FixtureTest::do_run()

// ----------------------------------------------------------------------------
// Assertion Macros
// ----------------------------------------------------------------------------

#define ASSERT_TRUE(expression)                                      \
    {                                                                \
        if (!(expression)) {                                         \
            ::cpput::Result::current()->addFailure(__FILE__, __LINE__, #expression); \
            throw ::cpput::AssertionFailureException();              \
        }                                                            \
    }

#define ASSERT_FALSE(expression) ASSERT_TRUE(!(expression))

#define ASSERT_EQ(expected, actual)                                       \
    {                                                                     \
        if (!((expected) == (actual))) {                                  \
            ::cpput::Result::current()->addFailure(__FILE__, __LINE__, expected, actual); \
            throw ::cpput::AssertionFailureException();                   \
        }                                                                 \
    }

#define ASSERT_NEQ(expected, actual)                                      \
    {                                                                     \
        if (((expected) == (actual))) {                                   \
            ::cpput::Result::current()->addFailure(__FILE__, __LINE__, expected, actual); \
            throw ::cpput::AssertionFailureException();                   \
        }                                                                 \
    }

#define ASSERT_STREQ(expected, actual)                                    \
    {                                                                     \
        if (!(std::string(expected) == std::string(actual))) {            \
            ::cpput::Result::current()->addFailure(__FILE__, __LINE__, expected, actual); \
            throw ::cpput::AssertionFailureException();                   \
        }                                                                 \
    }

#define ASSERT_NEAR(expected, actual, epsilon)                                                    \
    {                                                                                             \
        double _tmp_var_actualTmp = actual;                                                       \
        double _tmp_var_expectedTmp = expected;                                                   \
        double _tmp_var_diff = _tmp_var_expectedTmp - _tmp_var_actualTmp;                         \
        if ((_tmp_var_diff > epsilon) || (-_tmp_var_diff > epsilon)) {                            \
            ::cpput::Result::current()->addFailure(__FILE__, __LINE__, _tmp_var_expectedTmp, _tmp_var_actualTmp); \
            throw ::cpput::AssertionFailureException();                                           \
        }                                                                                         \
    }

#endif  // CPPUT_TESTHARNESS_HPP
