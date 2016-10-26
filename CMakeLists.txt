cmake_minimum_required(VERSION 3.6)
project(TwitterClient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES examples/main.cpp include/twitter/client.h sources/client.cpp tests/check.cpp)
add_executable(TwitterClient ${SOURCE_FILES})
