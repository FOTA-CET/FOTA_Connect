cmake_minimum_required(VERSION 3.10)
add_library(fotaConnectApp STATIC fota_connect_app.cpp)

target_include_directories(fotaConnectApp PUBLIC "./")

# External librarries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

target_link_libraries(fotaConnectApp PUBLIC fotaDownload ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})