# Arguments:
# - COMMAND: string - the command to run with all it's arguments
# - TEST_OUTPUT_FILE: string - if not empty - capture the output of the command and dump it to this file
#
# To run something through this script use cmake like this:
# cmake -DCOMMAND="path/to/my.exe -arg1 -arg2" -DTEST_OUTPUT_FILE=path/to/output.txt -P path/to/exec_test.cmake

message("")
message("COMMAND: ${COMMAND}")
message("")
message("TEST_OUTPUT_FILE: ${TEST_OUTPUT_FILE}")
message("")

if(NOT "${TEST_OUTPUT_FILE}" STREQUAL "")
    execute_process(COMMAND "${COMMAND}" RESULT_VARIABLE CMD_RESULT OUTPUT_FILE ${TEST_OUTPUT_FILE} ERROR_FILE ${TEST_OUTPUT_FILE})
else()
    execute_process(COMMAND "${COMMAND}" RESULT_VARIABLE CMD_RESULT)
endif()

message(${CMD_RESULT})

if(NOT "${CMD_RESULT}" STREQUAL "")
    message(FATAL_ERROR "Running '${COMMAND}' ended with code '${CMD_RESULT}'")
endif()
