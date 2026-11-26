cmake_minimum_required(VERSION 3.13)
project(glitch_test VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(case case.cpp)
add_executable(scramble scramble.cpp)
add_executable(rev-text rev.cpp)

include(GNUInstallDirs)
install(TARGETS case scramble rev-text
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

configure_file(cmake/uninstall.cmake.in cmake_uninstall.cmake @ONLY)
add_custom_target(uninstall
    COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    VERBATIM
)
