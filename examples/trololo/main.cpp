#include "in.h"

#include <sstream>
#include <cstdlib>
#include <cstring>

String::String(const char* in)
        : data(0) {
    if(in && strlen(in) > 0) {
        data = (char*)malloc(strlen(in) + 1);
        strcpy(data, in);
    }
}
String::String(const String& other)
        : data(0) {
    if(other.data && strlen(other.data) > 0) {
        data = (char*)malloc(strlen(other.data) + 1);
        strcpy(data, other.data);
    }
}
String& String::operator=(const String& other) {
    if(data)
        free(data);
    data = 0;
    if(other.data && strlen(other.data) > 0) {
        data = (char*)malloc(strlen(other.data) + 1);
        strcpy(data, other.data);
    }
    return *this;
}
String::~String() {
    if(data)
        free(data);
}

std::ostream* createStream() { return new std::stringstream(); }
String getStreamResult(std::ostream* in) {
    return static_cast<std::stringstream*>(in)->str().c_str();
}
void freeStream(std::ostream* in) { delete in; }

int main() {
    void f();
    f();

    system("pause");

    return 0;
}
