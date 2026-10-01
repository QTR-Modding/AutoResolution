if(DEFINED SKYRIM_AUTORESOLUTION_DEPENDENCIES_INCLUDED)
  return()
endif()
set(SKYRIM_AUTORESOLUTION_DEPENDENCIES_INCLUDED TRUE)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(COMMONLIB_SSE_FOLDER "${PROJECT_ROOT}/external/alandtse/CommonLibSSE-NG")
set(VCPKG_ROOT "${PROJECT_ROOT}/external/microsoft/vcpkg")
set(COMMONLIB_SSE_MARKER "${COMMONLIB_SSE_FOLDER}/extern/openvr/lib/win64/openvr_api.lib")
set(COMMONLIB_SSE_REPOSITORY "https://github.com/alandtse/CommonLibSSE-NG.git")
set(COMMONLIB_SSE_COMMIT "39f9d07a6ffabea8fb559eee87ab7d27cd463e8a")
set(VCPKG_REPOSITORY "https://github.com/microsoft/vcpkg.git")
set(VCPKG_COMMIT "eb2d3a3279fd019cb7733072d86900d0ad2a1aef")

find_program(GIT_EXECUTABLE git REQUIRED)

function(clone_dependency_if_missing DEPENDENCY_PATH DEPENDENCY_URL DEPENDENCY_COMMIT DEPENDENCY_MARKER)
  if(EXISTS "${DEPENDENCY_PATH}")
    if(EXISTS "${DEPENDENCY_PATH}/.git")
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" -C "${DEPENDENCY_PATH}" rev-parse HEAD
        RESULT_VARIABLE REV_PARSE_RESULT
        OUTPUT_VARIABLE CURRENT_COMMIT
        ERROR_VARIABLE REV_PARSE_ERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )

      if(REV_PARSE_RESULT EQUAL 0 AND CURRENT_COMMIT STREQUAL DEPENDENCY_COMMIT)
        execute_process(
          COMMAND "${GIT_EXECUTABLE}" -C "${DEPENDENCY_PATH}" submodule update --init --recursive
          RESULT_VARIABLE SUBMODULE_RESULT
          OUTPUT_VARIABLE SUBMODULE_OUTPUT
          ERROR_VARIABLE SUBMODULE_ERROR
        )

        if(SUBMODULE_RESULT EQUAL 0 AND EXISTS "${DEPENDENCY_MARKER}")
          return()
        endif()

        message(FATAL_ERROR
          "Pinned dependency submodules are incomplete: ${DEPENDENCY_PATH}\n"
          "${SUBMODULE_OUTPUT}${SUBMODULE_ERROR}")
      endif()

      message(FATAL_ERROR
        "Dependency directory is not at the required pinned commit: ${DEPENDENCY_PATH}\n"
        "Expected: ${DEPENDENCY_COMMIT}\n"
        "Found: ${CURRENT_COMMIT}\n"
        "Remove it and re-run CMake so it can be cloned again.\n"
        "${REV_PARSE_ERROR}")
    endif()

    message(FATAL_ERROR
      "Dependency directory exists but is incomplete: ${DEPENDENCY_PATH}\n"
      "Remove it and re-run CMake so it can be cloned again.")
  endif()

  get_filename_component(DEPENDENCY_PARENT "${DEPENDENCY_PATH}" DIRECTORY)
  file(MAKE_DIRECTORY "${DEPENDENCY_PARENT}")
  message(STATUS "Cloning ${DEPENDENCY_URL} at ${DEPENDENCY_COMMIT} into ${DEPENDENCY_PATH}")

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" clone --filter=blob:none --no-checkout "${DEPENDENCY_URL}" "${DEPENDENCY_PATH}"
    RESULT_VARIABLE CLONE_RESULT
    OUTPUT_VARIABLE CLONE_OUTPUT
    ERROR_VARIABLE CLONE_ERROR
  )

  if(NOT CLONE_RESULT EQUAL 0)
    message(FATAL_ERROR
      "Failed to clone ${DEPENDENCY_URL} into ${DEPENDENCY_PATH}.\n"
      "${CLONE_OUTPUT}${CLONE_ERROR}")
  endif()

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" -C "${DEPENDENCY_PATH}" checkout --detach "${DEPENDENCY_COMMIT}"
    RESULT_VARIABLE CHECKOUT_RESULT
    OUTPUT_VARIABLE CHECKOUT_OUTPUT
    ERROR_VARIABLE CHECKOUT_ERROR
  )

  if(NOT CHECKOUT_RESULT EQUAL 0)
    message(FATAL_ERROR
      "Failed to check out pinned commit ${DEPENDENCY_COMMIT} for ${DEPENDENCY_URL}.\n"
      "${CHECKOUT_OUTPUT}${CHECKOUT_ERROR}")
  endif()

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" -C "${DEPENDENCY_PATH}" submodule update --init --recursive
    RESULT_VARIABLE SUBMODULE_RESULT
    OUTPUT_VARIABLE SUBMODULE_OUTPUT
    ERROR_VARIABLE SUBMODULE_ERROR
  )

  if(NOT SUBMODULE_RESULT EQUAL 0 OR NOT EXISTS "${DEPENDENCY_MARKER}")
    message(FATAL_ERROR
      "Failed to initialize the pinned dependency ${DEPENDENCY_URL}.\n"
      "${SUBMODULE_OUTPUT}${SUBMODULE_ERROR}")
  endif()
endfunction()

clone_dependency_if_missing(
  "${COMMONLIB_SSE_FOLDER}"
  "${COMMONLIB_SSE_REPOSITORY}"
  "${COMMONLIB_SSE_COMMIT}"
  "${COMMONLIB_SSE_MARKER}"
)
clone_dependency_if_missing(
  "${VCPKG_ROOT}"
  "${VCPKG_REPOSITORY}"
  "${VCPKG_COMMIT}"
  "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
)

if(CMAKE_HOST_WIN32 AND NOT EXISTS "${VCPKG_ROOT}/vcpkg.exe")
  execute_process(
    COMMAND "${VCPKG_ROOT}/bootstrap-vcpkg.bat" -disableMetrics
    WORKING_DIRECTORY "${VCPKG_ROOT}"
    RESULT_VARIABLE VCPKG_BOOTSTRAP_RESULT
    OUTPUT_VARIABLE VCPKG_BOOTSTRAP_OUTPUT
    ERROR_VARIABLE VCPKG_BOOTSTRAP_ERROR
  )

  if(NOT VCPKG_BOOTSTRAP_RESULT EQUAL 0 OR NOT EXISTS "${VCPKG_ROOT}/vcpkg.exe")
    message(FATAL_ERROR
      "Failed to bootstrap vcpkg in ${VCPKG_ROOT}.\n"
      "${VCPKG_BOOTSTRAP_OUTPUT}${VCPKG_BOOTSTRAP_ERROR}")
  endif()
endif()
