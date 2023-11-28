cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Worms LANGUAGES CXX)
find_package(box2d REQUIRED)
find_package(SDL2pp REQUIRED)
find_package(yaml-cpp REQUIRED)

# add compilation flags
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DRESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")

include_directories(${SDL2PP_INCLUDE_DIRS})


# Agregar un ejecutable llamado "client" y especificar los archivos fuente
add_executable(client ./src/client/client_main.cpp
./src/client/client_client.cpp
./src/client/client_receiver_thread.cpp
./src/client/client_sender_thread.cpp
./src/client/client_worldview.cpp
./src/client/client_worm_view.cpp
./src/client/client_protocol.cpp
./src/common/socket.cpp 
./src/common/liberror.cpp 
./src/common/resolver.cpp 
./src/common/resolvererror.cpp
./src/common/worm_states)


target_link_libraries(client PUBLIC pthread)
target_link_libraries(client PUBLIC SDL2pp::SDL2pp)

# define targets
add_executable(server ./src/server/server_main.cpp 
					  ./src/server/server_server.cpp 
					  ./src/server/server_accept_thread.cpp
					  ./src/server/server_player.cpp
					  ./src/server/player_sender_thread.cpp
					  ./src/server/client_handler_thread.cpp
					  ./src/server/server_games_handler.cpp
					  ./src/server/server_game.cpp
					  ./src/server/game_manager.cpp
					  ./src/server/broadcaster.cpp
					  ./src/server/world.cpp
					  ./src/server/worm_body.cpp
					  ./src/server/beam_body.cpp
					  ./src/server/server_protocol.cpp
					  ./src/server/contact_listener.cpp
					  ./src/common/socket.cpp 
					  ./src/common/liberror.cpp 
					  ./src/common/resolver.cpp 
					  ./src/common/resolvererror.cpp
					  ./src/common/worm_states.cpp)

target_link_libraries (server PUBLIC box2d)
target_link_libraries(server PUBLIC pthread)
target_link_libraries(server PUBLIC yaml-cpp)

# Agregar el ejecutable de pruebas y especificar los archivos fuente
#add_executable(tests ./tests/protocol_tests.cpp
#					./src/common/socket.cpp 
#					./src/common/liberror.cpp 
#					./src/common/resolver.cpp 
#					./src/common/resolvererror.cpp
#					./src/common/protocol.cpp)

# Enlazar con Catch2
#find_package(Catch2 REQUIRED)
#target_link_libraries(tests PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)


############################################
#find_package(Catch2 CONFIG REQUIRED)
#include(CTest)
#include(Catch)
#catch_discover_tests(tests)
###########################################
