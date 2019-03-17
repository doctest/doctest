## Reporters

Doctest has a modular reporter/listener system with which users can write their own reporters and register them. The reporter interface can also be used for "listening" to events.

You can list all registered reporters with ```--list-reporters```. There are a few implemented reporters in the framework:
- ```console``` - streaming - writes normal lines of text with coloring if a capable terminal is detected
- ```xml``` - streaming - writes in xml format tailored to doctest

Streaming means that results are delivered progressively and not at the end of the test run.

The output is by default written to ```stdout``` but can be redirected with the use of the ```--out=<filename>``` [**command line option**](commandline.md).

Example how to define your own reporter:

```c++
#include <doctest/doctest.h>

#include <mutex>

using namespace doctest;

struct MyXmlReporter : public IReporter
{
    // caching pointers/references to objects of these types - safe to do
    std::ostream&         stdout_stream;
    const ContextOptions& opt;
    const TestCaseData*   tc;
    std::mutex            mutex;

    // constructor has to accept the ContextOptions by ref as a single argument
    MyXmlReporter(const ContextOptions& in)
            : stdout_stream(*in.stdout_stream)
            , opt(in) {}

    void test_run_start() override {}

    void test_run_end(const TestRunStats& /*in*/) override {}

    void test_case_start(const TestCaseData& in) override { tc = &in; }

    void test_case_end(const CurrentTestCaseStats& /*in*/) override {}

    void test_case_exception(const TestCaseException& /*in*/) override {}

    void subcase_start(const SubcaseSignature& /*in*/) override {}

    void subcase_end(const SubcaseSignature& /*in*/) override {}

    void log_assert(const AssertData& in) override {
        // don't include successful asserts by default - this is done here
        // instead of in the framework itself because doctest doesn't know
        // if/when a reporter/listener cares about successful results
        if(!in.m_failed && !opt.success)
            return;

        // make sure there are no races - this is done here instead of in the
        // framework itself because doctest doesn't know if reporters/listeners
        // care about successful asserts and thus doesn't lock a mutex unnecessarily
        std::lock_guard<std::mutex> lock(mutex);

        // ...
    }

    void log_message(const MessageData& /*in*/) override {
        // messages too can be used in a multi-threaded context - like asserts
        std::lock_guard<std::mutex> lock(mutex);

        // ...
    }

    void test_case_skipped(const TestCaseData& /*in*/) override {}
};

// "1" is the priority - used for ordering when multiple reporters/listeners are used
REGISTER_REPORTER("my_xml", 1, MyXmlReporter);
```

Multiple reporters can be used at the same time - just specify them through the ```--reporters=...``` [**command line filtering option**](commandline.md) using commas to separate them like this: ```--reporters=myListener,xml``` and their order of execution will be based on their priority - that is the number "1" in the case of the example reporter above (lower means earlier - the default console/xml reporters from the framework have 0 as their priority and negative numbers are accepted as well).

When implementing a reporter users are advised to follow the comments from the example above and look at the few implemented reporters in the framework itself. Also check out the [**example**](../../examples/all_features/reporters_and_listeners.cpp).

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
