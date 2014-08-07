@mkdir mingw
@cd mingw
@cmake -G "MinGW Makefiles" ../
@cd ..

@mkdir msvc
@cd msvc
@cmake -G "Visual Studio 12 Win64" ../
@cd ..