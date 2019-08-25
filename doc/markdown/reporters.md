## Reporters

Doctest has a modular reporter/listener system with which users can write their own reporters and register them. The reporter interface can also be used for "listening" to events.

You can list all registered reporters/listeners with ```--list-reporters```. There are a few implemented reporters in the framework:
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
            : stdout_stream(*in.cout)
            , opt(in) {}

    void report_query(const QueryData& /*in*/) override {}

    void test_run_start() override {}

    void test_run_end(const TestRunStats& /*in*/) override {}

    void test_case_start(const TestCaseData& in) override { tc = &in; }

    // called when a test case is reentered because of unfinished subcases
    void test_case_reenter(const TestCaseData& /*in*/) override {}

    void test_case_end(const CurrentTestCaseStats& /*in*/) override {}

    void test_case_exception(const TestCaseException& /*in*/) override {}

    void subcase_start(const SubcaseSignature& /*in*/) override {
        std::lock_guard<std::mutex> lock(mutex);
    }

    void subcase_end() override {
        std::lock_guard<std::mutex> lock(mutex);
    }

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

// "1" is the priority - used for ordering when multiple reporters are used
REGISTER_REPORTER("my_xml", 1, MyXmlReporter);

// registering the same class as a reporter and as a listener is nonsense but it's possible
REGISTER_LISTENER("my_listener", 1, MyXmlReporter);
```

Custom `IReporter` implementations must be registered with one of:

* `REGISTER_REPORTER`, for when the new reporter is an option that users may choose at run-time.
* `REGISTER_LISTENER`, for when the reporter is actually a listener and must always be executed, regardless of which reporters have been chosen at run-time.

Multiple reporters can be used at the same time - just specify them through the ```--reporters=...``` [**command line filtering option**](commandline.md) using commas to separate them like this: ```--reporters=myReporter,xml``` and their order of execution will be based on their priority - that is the number "1" in the case of the example reporter above (lower means earlier - the default console/xml reporters from the framework have 0 as their priority and negative numbers are accepted as well).

All registered listeners (```REGISTER_LISTENER```) will be executed before any reporter - they do not need to be specified and cannot be filtered through the command line.

When implementing a reporter users are advised to follow the comments from the example above and look at the few implemented reporters in the framework itself. Also check out the [**example**](../../examples/all_features/reporters_and_listeners.cpp).

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
