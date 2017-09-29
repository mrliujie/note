cmake_minimum_required(VERSION 2.8)

# c++11 required
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c++11")
project( debug )

include_directories(inc)
add_executable( debug test.cpp utm.cpp CSV_Reader.cpp
	reader.cpp
)

target_link_libraries(debug)
