cmake_minimum_required(VERSION 3.19)
project(PP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

add_executable(PP_block_1_task_1 task/block1/task1.cpp)
add_executable(PP_block_1_task_2 task/block1/task2.cpp)
add_executable(PP_block_1_task_3 task/block1/task3.cpp)
add_executable(PP_block_1_task_4 task/block1/task4.cpp)
add_executable(PP_block_1_task_5 task/block1/task5.cpp)