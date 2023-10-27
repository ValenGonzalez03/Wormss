cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Worms LANGUAGES CXX)

# add compilation flags
set(CMAKE_CXX_STANDARD 17)

# define targets
add_executable(server ./src/server/server_main.cpp ./src/server/server_server.cpp)
