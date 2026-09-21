#minimum required version of cmake (i guessed)
cmake_minimum_required(VERSION 3.10)

#project name!
project("Photon 7")

#set standard compiler version of c++ (i guessed again)
set(CMAKE_CXX_STANDARD 14)

#set SOURCE_FILES to the list of files we are compiling
file(GLOB SOURCE_FILES "./src/*.cpp" "./src/*.h" "./src/udp/*.c")

#create exe
add_executable(main ${SOURCE_FILES})

#include cpptk files
target_include_directories(main PRIVATE "./lib/cpptk-1.0.2/")
target_include_directories(main PRIVATE "./lib/cpptk-1.0.2/base/")
