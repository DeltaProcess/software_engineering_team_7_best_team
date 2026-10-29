#minimum required version of cmake
cmake_minimum_required(VERSION 3.15)

#project name!
project("Photon_7")

#set standard compiler version of c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#openCV and threads packages
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

#set source (.c, .cc, .cpp) and header files
file(GLOB CPPTK_SOURCE "lib/cpptk-1.0.2/*.cc" "lib/cpptk-1.0.2/base/*.cc")
file(GLOB MINIAUDIO_SOURCE "lib/miniaudio-0.11.25/*.c" "lib/miniaudio-0.11.25/*.h")
file(GLOB SOURCE_FILES "src/*.cpp" "src/udp/*.cpp")
file(GLOB HEADER_FILES "src/*.h" "src/udp/*.h")

#create main.exe
add_executable(main ${SOURCE_FILES} ${HEADER_FILES} ${CPPTK_SOURCE} ${MINIAUDIO_SOURCE})

#link dependency headers
target_include_directories(main PRIVATE 
	"lib/cpptk-1.0.2/"
	"/usr/include/tcl/"
	"/usr/include/tk/"
	"/usr/include/opencv4/"
)

#link libraries
target_link_libraries(main PRIVATE dl tcl tk pqxx ${OpenCV_LIBS} Threads::Threads)
