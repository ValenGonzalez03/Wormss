cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Worms LANGUAGES CXX)
find_package(box2d REQUIRED)
find_package(SDL2pp REQUIRED)
find_package(yaml-cpp REQUIRED)

# add compilation flags
set(CMAKE_CXX_STANDARD 17)
#add_definitions(-DRESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")

include_directories(${SDL2PP_INCLUDE_DIRS})

# Agregar un ejecutable llamado "client" y especificar los archivos fuente
add_executable(client ./src/client/client_main.cpp
./src/client/client_client.cpp
./src/client/client_receiver_thread.cpp
./src/client/client_sender_thread.cpp
./src/client/client_worldview.cpp
./src/client/client_worm.cpp
./src/client/client_missile.cpp
./src/client/client_explosion.cpp
./src/client/client_protocol.cpp
./src/client/client_beam.cpp
./src/client/client_lobby.cpp
./src/client/client_resource_pool.cpp
./src/common/lib/socket.cpp 
./src/common/lib/liberror.cpp 
./src/common/lib/resolver.cpp 
./src/common/lib/resolvererror.cpp)


target_link_libraries(client PUBLIC pthread)
target_link_libraries(client PUBLIC SDL2pp::SDL2pp)

# define targets
add_executable(server ./src/server/server_main.cpp 
					  ./src/server/server_server.cpp 
					  ./src/server/server_accept_thread.cpp
					  ./src/server/client_manager_thread.cpp
					  ./src/server/server_player.cpp
					  ./src/server/player_sender_thread.cpp
					  ./src/server/server_receiver_thread.cpp
					  ./src/server/server_games_handler.cpp
					  ./src/server/server_game.cpp
					  ./src/server/game_manager.cpp
					  ./src/server/broadcaster.cpp
					  ./src/server/explosion_callback.cpp
					  ./src/server/server_explosion.cpp
					  ./src/server/world.cpp
					  ./src/server/bodies/body.cpp
					  ./src/server/bodies/worm_body.cpp
					  ./src/server/bodies/missile_body.cpp
					  ./src/server/bodies/beam_body.cpp
					  #./src/server/bodies/water_body.cpp
					  #./src/server/bodies/weapons/bazooka.cpp
					  #./src/server/bodies/weapons/banana.cpp
					  #./src/server/bodies/weapons/dynamite.cpp
					  ./src/server/server_protocol.cpp
					  ./src/server/contact_listener.cpp
					  ./src/server/query_callback.cpp
					  ./src/common/lib/socket.cpp 
					  ./src/common/lib/liberror.cpp 
					  ./src/common/lib/resolver.cpp 
					  ./src/common/lib/resolvererror.cpp)

target_link_libraries (server PUBLIC box2d)
target_link_libraries(server PUBLIC pthread)
target_link_libraries(server PUBLIC yaml-cpp)


add_executable(editor ./level_editor/main.cpp
./level_editor/lobby.cpp
./level_editor/file_editor.cpp
./level_editor/position_converter.cpp
./level_editor/level_editor.cpp
./level_editor/world.cpp)

target_link_libraries(editor PUBLIC yaml-cpp)
target_link_libraries(editor PUBLIC SDL2pp::SDL2pp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_compile_definitions(client PUBLIC RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")
target_compile_definitions(server PUBLIC RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")
target_compile_definitions(editor PUBLIC RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")
