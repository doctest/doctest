#include "in.h"
#include <sstream>

std::ostream* createStream() { return new std::stringstream(); }
std::string getStreamResult(std::ostream* in) { return static_cast<std::stringstream*>(in)->str().c_str(); }
void freeStream(std::ostream* in) { delete in; }

int main() {
    void f();
    f();
    return 0;
}
