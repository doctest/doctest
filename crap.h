#pragma once
#include "doctest.h"

#include <iostream>
using namespace std;

struct Data {
    Data() {
        a = 5;
        cout << "initing fixture\n";
    }
    int a;
};

doctest_fixture(Data) {
    cout << "fixture\n";
}

doctest {
    cout << "alabala\n";
}

