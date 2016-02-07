#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <cstdlib>
#include <exception>
using namespace std;

testsuite(MAIN);
test(zzz)
{
    cout << "main!" << endl;
}
testsuite_end;

//test(thrower)
//{
//    if(rand() > 4) {
//        throw std::exception();
//    } else {
//        cout << "trololo" << endl;
//    }
//}

static int testWrapper(void (*f)(void))
{
    try {
        f();
    } catch(std::exception& e) {
        cout << "caught the bugger! " << e.what() << endl;
        return 1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    // initialize
    void* params = doctest::createParams(argc, argv);
    doctest::setTestExecutionWrapper(params, testWrapper);

    // overrides
    doctest::setOption(params, "doctest_case_sensitive", true);
    //doctest::addFilter(params, "doctest_name", "zzz");

    // run
    int res = doctest::runTests(params);

    // free
    doctest::freeParams(params);

#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}

test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
test("") { cout << "TEST " << __LINE__ << endl; }
