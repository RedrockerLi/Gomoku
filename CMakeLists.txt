cmake_minimum_required(VERSION 3.15)

project(gomoku)

include_directories(${PROJECT_SOURCE_DIR}/Inc)

aux_source_directory(${PROJECT_SOURCE_DIR}/Src SRC_LIST)
add_executable(main ${SRC_LIST})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -pg -fstack-protector")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -fstack-protector")