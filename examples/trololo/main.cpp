#include "in.h"
#include <sstream>
#include <cstdlib>
#include <cstring>


String::String(const char* in) {
    m_str = static_cast<char*>(malloc(strlen(in) + 1));
    strcpy(m_str, in);
}

String::String(const String& other)
    : m_str(0) {
    copy(other);
}

void String::copy(const String& other) {
    if (m_str)
        free(m_str);
    m_str = 0;

    if (other.m_str) {
        m_str = static_cast<char*>(malloc(strlen(other.m_str) + 1));
        strcpy(m_str, other.m_str);
    }
}

String::~String() {
    if (m_str)
        free(m_str);
}

String& String::operator=(const String& other) {
    if (this != &other)
        copy(other);
    return *this;
}

String String::operator+(const String& other) const { return String(m_str) += other; }

String& String::operator+=(const String& other) {
    if (m_str == 0) {
        copy(other);
    }
    else if (other.m_str != 0) {
        char* newStr = static_cast<char*>(malloc(strlen(m_str) + strlen(other.m_str) + 1));
        strcpy(newStr, m_str);
        strcpy(newStr + strlen(m_str), other.m_str);
        free(m_str);
        m_str = newStr;
    }
    return *this;
}

std::ostream* createStream() { return new std::ostringstream(); }
String getStreamResult(std::ostream* in) { return static_cast<std::ostringstream*>(in)->str().c_str(); }
void freeStream(std::ostream* in) { delete in; }

int main() {
    void f();
    f();

    //std::cout << "omg";

    system("pause");

    return 0;
}
