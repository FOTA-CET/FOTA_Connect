cmake_minimum_required(VERSION 3.10)
add_library(cloudUrl STATIC cloudUrl.cpp)
target_include_directories(cloudUrl PUBLIC "./")

# External librarries
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

target_link_libraries(cloudUrl PUBLIC jsonKey ${JSONCPP_LIBRARIES})