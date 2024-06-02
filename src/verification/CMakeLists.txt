cmake_minimum_required(VERSION 3.10)
add_library(verification STATIC verification.cpp)

target_include_directories(verification PUBLIC "./")

# External librarries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

find_package(Threads REQUIRED)

target_link_libraries(verification PUBLIC fotaDownload cloudUrl jsonKey restAdapter Threads::Threads ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})