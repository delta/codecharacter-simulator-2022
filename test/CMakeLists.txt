add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PRIVATE CONAN_PKG::catch2)

file(GLOB_RECURSE TEST_FILES *.cpp)
list(REMOVE_ITEM TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/catch_main.cpp)

add_executable(tests ${TEST_FILES})
target_link_libraries(tests PRIVATE catch_main)
target_link_libraries(tests PRIVATE cc_simulator)

install(TARGETS tests DESTINATION bin)
