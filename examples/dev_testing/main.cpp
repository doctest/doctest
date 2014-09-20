#include "doctest.h"

#include <cstdio>

#include <vector>
using namespace std;

testsuite(MAIN)

test(offf) {
    printf("+_______+\n");
}

testsuite_end


int main(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);
    
    vector<int, doctestns::MallocAllocator<int> > vec;
    
    vec.push_back(5);
    
#if defined(_MSC_VER)
    system("pause");
#endif // _MSC_VER

    return 0;
}
