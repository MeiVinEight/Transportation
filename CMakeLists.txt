cmake_minimum_required(VERSION 3.26)
project(Transportation)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)
aux_source_directory(src SOURCE)
add_definitions(-DCMAKE_BUILD)

link_directories(
	lib
	.release
)

add_executable(Transportation ${SOURCE})
target_link_libraries(Transportation
	Memory.lib
	String.lib
	Streaming.lib
	Filesystem.lib
)