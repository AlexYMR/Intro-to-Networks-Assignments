cmake_minimum_required(VERSION 3.1)
project(serializer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2")

set(SOURCE_FILES 
  src/main.cpp
  src/bakery.cpp
)

add_executable(main ${SOURCE_FILES})
target_include_directories(main PRIVATE include)
