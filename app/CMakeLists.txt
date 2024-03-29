add_executable(Basics basics.cpp)
target_link_libraries(Basics PRIVATE MatrixLib)
add_executable(Basics2 basics2.cpp)
target_include_directories(Basics2 PRIVATE ${PROJECT_SOURCE_DIR}/include)