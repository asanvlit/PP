cmake_minimum_required(VERSION 3.19)
project(mpi)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)
#add_executable(mpi task/block3/block3Tasks.cpp)
#add_executable(mpi task/block4/block4Tasks.cpp)
add_executable(mpi task/block4/block4Tasks.cpp)
target_link_libraries(mpi PUBLIC MPI::MPI_CXX)

