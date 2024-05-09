cmake_minimum_required(VERSION 3.10)
add_library(jsonKey STATIC jsonKey.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

target_link_libraries(jsonKey PUBLIC ${JSONCPP_LIBRARIES})
target_include_directories(jsonKey PUBLIC "./")