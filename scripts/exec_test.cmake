# Arguments:
# - COMMAND: string - the command to run with all it's arguments
# - TEST_OUTPUT_FILE: string - if not empty - capture the output of the command and dump it to this file
#
# To run something through this script use cmake like this:
# cmake -DCOMMAND=path/to/my.exe -arg1 -arg2 -DTEST_OUTPUT_FILE=path/to/output.txt -P path/to/exec_test.cmake

string(REPLACE " " ";" COMMAND ${COMMAND})

set(cmd COMMAND ${COMMAND} RESULT_VARIABLE CMD_RESULT)
if(NOT "${TEST_OUTPUT_FILE}" STREQUAL "")
    list(APPEND cmd OUTPUT_FILE ${TEST_OUTPUT_FILE} ERROR_FILE ${TEST_OUTPUT_FILE})
endif()

execute_process(${cmd})

if(CMD_RESULT)
    message(FATAL_ERROR "Running '${COMMAND}' ended with code '${CMD_RESULT}'")
endif()
