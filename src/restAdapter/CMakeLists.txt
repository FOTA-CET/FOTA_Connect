cmake_minimum_required(VERSION 3.10)
add_library(restAdapter STATIC restAdapter.cpp)

target_include_directories(restAdapter PUBLIC "./")

# External librarries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

target_link_libraries(restAdapter PUBLIC jsonKey ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})