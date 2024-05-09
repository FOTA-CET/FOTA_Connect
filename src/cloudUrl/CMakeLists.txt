cmake_minimum_required(VERSION 3.10)
add_library(cloudUrl STATIC cloudUrl.cpp)
target_include_directories(cloudUrl PUBLIC "./")