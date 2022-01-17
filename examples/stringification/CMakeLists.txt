# separated executable to ensure there is no specializations of StringMaker leaked
# In opposite to all_features/stringification.cpp this test intended to check all the bundled
# output rules for all the fundamental types, std containers etc
# it might overlap with other tests, but better to test twice, than miss some cases
add_executable(stringification main.cpp)
target_link_libraries(stringification doctest)
target_compile_definitions(stringification PRIVATE SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P})
if(MSVC)
    # See https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    target_compile_options(stringification PUBLIC "/Zc:__cplusplus")
endif()

# omit the version and the num test cases skipped from the summary - this way the output will change less often
set(common_args COMMAND $<TARGET_FILE:stringification> --no-skipped-summary --no-version)

doctest_add_test(NAME stringification_all COMMAND ${common_args})

# Unfortunately tests could and actually do affect each other, so rerun each test case separately
doctest_detect_test_cases(main.cpp stringification_test_cases)
foreach(test_case IN LISTS stringification_test_cases)
    string(MAKE_C_IDENTIFIER "${test_case}" test_case_identifier)
    string(TOLOWER "${test_case_identifier}" test_case_identifier)
    doctest_add_test(NAME "stringification_${test_case_identifier}" COMMAND ${common_args} --test-case=${test_case})
endforeach()

if(MSVC)
    target_compile_options(stringification PRIVATE /wd4505) # unreferenced local function has been removed
    target_compile_options(stringification PRIVATE /wd4100) # unreferenced formal parameter
    target_compile_options(stringification PRIVATE /wd4189) # local variable is initialized but not referenced
    target_compile_options(stringification PRIVATE /wd4312) # 'reinterpret_cast': conversion from 'unsigned int' to 'int *' of greater size
    target_compile_options(stringification PRIVATE /wd5245) # '`anonymous-namespace'::operator <<': unreferenced function with internal linkage has been removed
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(stringification PRIVATE -Wno-unknown-warning-option)
    target_compile_options(stringification PRIVATE -Wno-unneeded-internal-declaration)
    target_compile_options(stringification PRIVATE -Wno-unused-function)
    target_compile_options(stringification PRIVATE -Wno-unused-parameter)
    target_compile_options(stringification PRIVATE -Wno-unused-variable)
    target_compile_options(stringification PRIVATE -Wno-unused-template)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(stringification PRIVATE -Wno-unused-function)
    target_compile_options(stringification PRIVATE -Wno-unused-parameter)
    target_compile_options(stringification PRIVATE -Wno-unused-variable)
    target_compile_options(stringification PRIVATE -Wno-switch-default)
    target_compile_options(stringification PRIVATE -Wno-useless-cast) # static_casts used to explicitly set used type
endif()
