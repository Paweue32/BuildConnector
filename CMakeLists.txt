cmake_minimum_required(VERSION 3.12)
project(BuildConnector)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(buildcon
        main.cpp
        parsing.cpp
        variables.cpp
        compile.cpp
)