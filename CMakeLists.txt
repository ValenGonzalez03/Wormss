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
add_executable(client 
				./src/client/client_main.cpp
				./src/client/client_client.cpp
				./src/client/client_lobby.cpp
				./src/client/client_resource_pool.cpp
				./src/client/comm/client_receiver_thread.cpp
				./src/client/comm/client_sender_thread.cpp
				./src/client/comm/client_protocol.cpp
				./src/client/view/client_worldview.cpp
				./src/client/view/client_camera.cpp
				./src/client/view/client_beam.cpp
				./src/client/view/client_worm.cpp
				./src/client/view/client_explodable.cpp
				./src/client/view/client_explosion.cpp
				./src/client/weapons/client_bazooka.cpp
				./src/client/weapons/client_bat.cpp
				./src/client/weapons/client_grenade.cpp
				./src/common/lib/socket.cpp 
				./src/common/lib/liberror.cpp 
				./src/common/lib/resolver.cpp 
				./src/common/lib/resolvererror.cpp
)


target_link_libraries(client PUBLIC pthread)
target_link_libraries(client PUBLIC SDL2pp::SDL2pp)

# define targets
add_executable(server 
					  ./src/server/server_main.cpp 
					  ./src/server/server_server.cpp 
					  ./src/server/comm/server_accept_thread.cpp
					  ./src/server/comm/client_manager_thread.cpp
					  ./src/server/comm/server_player.cpp
					  ./src/server/comm/player_sender_thread.cpp
					  ./src/server/comm/server_receiver_thread.cpp
					  ./src/server/comm/broadcaster.cpp
					  ./src/server/comm/server_protocol.cpp
					  ./src/server/game/server_games_handler.cpp
					  ./src/server/game/worlds_reader.cpp
					  ./src/server/game/game_manager.cpp
					  ./src/server/game/server_game.cpp
					  ./src/server/bodies/body.cpp
					  ./src/server/bodies/dynamic_body.cpp
					  ./src/server/bodies/static_body.cpp
					  ./src/server/bodies/worm_body.cpp
					  ./src/server/bodies/explodable.cpp
					  ./src/server/bodies/missile_body.cpp
					  ./src/server/bodies/grenade_body.cpp
					  ./src/server/bodies/beam_body.cpp
					  ./src/server/world/explosion_callback.cpp
					  ./src/server/world/server_explosion.cpp
					  ./src/server/world/server_world.cpp
					  ./src/server/world/baseball_bat_callback.cpp
					  ./src/server/world/contact_listener.cpp
					  ./src/server/world/query_callback.cpp
					  ./src/server/world/trajectory_missile_callback.cpp
					  ./src/common/lib/socket.cpp 
					  ./src/common/lib/liberror.cpp 
					  ./src/common/lib/resolver.cpp 
					  ./src/common/lib/resolvererror.cpp
)

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
