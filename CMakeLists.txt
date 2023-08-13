cmake_minimum_required(VERSION 3.20)

project(r2v LANGUAGES CXX)

enable_testing()

add_subdirectory(tests)

add_executable(r2v src/main.cpp)

target_compile_features(r2v PRIVATE cxx_std_20)
target_include_directories(r2v PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

add_test(NAME unit_tests COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/unit_tests)

add_custom_command(TARGET r2v POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r2v> ${CMAKE_CURRENT_LIST_DIR})