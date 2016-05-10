#include "in.h"
#include <sstream>

std::ostream* createStream() { return new std::ostringstream(); }
std::string getStreamResult(std::ostream* in) { return static_cast<std::ostringstream*>(in)->str().c_str(); }
void freeStream(std::ostream* in) { delete in; }

int main() {
    void f();
    f();

    //std::cout << "omg";

    //system("pause");

    return 0;
}
