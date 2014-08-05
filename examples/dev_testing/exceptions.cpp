// open a cmd and execute this: "C:\Program Files (x86)\Microsoft Visual Studio 11.0\vc\vcvarsall.bat" amd64
// compile with:
//  cl.exe a.cpp /EHs
//  cl.exe a.cpp /EHa
 
#ifdef _MSC_VER
//#include <windows.h>
#endif
 
#include <iostream>
using namespace std;
 
void f(int argc, char** argv) {
    try {
        *((char*)666) = 1;
    } catch(...) {
        cout << "bad boy!" << endl;
    }
   
    try {
        int out = 5 / (argc - 1);
        cout << out;
    } catch(...) {
        cout << "bad boy!" << endl;
    }
}
 
int main(int argc, char** argv) {
 
#ifdef _MSC_VER
    __try {
#endif
 
    f(argc, argv);
 
#ifdef _MSC_VER
    //} __except(EXCEPTION_EXECUTE_HANDLER) {
    } __except(1) {
        cout << "bad MS boy!" << endl;
    }
#endif
 
    return 0;
}