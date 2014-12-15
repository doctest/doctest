#include "doctest.h"

#include <iostream>
using namespace std;

testsuite(test);

test(ttt)
{
    cout << "test!" << endl;
}

struct F {
    F()
    {
        cout << "ctor!" << endl;
    }
    ~F()
    {
        cout << "dtor..." << endl;
    }
};

fixture_noname(F)
{
    cout << "    fixturing!" << endl;
}

testsuite_end;
