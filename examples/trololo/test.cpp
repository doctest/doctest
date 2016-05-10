#include "in.h"

#include <vector>
#include <iostream>



struct myType {};
struct myType2 {};
std::ostream& operator<<(std::ostream& s, const myType&) { s << "myType"; return s; }




template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& in) { s << "vector<T>"; return s; }

void f() {
    myType a;              std::cout << stringify(a) << std::endl;
                           std::cout << stringify(6) << std::endl;
    std::vector<int> v(5); std::cout << stringify(v) << std::endl;


    //std::cout << myType2();
}
