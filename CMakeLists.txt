cmake_minimum_required(VERSION 3.1...3.29)
project(
    MatrixImplementation
    DESCRIPTION "Implementation of different algorithms of Matrix Multiplication"
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(app)
# add_subdirectory(tests)
