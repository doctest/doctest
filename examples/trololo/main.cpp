#include "in.h"

#include <sstream>

std::ostream* createStream() { return new std::stringstream(); }
std::string getStreamResult(std::ostream* in) { return static_cast<std::stringstream*>(in)->str(); }
void freeStream(std::ostream* in) { delete in; }

void defaultPrint(std::ostream& stream) { stream << std::string("{?}"); }

int main() {
    void f();
    f();

    system("pause");

    return 0;
}
