cmake_minimum_required(VERSION 3.5)

project(SmartSeparated)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCES "src/roundedbutton.cpp" "src/ofsmartseparate.cpp")
file(GLOB TEST test/*)

add_executable(unit_tests ${SOURCES} ${TEST})
