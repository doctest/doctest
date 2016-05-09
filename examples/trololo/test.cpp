#include <vector>
#include <ostream>

struct myType
{ int a; };

std::ostream& operator<<(std::ostream& stream, const myType&) {
    stream << "dfgsdgfgfdsg";
    return stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& in) {
    stream << "dfgsdgfgfdsg";
    return stream;
}

#include "in.h"


#include <iostream>

void f() {
    myType a{5};           std::cout << stringify(a) << std::endl;
                           std::cout << stringify(6) << std::endl;
    std::vector<int> v(5); std::cout << stringify(v) << std::endl;
}
