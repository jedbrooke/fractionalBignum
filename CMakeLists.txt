cmake_minimum_required(VERSION 3.7)

project(fractionalBignum)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-O3")

add_library(
        fractionalBignum
        utility.hpp
        utility.cpp
        fractionalBignum.hpp
)

project(basic_usage)
add_executable(basic_usage examples/basic_usage.cpp)
target_link_libraries(basic_usage fractionalBignum)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)

project(speed_comparison)
add_executable(speed_comparison examples/speed_comparison.cpp)
target_link_libraries(speed_comparison fractionalBignum PkgConfig::GMPXX)

project(ln2)
add_executable(ln2 examples/ln2.cpp)
target_link_libraries(ln2 fractionalBignum PkgConfig::GMPXX)