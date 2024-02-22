cmake_minimum_required(VERSION 3.14)

project(ISE)

set(CXX_STANDARD C++17)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC "src/**.cpp")
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC include/)