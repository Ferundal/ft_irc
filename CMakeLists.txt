cmake_minimum_required(VERSION 3.16)
project(ircserc)

set(CMAKE_CXX_STANDARD 98)

include_directories(.)


AUX_SOURCE_DIRECTORY(src SRC)

add_executable(ircserc
        ${SRC}
        )
