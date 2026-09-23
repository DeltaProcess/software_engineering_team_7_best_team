#minimum required version of cmake (i guessed)
cmake_minimum_required(VERSION 3.10)

#project name!
project("Photon 7")

#set standard compiler version of c++ (i guessed again)
set(CMAKE_CXX_STANDARD 14)

#set SOURCE_FILES and HEADER_FILES
file(GLOB SOURCE_FILES "./src/*.cpp" "./src/udp/*.c" "./lib/cpptk-1.0.2/*.cc" "./lib/cpptk-1.0.2/base/*.cc")
file(GLOB HEADER_FILES "./src/*.h")

#create exe
add_executable(main ${SOURCE_FILES} ${HEADER_FILES})

#include cpptk files
target_include_directories(main PRIVATE "./lib/cpptk-1.0.2/")
target_include_directories(main PRIVATE "./lib/cpptk-1.0.2/base/")
