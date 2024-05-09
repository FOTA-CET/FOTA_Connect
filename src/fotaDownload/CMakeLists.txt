cmake_minimum_required(VERSION 3.10)
add_library(fotaDownload STATIC fotaDownload.cpp)

target_include_directories(fotaDownload PUBLIC "./")

# External librarries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

target_link_libraries(fotaDownload PUBLIC cloudUrl jsonKey restAdapter ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})