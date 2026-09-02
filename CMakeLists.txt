cmake_minimum_required(VERSION 3.10)
project(EntropyTest)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/headers)


add_executable(main
	main.cpp
	cpp/entropy.cpp
	cpp/functions.cpp
	cpp/sha_fan.cpp
	cpp/huffman.cpp
    )