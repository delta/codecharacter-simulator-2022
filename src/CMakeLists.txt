add_library(cc_simulator)
target_include_directories(cc_simulator PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(utils)
add_subdirectory(actor)
add_subdirectory(attacker)
add_subdirectory(defender)
add_subdirectory(game)
add_subdirectory(logger)

add_executable(main main.cpp)
target_link_libraries(main cc_simulator)
