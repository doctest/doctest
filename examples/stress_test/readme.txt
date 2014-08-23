machine:
    intel i7 3770k
    16g ram
    ssd
OS:
    windows 7 x64
    ubuntu 14.04
MinGW:
    gcc (x86_64-posix-seh-rev1, Built by MinGW-W64 project) 4.9.0
MSVC:
    
Linux GCC:
    gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2

10k doctests in each cpp

gcc (mingw/linux): "-std=c++98 -m64"

debug build:
    mingw:  121 sec  (~100 sec link time)
    msvc:   11 sec

release build:
    mingw:  148 sec
    msvc:   21 sec
    
    
    
    
    
    
    
    
    33:40