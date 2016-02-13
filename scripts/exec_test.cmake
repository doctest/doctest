# Arguments:
# - COMMAND: the command to run with all it's arguments
# - TEST_MODE: NORMAL/VALGRIND/COLLECT/COMPARE
# - TEST_OUTPUT_FILE: the file to/from which to write/read the output of the test
# - TEST_TEMP_FILE: the temp file for the current test output used in COMPARE mode 
# To run something through this script use cmake like this:
# cmake -DCOMMAND=path/to/my.exe -arg1 -arg2 -DTEST_MODE=VALGRIND -P path/to/exec_test.cmake

#message("COMMAND: ${COMMAND}")
#message("TEST_MODE: ${TEST_MODE}")
#message("TEST_OUTPUT_FILE: ${TEST_OUTPUT_FILE}")
#message("TEST_TEMP_FILE: ${TEST_TEMP_FILE}")

string(REPLACE " " ";" COMMAND ${COMMAND})

set(cmd COMMAND ${COMMAND} RESULT_VARIABLE CMD_RESULT)
if("${TEST_MODE}" STREQUAL "COLLECT")
    list(APPEND cmd OUTPUT_FILE ${TEST_OUTPUT_FILE} ERROR_FILE ${TEST_OUTPUT_FILE})
elseif("${TEST_MODE}" STREQUAL "COMPARE")
    list(APPEND cmd OUTPUT_FILE ${TEST_TEMP_FILE} ERROR_FILE ${TEST_TEMP_FILE})
endif()

execute_process(${cmd})

if("${TEST_MODE}" STREQUAL "COMPARE")
    execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_OUTPUT_FILE} ${TEST_TEMP_FILE} RESULT_VARIABLE cmp_result)
    
    if(cmp_result)
        set(CMD_RESULT "Output is different from reference file ${TEST_OUTPUT_FILE}")
    endif()
endif()

if(CMD_RESULT)
    message(FATAL_ERROR "Running '${COMMAND}' ended with code '${CMD_RESULT}'")
endif()
