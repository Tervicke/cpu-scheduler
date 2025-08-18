cmake_minimum_required(VERSION 3.31)
project(cpu_Scheduling)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpu_Scheduling main.cpp)

# Tell this target to look in ./include for headers
target_include_directories(cpu_Scheduling PRIVATE ${CMAKE_SOURCE_DIR}/include)
