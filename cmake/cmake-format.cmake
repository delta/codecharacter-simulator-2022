find_program(
  CMAKE_FORMAT_EXE
  NAMES "cmake-format"
  DOC "Path to cmake-format executable")

if(NOT CMAKE_FORMAT_EXE)
  message(SEND_ERROR "cmake-format executable not found")
else()
  message("** Found cmake-format")
  message("** cmake-format executable location: ${CMAKE_FORMAT_EXE}")
endif()
file(GLOB_RECURSE ALL_CMAKE_FILES *.cmake *CMakeLists.txt)

list(
  FILTER
  ALL_CMAKE_FILES
  EXCLUDE
  REGEX
  ${CMAKE_SOURCE_DIR}/build*)
list(
  FILTER
  ALL_CMAKE_FILES
  EXCLUDE
  REGEX
  ${CMAKE_SOURCE_DIR}/ext*)
list(
  FILTER
  ALL_CMAKE_FILES
  EXCLUDE
  REGEX
  ${CMAKE_INSTALL_PREFIX}/*)

add_custom_target(cmakeformat ALL COMMAND ${CMAKE_FORMAT_EXE} -i
                                          ${ALL_CMAKE_FILES})
