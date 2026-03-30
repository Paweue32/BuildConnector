cmake_minimum_required(VERSION 3.30)
project(BuildConnector)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS json)

add_executable(buildcon
        main.cpp
        parsing.cpp
        variables.cpp
        compile.cpp
)

target_link_libraries(buildcon PRIVATE Boost::json)