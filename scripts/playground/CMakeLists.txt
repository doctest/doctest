add_executable(playground main.cpp test.cpp)
target_link_libraries(playground doctest)

doctest_add_test(NAME playground NO_OUTPUT COMMAND $<TARGET_FILE:playground> -nv)

#add_custom_command(TARGET playground POST_BUILD COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
#add_custom_command(TARGET playground POST_BUILD COMMAND ctest -C $<CONFIGURATION> --output-on-failure)
