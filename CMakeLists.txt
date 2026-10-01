cmake_minimum_required(VERSION 3.21)

set(_COMMONLIB_SUBMODULE "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE")
if(NOT EXISTS "${_COMMONLIB_SUBMODULE}/CMakeLists.txt")
  message(FATAL_ERROR
    "CommonLibSSE not found. Initialize the pinned submodule:\n"
    "  git submodule update --init --recursive extern/CommonLibSSE")
endif()

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)
include(cmake/lib/copyOutputs.cmake)
include(cmake/lib/automaticGameFolderOutput.cmake)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Set your project name. This will be the name of your SKSE .dll file.
set(AUTHOR_NAME "Quantumyilmaz")
set(PRODUCT_NAME "PreSSEDisplayTweaks")
project(${PRODUCT_NAME} VERSION 1.2.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)

set(RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
set(RESOURCE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/version.res)

add_custom_command(
    OUTPUT ${RESOURCE_OUTPUT}
    COMMAND rc /fo ${RESOURCE_OUTPUT} ${RESOURCE_FILE}
    DEPENDS ${RESOURCE_FILE}
)

# Add a custom target to ensure the resource file is generated
add_custom_target(Resource ALL DEPENDS ${RESOURCE_OUTPUT})

set(ENABLE_SKYRIM_SE ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL " " FORCE)
set(ENABLE_SKYRIM_VR ON CACHE BOOL " " FORCE)
set(BUILD_TESTS OFF CACHE BOOL " " FORCE)

add_subdirectory("${_COMMONLIB_SUBMODULE}" ${CMAKE_CURRENT_BINARY_DIR}/_deps/clib-build)
get_target_property(COMMONLIB_SRC_DIR CommonLibSSE SOURCE_DIR)

file(
  GLOB_RECURSE
  sources
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  ${sources}
)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${headers} ${sources}
)

set(BUILD_TESTS OFF)
include(${COMMONLIB_SRC_DIR}/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(
  ${PROJECT_NAME}
  AUTHOR "${AUTHOR_NAME}"
  SOURCES ${headers} ${sources}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23) # <--- use C++23 standard
target_precompile_headers(${PROJECT_NAME} PRIVATE include/PCH.h) # <--- PCH.h is required!
target_include_directories(
	${PROJECT_NAME}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME} PRIVATE IS_HOST_PLUGIN)

set(wildlander_output false)
set(steam_owrt_output false)
set(steam_mods_output true)
automaticGameFolderOutput(wildlander_output, steam_owrt_output, steam_mods_output)

find_package(Git REQUIRED)
execute_process(
  COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  OUTPUT_VARIABLE SOURCE_REVISION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND_ERROR_IS_FATAL ANY
)
configure_file(cmake/source.txt.in SOURCE.txt @ONLY)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION SKSE/Plugins COMPONENT AutoResolution)
install(FILES LICENSE NOTICE EXCEPTIONS.md "${CMAKE_CURRENT_BINARY_DIR}/SOURCE.txt"
  DESTINATION licenses/AutoResolution COMPONENT AutoResolution)
install(FILES licenses/LICENSE-MIT.txt
  DESTINATION licenses/AutoResolution COMPONENT AutoResolution)
install(FILES "${COMMONLIB_SRC_DIR}/COPYING.txt" "${COMMONLIB_SRC_DIR}/EXCEPTIONS.md"
  DESTINATION licenses/AutoResolution/CommonLibSSE-NG COMPONENT AutoResolution)
install(DIRECTORY "${COMMONLIB_SRC_DIR}/licenses/"
  DESTINATION licenses/AutoResolution/CommonLibSSE-NG COMPONENT AutoResolution
  FILES_MATCHING PATTERN "LICENSE-*.txt")
install(FILES "${COMMONLIB_SRC_DIR}/extern/openvr/LICENSE"
  DESTINATION licenses/AutoResolution/OpenVR COMPONENT AutoResolution)
foreach(dependency directxmath directxtk fmt rapidcsv simpleini spdlog)
  install(FILES "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/${dependency}/copyright"
    DESTINATION "licenses/AutoResolution/${dependency}" COMPONENT AutoResolution)
endforeach()
