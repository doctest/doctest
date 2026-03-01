execute_process(
  COMMAND "${TEST_PROGRAM}" --reporters=fatal_trace
  OUTPUT_FILE "${TEST_TEMP_FILE}"
  ERROR_FILE "${TEST_TEMP_FILE}"
  RESULT_VARIABLE result
)

if(NOT CMAKE_HOST_UNIX)
  execute_process(COMMAND dos2unix "${TEST_TEMP_FILE}")
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.14.0")
  set(IGNORE_EOL --ignore-eol)
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E compare_files ${IGNORE_EOL} "${TEST_OUTPUT_FILE}" "${TEST_TEMP_FILE}"
  RESULT_VARIABLE cmp_result
)

if(cmp_result)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} diff --no-index "${TEST_OUTPUT_FILE}" "${TEST_TEMP_FILE}"
      OUTPUT_VARIABLE DIFF
    )
    message("${DIFF}")
  endif()

  message(FATAL_ERROR "fatal_subcase_unwind output differed from reference; process result: ${result}")
endif()
