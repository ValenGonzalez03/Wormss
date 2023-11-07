cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Worms LANGUAGES CXX)
find_package(box2d REQUIRED)

# add compilation flags
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

# define targets
add_executable(server ./src/server/server_main.cpp 
					  ./src/server/server_server.cpp 
					  ./src/server/server_accept_thread.cpp
					  ./src/server/server_player.cpp
					  ./src/server/player_sender_thread.cpp
					  ./src/server/client_handler_thread.cpp
					  ./src/server/worm.cpp
					  ./src/server/beam.cpp
					  ./src/server/server_games_handler.cpp
					  ./src/server/server_game.cpp
					  ./src/server/commands/command.cpp 
					  ./src/common/socket.cpp 
					  ./src/common/liberror.cpp 
					  ./src/common/resolver.cpp 
					  ./src/common/resolvererror.cpp)

target_link_libraries (server PUBLIC box2d)
