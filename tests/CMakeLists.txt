cmake_minimum_required(VERSION 3.20)

project(unit_tests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unit_tests unit_tests.cpp)

target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include/)