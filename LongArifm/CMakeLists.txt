cmake_minimum_required(VERSION 3.26)
project(LongArifm)

set(CMAKE_CXX_STANDARD 20)

add_executable(LongArifm main.cpp
        src/BigInt.cpp
        src/exceptions.cpp
        test/Test.cpp)
