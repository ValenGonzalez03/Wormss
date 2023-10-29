cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Worms LANGUAGES CXX)

# add compilation flags
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

# define targets
add_executable(server ./src/server/server_main.cpp 
					  ./src/server/server_server.cpp 
					  ./src/server/server_aceptador.cpp
					  ./src/server/player.cpp
					  ./src/common/socket.cpp 
					  ./src/common/liberror.cpp 
					  ./src/common/resolver.cpp 
					  ./src/common/resolvererror.cpp)
