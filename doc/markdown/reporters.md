## Reporters

A very sloppy documentation of the partial reporters support.

Example how to define your own reporter:

```c++
using namespace doctest;

struct MyXmlReporter : public IReporter
{
    std::ostream&                 stdout_stream;
    std::vector<SubcaseSignature> subcasesStack;

    // caching pointers/references to objects of these types - safe to do
    const ContextOptions& opt;
    const TestCaseData*   tc;

    // constructor has to accept the ContextOptions as a single argument
    XmlReporter(const ContextOptions& in)
                : stdout_stream(*in.stdout_stream)
                , opt(in) {}

    void test_run_start() override {}

    void test_run_end(const TestRunStats& /*in*/) override {}

    void test_case_start(const TestCaseData& in) override { tc = &in; }

    void test_case_end(const CurrentTestCaseStats& /*in*/) override {}

    void test_case_exception(const TestCaseException& /*in*/) override {}

    void subcase_start(const SubcaseSignature& in) override { subcasesStack.push_back(in); }

    void subcase_end(const SubcaseSignature& /*in*/) override { subcasesStack.pop_back(); }

    void log_assert(const AssertData& /*in*/) override {}

    void log_message(const MessageData& /*in*/) override {}

    void test_case_skipped(const TestCaseData& /*in*/) override {}
};

REGISTER_REPORTER("xml", 1, MyXmlReporter); // priority 1 - a number user for ordering
```

Multiple reporters can be used at the same time - just specify them through the ```--reporters=...``` [**command line option**](commandline.md). The number ```1``` in this case is the priority - reporters will be called in the order defined by their priority when a few of them are selected to be used at the same time.

You can list all registered reporters with ```--list-reporters```. There is only 1 implemented reporter in the framework - a console reporter - an xml one is coming in the next version. the reporter interface can also be used for "listening" to events.

Reporters will be fully implemented and more thoroughly documented (with examples) for version 2.1 - [**roadmap**](roadmap.md).

When implementing a reporter users are advised to use a mutex in the ```log_assert``` and ```log_message``` functions.

Look at the reporters implemented in the doctest header for reference.

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
