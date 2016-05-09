#include "in.h"

struct myType
{ int a; };

std::ostream& operator<<(std::ostream& stream, const myType&) {
    stream << "omgomgomg";
    return stream;
}

#include <iostream>

void f() {
    myType a{5};
    std::cout << stringify(a) << std::endl;
    //std::cout << stringify(int(6)) << std::endl;
}
