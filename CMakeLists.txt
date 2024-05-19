cmake_minimum_required(VERSION 3.10)

project(TEST)

add_subdirectory(src/cloudUrl)
add_subdirectory(src/restAdapter)
add_subdirectory(src/jsonKey)
add_subdirectory(src/fotaDownload)
add_subdirectory(src/fota_connect_app)

# External librarries
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})

add_executable(fotaConnect src/main.cpp)

target_link_libraries(fotaConnect fotaConnectApp fotaDownload cloudUrl jsonKey restAdapter ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})