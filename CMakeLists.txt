#minimum required version of cmake
cmake_minimum_required(VERSION 3.15)

#project name!
project("Photon_7")

#set standard compiler version of c++
set(CMAKE_CXX_STANDARD 14)

#set source (.c, .cc, .cpp) and header files
file(GLOB CPPTK_SOURCE "lib/cpptk-1.0.2/*.cc" "lib/cpptk-1.0.2/base/*.cc")
file(GLOB SOURCE_FILES "src/*.cpp" "src/udp/*.c")
file(GLOB HEADER_FILES "src/*.h")

#create main.exe
add_executable(main ${SOURCE_FILES} ${HEADER_FILES} ${CPPTK_SOURCE})

#link dependency headers
target_include_directories(main PRIVATE "lib/cpptk-1.0.2/")

#link tcl8.6 and tk8.5 libraries
target_link_libraries(main PRIVATE tcl8.6)
target_link_libraries(main PRIVATE tk8.6)
target_link_libraries(main PRIVATE pqxx)
