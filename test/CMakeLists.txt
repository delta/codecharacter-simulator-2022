add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PRIVATE CONAN_PKG::catch2)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE catch_main)

install(TARGETS tests DESTINATION bin)
