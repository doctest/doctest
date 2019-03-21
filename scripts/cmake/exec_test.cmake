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

string(REPLACE " " ";" COMMAND_LIST ${COMMAND})
set(cmd COMMAND ${COMMAND_LIST} RESULT_VARIABLE CMD_RESULT)
if("${TEST_MODE}" STREQUAL "COLLECT")
    list(APPEND cmd OUTPUT_FILE ${TEST_OUTPUT_FILE} ERROR_FILE ${TEST_OUTPUT_FILE})
elseif("${TEST_MODE}" STREQUAL "COMPARE")
    list(APPEND cmd OUTPUT_FILE ${TEST_TEMP_FILE} ERROR_FILE ${TEST_TEMP_FILE})
endif()

execute_process(${cmd})

# fix line endings
if("${TEST_MODE}" STREQUAL "COLLECT" AND NOT CMAKE_HOST_UNIX)
    execute_process(COMMAND dos2unix ${TEST_OUTPUT_FILE})
endif()

if("${TEST_MODE}" STREQUAL "COMPARE")
    # fix line endings
    if(NOT CMAKE_HOST_UNIX)
        execute_process(COMMAND dos2unix ${TEST_TEMP_FILE})
    endif()
    
    execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_OUTPUT_FILE} ${TEST_TEMP_FILE} RESULT_VARIABLE cmp_result)
    
    if(cmp_result)
        find_package(Git)
        if(GIT_FOUND)
            set(cmd ${GIT_EXECUTABLE} diff --no-index ${TEST_OUTPUT_FILE} ${TEST_TEMP_FILE})
            execute_process(COMMAND ${GIT_EXECUTABLE} diff --no-index ${TEST_OUTPUT_FILE} ${TEST_TEMP_FILE} OUTPUT_VARIABLE DIFF)
            MESSAGE("${DIFF}")
        endif()
        
        # file(READ ${TEST_OUTPUT_FILE} orig)
        # file(READ ${TEST_TEMP_FILE} temp)
        
        # message("==========================================================================")
        # message("== CONTENTS OF ${TEST_OUTPUT_FILE}")
        # message("==========================================================================")
        # message("${orig}")
        # message("==========================================================================")
        # message("== CONTENTS OF ${TEST_TEMP_FILE}")
        # message("==========================================================================")
        # message("${temp}")
        # message("==========================================================================")
        # message("== CONTENTS END")
        # message("==========================================================================")
        
        set(CMD_RESULT "Output is different from reference file!")
    endif()
endif()

if(CMD_RESULT)
    message(FATAL_ERROR "Running '${COMMAND}' ended with code '${CMD_RESULT}'")
endif()
